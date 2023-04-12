#!/usr/bin/env python3

import time
from litex.tools.litex_term import CrossoverUART
import sys
import os
import serial
import argparse
import webbrowser


import dearpygui.dearpygui as dpg
import csv

class ValueHolder:
    def __init__(self, addr, offset, mask, bytes, delay, pattern):
        self.addr = addr
        self.offset = offset
        self.mask = mask
        self.bytes = bytes
        self.delay = delay
        self.pattern = pattern
        self.data = []
        self.collecting = False
        self.run_count = 0
        
    def run_cmd(self, sender, app_data, scripter):
        cmd = dpg.get_item_label(sender)
        bytes_field = dpg.get_value(self.bytes)
        mask = dpg.get_value(self.mask)
        delay = dpg.get_value(self.delay)
        addr = dpg.get_value(self.addr)
        offset = dpg.get_value(self.offset)
        pattern = dpg.get_value(self.pattern)
        if bytes_field == "":
            print("No bytes given...")
            return
        if mask == "":
            mask = "0xffffffff"
        if delay == "":
            delay = "0"
        if addr == "":
            addr = "0x0"
        if offset == "":
            offset = "0x800000"
        else:
            num_offset = int(offset, 16)
            num_offset >>= 5
            offset = hex(num_offset)
        if pattern == "":
            pattern = "0xa5a5a5a5"
        str_cmd = f"{cmd} {bytes_field} {mask} {delay} {addr} {offset} {pattern}\n"
        command = str_cmd.encode("ascii")
        scripter.write(command)
        reading = True
        while reading:
            reading = scripter.read(self) 

    def start_test(self, sender, app_data, scripter):
        str_cmd = f"hbm_test_start\n"
        command = str_cmd.encode("ascii")
        scripter.write(command)
        reading = True
        while reading:
            reading = scripter.read(self)

    def get_data(self, sender, app_data, scripter):
        str_cmd = f"hbm_bandwidth\n"
        command = str_cmd.encode("ascii")
        scripter.write(command)
        reading = True
        while reading:
            reading = scripter.read(self)
        self.updateSeries()
        self.run_count += 1

    def finish_test(self, sender, app_data, scripter):
        str_cmd = f"hbm_test_finish\n"
        command = str_cmd.encode("ascii")
        scripter.write(command)
        reading = True
        while reading:
            reading = scripter.read(self)

    def auto_collect(self, sender, app_data, scripter):
        auto_range = 10
        for i in range(auto_range):
            dpg.set_item_label("collector", f"Collecting...({i+1}/{auto_range})")
            self.get_data(sender, app_data, scripter)
            time.sleep(6)
        dpg.set_item_label("collector", "Start Collecting")


    def writeData(self, data):
        self.data.append(data + [self.run_count])

    def updateSeries(self):
        which_graph = dpg.get_value('graph_radio')
        x_vals = []
        y_vals = []
        if which_graph == "Total":
            counter = 0
            index = 0
            while counter < len(self.data):
                value = 0
                curr_data = [i for i in self.data if i[-1] == index]
                for val in curr_data:
                    value += max([int(val[4]), int(val[5])])
                counter += len(curr_data)
                x_vals.append(index)
                y_vals.append(value/1024)
                index += 1
        else:
            counter = 0
            for i in self.data:
                if i[0] == int(which_graph):
                    x_vals.append(counter)
                    y_vals.append(max([int(i[4]), int(i[5])])/1024)
                    counter += 1
        if counter:       
            dpg.set_value('series_tag', [x_vals, y_vals])
            dpg.set_item_label('plot', f'Throughput {which_graph}')
            dpg.set_axis_limits("x_axis", max([max(x_vals)-10, 0]), max(x_vals))
            dpg.set_axis_limits("y_axis", 0, int(max(y_vals)*1.3))

def open_help_link():
    webbrowser.open("https://github.com/Capstone2022Team17/drgbl/wiki")

class Scripter:
    def __init__(self, channel, port, filein=None, fileout=None, gui_on=False):
        self.channel = channel
        self.port = port
        self.filein = filein
        self.fileout = fileout
        self.string_buffer = ""
        self.num_buffer = []
        self.gui_on = gui_on
        if self.fileout:
            self.outfile = open(self.fileout, "w", encoding="utf-8")

    def write(self, command):
        str_cmd = command.decode('ascii')
        words = str_cmd.split(" ")
        if words[0] == "wait" and len(words) == 2:
            print(f"Waiting {words[1][:-1]} seconds")
            time.sleep(int(words[1]))
            return
        for i in range(len(str_cmd)):
                self.channel.rxtx.write(ord(str_cmd[i])) 

    def read(self, gui_runner=None):
        c = self.port.read()
        if (len(c) == 0):
            return False
        if self.fileout or self.gui_on:
            str_c = c.decode("utf-8")
            if str_c == " ": 
                if len(self.string_buffer) and self.string_buffer.isnumeric():      
                    self.num_buffer.append(self.string_buffer+",")
                    self.string_buffer = ""
                else:
                    self.string_buffer = ""
            elif str_c == "\n":
                if (len(self.num_buffer) == 5):
                    self.num_buffer.append(self.string_buffer+"\n")
                    if self.fileout:
                        for num in self.num_buffer:
                            self.outfile.write(num)
                    if self.gui_on:
                        gui_runner.writeData([int(digit[:-1]) for digit in self.num_buffer])
                self.string_buffer = ""
                self.num_buffer.clear()
            else:
                self.string_buffer += str_c

        sys.stdout.buffer.write(c)
        sys.stdout.flush()
        return True

    def processInput(self):
        outList = []
        with open(self.filein, encoding="utf-8") as f:
            for line in f:
                outList.append(line.encode("ascii"))
        return outList

    def closeOut(self):
        self.outfile.close()

parser = argparse.ArgumentParser(prog="Python script for processing hbm BIOS commands")

parser.add_argument("mode", choices=["file", "gui"], default="default")
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
        
    script_runner = Scripter(xover, serial_port, args.filein, args.outfile, (args.mode=="gui"))

    if args.mode == "gui":
        dpg.create_context()
        dpg.create_viewport(width=610, height=700)
        dpg.setup_dearpygui()

        with dpg.window(tag="Primary Window", label="HBM Testing"):
            
            with dpg.group(horizontal=True):
                with dpg.group():
                    bytes_field = dpg.add_input_text(label="# Bytes", tag="bytes_text")
                    mask = dpg.add_input_text(label="Port Mask", tag="mask_text")

                width, height, channels, data = dpg.load_image("litex/litex/tools/icon.png")
                with dpg.texture_registry(show=False):
                    dpg.add_static_texture(width=width, height=height, default_value=data, tag="texture_tag")
                dpg.add_image_button("texture_tag", callback=open_help_link, height=40, width=40, pos=[550, 0])


            with dpg.group(horizontal=True):
                with dpg.group():    
                    delay = dpg.add_input_text(label="Delay", tag="delay_text") 
                    addr = dpg.add_input_text(label="Starting Address", tag="address_text")
                width, height, channels, data = dpg.load_image("litex/litex/tools/DRGBL_Logo.png")
                with dpg.texture_registry(show=False):
                    dpg.add_static_texture(width=width, height=height, default_value=data, tag="drgbl_logo")
                dpg.add_image("drgbl_logo", height=20, width=40, pos=[554, 50])



            offset = dpg.add_input_text(label="Address Offset", tag="offset_text")
            pattern = dpg.add_input_text(label="Data Pattern", tag="data_text")
            
            
            my_vals = ValueHolder(addr, offset, mask, bytes_field, delay, pattern)
            with dpg.group(horizontal=True):
                dpg.add_button(label="hbm_read_set", callback=my_vals.run_cmd, user_data=script_runner, tag="read_button")
                dpg.add_button(label="hbm_write_set", callback=my_vals.run_cmd, user_data=script_runner, tag="write_button")
            with dpg.group(horizontal=True):    
                dpg.add_button(label="Start", callback=my_vals.start_test, user_data=script_runner, tag="start_button")
                dpg.add_button(label="Get Data", callback=my_vals.get_data, user_data=script_runner, tag="data_button")
                dpg.add_button(label="Finish", callback=my_vals.finish_test, user_data=script_runner, tag="finish_button")
                dpg.add_button(label="Start Collecting", tag="collector", callback=my_vals.auto_collect, user_data=script_runner)


            with dpg.tooltip("bytes_text"):
                dpg.add_text("Number of bytes to read/write per transaction")

            with dpg.tooltip("mask_text"):
                dpg.add_text("Ports to assign to read/write encoded as binary stream")

            with dpg.tooltip("delay_text"):
                dpg.add_text("Delay time between transaction in cycles")

            with dpg.tooltip("address_text"):
                dpg.add_text("Starting address for first read/write")

            with dpg.tooltip("offset_text"):
                dpg.add_text("Address offset between subsequent reads/writes")

            with dpg.tooltip("data_text"):
                dpg.add_text("Data byte pattern for writes")

            with dpg.tooltip("read_button"):
                dpg.add_text("Assign selected ports to read with selected settings")

            with dpg.tooltip("write_button"):
                dpg.add_text("Assign selected ports to write with selected settings")

            with dpg.tooltip("start_button"):
                dpg.add_text("Start transactions on all assigned ports")

            with dpg.tooltip("data_button"):
                dpg.add_text("Pause transactions and sample throughput")

            with dpg.tooltip("finish_button"):
                dpg.add_text("Stop all ports")

            with dpg.tooltip("collector"):
                dpg.add_text("Collect files every six seconds for one minute")

            radio_list = ["Total"]
            radio_list.extend([str(i) for i in range(32)])
            with dpg.child_window(horizontal_scrollbar=True, height=50):
                dpg.add_radio_button(items=tuple(radio_list), tag='graph_radio', default_value="Total", callback=my_vals.updateSeries, horizontal=True)
            # create plot
            with dpg.plot(label="Line Series", height=400, width=400, tag="plot"):
                dpg.add_plot_axis(dpg.mvXAxis, label="Sample #", tag="x_axis")
                dpg.add_plot_axis(dpg.mvYAxis, label="GiB/s", tag="y_axis")
                
                dpg.set_axis_limits("x_axis", 0, 10)
                dpg.set_axis_limits("y_axis", 0, 100000)

                dpg.add_line_series([], [], parent="y_axis", tag="series_tag")
            
        dpg.show_viewport()
        dpg.set_primary_window("Primary Window", True)
        while dpg.is_dearpygui_running():
            dpg.render_dearpygui_frame()
        dpg.destroy_context()

    else:

        if args.mode == "file":
            cmdList = script_runner.processInput()

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
