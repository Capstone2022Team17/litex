import litex.soc.interconnect.axi.axi_lite as al

def main():
    my_interface = al.AXILiteInterface()
    my_parts = list(my_interface.write(0x0, 0x0))
    print(my_parts)



main()