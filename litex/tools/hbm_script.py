#!/usr/bin/env python3

from litex.tools.litex_term import CrossoverUART
import sys
import os
import serial
import argparse

class Scripter:
    def __init__(self, channel, port, filein=None, fileout=None):
        self.channel = channel
        self.port = port
        self.filein = filein
        self.fileout = fileout
        self.string_buffer = ""
        self.num_buffer = []
        if self.fileout:
            self.outfile = open(self.fileout, "w", encoding="utf-8")

    def write(self, command):
        str_cmd = command.decode('ascii')
        for i in range(len(str_cmd)):
                self.channel.rxtx.write(ord(str_cmd[i])) 

    def read(self):
        c = self.port.read()
        if (len(c) == 0):
            return False
        if self.fileout:
            str_c = c.decode("utf-8")
            self.string_buffer += str_c
            if str_c == "\n":
                parts = self.string_buffer.split(":")
                if (len(parts) > 1):
                    if (len(self.num_buffer) == 2):
                        subparts = parts[0].split(" ")
                        self.num_buffer.append(int(subparts[-1]))
                    subparts = parts[1].split(" ")
                    self.num_buffer.append(int(subparts[1]))

                if (len(self.num_buffer) == 4):
                    output = "".join(f"{i}," for i in self.num_buffer)[:-1] + "\n"        
                    self.outfile.write(output)
                    self.num_buffer.clear()

                self.string_buffer = ""
        sys.stdout.buffer.write(c)
        sys.stdout.flush()
        return True

    def processInput(self):
        outList = []
        with open(self.filein, encoding="utf-8") as f:
            for line in f:
                print(line)
                outList.append(line.encode("ascii"))
        return outList

    def closeOut(self):
        self.outfile.close()

parser = argparse.ArgumentParser(prog="Python script for processing hbm BIOS commands")

parser.add_argument("mode", choices=["file", "gui", "args", "default"], default="default")
parser.add_argument("-c", "--cmd", default=None, nargs=3)
parser.add_argument("--mask", default="0xffffffff")
parser.add_argument("--bytes", default="1")
parser.add_argument("-f", "--filein", type=str, default=None)
parser.add_argument("-o", "--outfile", type=str, default=None)

def main():
    args = parser.parse_args()
    
    if args.filein is None and args.mode == "file":
        print("No file given")
        return

    xover = CrossoverUART(base_address=None, csr_csv=None, name="uart_xover")
    xover.open()
    port = os.ttyname(xover.name)
    serial_port = serial.serial_for_url(url=port, baudrate=115200, timeout=1)
    
    default_command = b'hbm_test 0x0 0x800000 0xffffffff 900\x0a'
    
    script_runner = Scripter(xover, serial_port, args.filein, args.outfile)

    if args.mode == "file":
        cmdList = script_runner.processInput()
    
    elif args.mode == "args" and len(args.cmd) == 3:
        args_command = f'{args.cmd[0]} {args.cmd[1]} {args.cmd[2]} {args.mask} {args.bytes}\n'
        cmdList = [args_command.encode("ascii")]

    elif args.mode == "default":
        cmdList = [default_command]

    else:
        cmdList = None

    for command in cmdList:
            script_runner.write(command)
            reading = True
            while reading:
                reading = script_runner.read()      

    if args.outfile:
        script_runner.closeOut()

    print("Done!")
    xover.close()

if __name__ == "__main__":
    main()
