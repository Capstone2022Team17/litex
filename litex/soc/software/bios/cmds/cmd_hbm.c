#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libbase/memtest.h>

#include <generated/soc.h>
#include <generated/csr.h>
#include <generated/mem.h>
#include <libbase/i2c.h>



#include "../command.h"
#include "../helpers.h"




// void hbm_set_writing_ports(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask, uint32_t delay_cycles);
// void hbm_set_reading_ports(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask, uint32_t delay_cycles);




// #ifdef CSR_HBM_31_BASE

#include <libbase/hbm_test.h>

/**
 * Command *hbm_read_set*
 * 
 * Set ports up for reading test
 * Note: Manual for HBM controller says that lower 5 bits are not used. 
 * 
*/
static void hbm_read_set(int nb_params, char **params)
{
    char *c;
    uint32_t bytes;
    uint32_t delay_cycles = 0;
    uint32_t port_mask = 0xffffffff;
	uint32_t address = 0x0;
    uint32_t addr_offset = 0x800000;
	if (nb_params < 1) {
		printf("hbm_read \n"
               "<bytes: Number of bytes to write (continuously)>\n"
               "[port mask: 32-bit signal, each bit representing a port to enable (default=0xffffffff)]\n"
               "[delay val: 32-bit signal, number of clock cycles to delay reads/writes(default=0)]\n"
               "[address: 28-bit signal (<< 5), starting address of beginning port (default=0x0)]\n"
               "[address offset: 28-bit signal (<< 5), incremented address for every other port (default=0x800000)]\n"
               "[data pattern: 32-bit signal, data pattern to write (default=0xa5a5a5a5)]\n"
               "\nNote: Addresses in fsm are 33 bit signals, lower 5 bits are not used.\n");
		return;
	}
	bytes = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect bytes");
		return;
	}
    if (nb_params > 1) {
		delay_cycles = strtoul(params[1], &c, 0);
		if (*c != 0) {
			printf("Incorrect delay_cycles");
			return;
		}
	}
	if (nb_params > 2) {
		port_mask = strtoul(params[2], &c, 0);
		if (*c != 0) {
			printf("Incorrect port_mask");
			return;
		}
	}
    if (nb_params > 3) {
		address = strtoul(params[3], &c, 0);
		if (*c != 0) {
			printf("Incorrect address");
			return;
		}
	}
    if (nb_params > 4) {
		addr_offset = strtoul(params[4], &c, 0);
		if (*c != 0) {
			printf("Incorrect addr_offset");
			return;
		}
	}

	hbm_set_reading_ports(address, addr_offset, bytes, port_mask, delay_cycles);
}
define_command(hbm_read_set, hbm_read_set, "Set CSRS for reading test", HBM_CMDS);

/**
 * Command *hbm_write_set*
 * 
 * Set ports up for writing test
 * 
*/
static void hbm_write_set(int nb_params, char **params)
{
    char *c;
    uint32_t bytes;
    uint32_t delay_cycles = 0;
    uint32_t port_mask = 0xffffffff;
	uint32_t address = 0x0;
    uint32_t addr_offset = 0x800000;
    uint32_t data_pattern = 0xa5a5a5a5;
	if (nb_params < 1) {
		printf("hbm_read \n"
               "<bytes: Number of bytes to write (continuously)>\n"
               "[port mask: 32-bit signal, each bit representing a port to enable (default=0xffffffff)]\n"
               "[delay val: 32-bit signal, number of clock cycles to delay reads/writes(default=0)]\n"
               "[address: 28-bit signal (<< 5), starting address of beginning port (default=0x0)]\n"
               "[address offset: 28-bit signal (<< 5), incremented address for every other port (default=0x800000)]\n"
               "[data pattern: 32-bit signal, data pattern to write (default=0xa5a5a5a5)]\n"
               "\nNote: Addresses in fsm are 33 bit signals, lower 5 bits are not used.\n");
		return;
	}
	bytes = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect bytes");
		return;
	}
    if (nb_params > 1) {
		delay_cycles = strtoul(params[1], &c, 0);
		if (*c != 0) {
			printf("Incorrect delay_cycles");
			return;
		}
	}
	if (nb_params > 2) {
		port_mask = strtoul(params[2], &c, 0);
		if (*c != 0) {
			printf("Incorrect port_mask");
			return;
		}
	}
    if (nb_params > 3) {
		address = strtoul(params[3], &c, 0);
		if (*c != 0) {
			printf("Incorrect address");
			return;
		}
	}
    if (nb_params > 4) {
		addr_offset = strtoul(params[4], &c, 0);
		if (*c != 0) {
			printf("Incorrect addr_offset");
			return;
		}
	}
    if (nb_params > 5) {
		data_pattern = strtoul(params[5], &c, 0);
		if (*c != 0) {
			printf("Incorrect data_pattern");
			return;
		}
	}

	hbm_set_writing_ports(address, addr_offset, bytes, port_mask, delay_cycles, data_pattern);
}
define_command(hbm_write_set, hbm_write_set, "Set CSRS for writing test", HBM_CMDS);

/**
 * Command *hbm_start*
 * 
 * Start the port tests all at once
 * 
*/
define_command(hbm_test_start, hbm_test_start, "Start all HBM tests", HBM_CMDS);

/**
 * Command *hbm_finish*
 * 
 * Stop all state machines. 
 * 
*/
define_command(hbm_test_finish, hbm_test_finish, "Stop all HBM tests", HBM_CMDS);

// /**
//  * 
//  * Command *hbm_cycles_dependent_bandwidth*
//  * 
//  * Get cycles-dependent bandwidth
// */
// define_command(hbm_cycles_dependent_bandwidth, hbm_cycles_dependent_bandwidth, "Get cycles-dependent bandwidth", HBM_CMDS);

// /**
//  * 
//  * Command *hbm_data_dependent_bandwidth*
//  * 
//  * Get data-dependent bandwidth
// */
// define_command(hbm_data_dependent_bandwidth, hbm_data_dependent_bandwidth, "Get data-dependent bandwidth", HBM_CMDS);

/**
 * 
 * Command *hbm_data_dependent_bandwidth*
 * 
 * Get data-dependent bandwidth
*/
define_command(hbm_bandwidth, hbm_bandwidth, "Get bandwidth", HBM_CMDS);

// #endif //CSR_HBM_31_BASE











// /**
//  * Command *read_hbm_test*
//  * 
//  * Run a read to hbm once
//  * 
// */

// #if defined(CSR_HBM_4_BASE)
// static void hbm_read_hbm_test(int nb_params, char **params) 
// {
// 	char *c;
// 	int address;
// 	if (nb_params < 1) {
// 		printf("hbm_read <address>");
// 		return;
// 	}
// 	address = strtoul(params[0], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect address");
// 		return;
// 	}

// 	hbm_read_fsm(address);
// }
// define_command(hbm_read_fsm, hbm_read_hbm_test, "Read once to hbm", HBM_CMDS);
// #endif

// /**
//  * Command *write_hbm_test"
//  * 
//  * Run a write to hbm once
//  * 
//  *
// */
// #if defined(CSR_HBM_4_BASE)
// static void hbm_test_readwrite(int nb_params, char **params)
// {
// 	char *c;
// 	int address;
// 	int addr_offset;
// 	uint32_t port_quantity = 0xffffffff;
// 	uint32_t num_bytes = 1;


// 	if (nb_params < 2) {
// 		printf("hbm_test\n
// 		<address: Beginning address to write to>\n
// 		<addr_offset: Address each port writes to>\n
// 		[port_quantity=0xffffffff]\n
// 		[num_bytes=1]\n");
// 		return;
// 	} 
// 	address = strtoul(params[0], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect address");
// 		return;
// 	}
// 	addr_offset = strtoul(params[1], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect address offset");
// 		return;
// 	}
// 	if (nb_params > 2) {
// 		port_quantity = strtoul(params[2], &c, 0);
// 		if (*c != 0) {
// 			printf("Incorrect port_quantity");
// 			return;
// 		}
// 	}
// 	if (nb_params > 3) {
// 		num_bytes = strtoul(params[3], &c, 0);
// 		if (*c != 0) {
// 			printf("Incorrect num_bytes");
// 			return;
// 		}
// 	}
// 	//void hbm_test(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t port_quantity)
// 	hbm_test(address, addr_offset, num_bytes, port_quantity);
// }
// define_command(hbm_test, hbm_test_readwrite, "Write multiple times to hbm", HBM_CMDS);
// #endif


// ///////////////////////////////////////////////////////////////////// newer (old) state machine

// /**
//  * 
//  * Command hbm_write
//  * 
//  * Run the hbm writer
// */
// static void hbm_writer_handler(int nb_params, char **params)
// {
// 	char *c;
// 	int port_count;
// 	int addr_offset_port = 0x800000;
// 	int addr_base = 0x0;
// 	int num_bytes = 32;
// 	int addr_length = 0xff;
// 	int pattern = 0xa5a5a5a5;
// 	int random_flag = 0;

// 	if (nb_params < 1) {
// 		printf("hbm_write \n<port_count : Number of ports to use>\n");
// 		printf("<addr_offset_port : Address spacing between ports(default=0x800000)>\n");
// 		printf("<addr_base : Starting address for first port(default=0x0)>\n");
// 		printf("<num_bytes : Number of bytes to write(default=32)>\n");
// 		printf("<addr_length : Number of addresses after base(default=0xff)>\n");
// 		printf("<pattern : Data pattern to write(default=0xa5a5a5a5)>\n");
// 		printf("<random : Write random data(default=0)>\n");
// 		return;
// 	} 
// 	port_count = strtoul(params[0], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect port_count");
// 		return;
// 	}
// 	if (nb_params > 1) {
// 		addr_offset_port = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect addr_offset_port value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 2) {
// 		addr_base = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect addr_base value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 3) {
// 		num_bytes = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect num_bytes value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 4) {
// 		addr_length = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect addr_length value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 5) {
// 		pattern = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect pattern value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 6) {
// 		random_flag = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect random_flag value");
// 			return;
// 		}
// 	}

// 	hbm_write(port_count, addr_offset_port, addr_base, num_bytes, addr_length, pattern, random_flag);
// }
// define_command(hbm_write, hbm_writer_handler, "hbm write test", HBM_CMDS);

// /**
//  * 
//  * Command hbm_test
//  * 
//  * Run the hbm read/write test
// */
// static void hbm_tester_handler(int nb_params, char **params)
// {
// 	char *c;
// 	int port_count;
// 	int addr_offset_port = 0x800000;
// 	int addr_base = 0x0;
// 	int num_bytes = 32;
// 	int addr_length = 0xff;
// 	int pattern = 0xa5a5a5a5;
// 	int random_flag = 0;

// 	if (nb_params < 1) {
// 		printf("hbm_test \n<port_count : Number of ports to use>\n");
// 		printf("<addr_offset_port : Address spacing between ports(default=0x800000)>\n");
// 		printf("<addr_base : Starting address for first port(default=0x0)>\n");
// 		printf("<num_bytes : Number of bytes to write(default=32)>\n");
// 		printf("<addr_length : Number of addresses after base(default=0xff)>\n");
// 		printf("<pattern : Data pattern to write(default=0xa5a5a5a5)>\n");
// 		printf("<random : Write random data(default=0)>\n");
// 		return;
// 	} 
// 	port_count = strtoul(params[0], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect port_count");
// 		return;
// 	}
// 	if (nb_params > 1) {
// 		addr_offset_port = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect addr_offset_port value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 2) {
// 		addr_base = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect addr_base value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 3) {
// 		num_bytes = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect num_bytes value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 4) {
// 		addr_length = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect addr_length value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 5) {
// 		pattern = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect pattern value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 6) {
// 		random_flag = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect random_flag value");
// 			return;
// 		}
// 	}

// 	hbm_test(port_count, addr_offset_port, addr_base, num_bytes, addr_length, pattern, random_flag);
// }
// define_command(hbm_test, hbm_tester_handler, "hbm read/write test", HBM_CMDS);

// /**
//  * 
//  * Command hbm_test
//  * 
//  * Run the hbm read/write test
// */
// static void hbm_read_handler(int nb_params, char **params)
// {
// 	char *c;
// 	int port_count;
// 	int addr_offset_port = 0x800000;
// 	int addr_base = 0x0;
// 	int num_bytes = 32;
// 	int addr_length = 0xff;

// 	if (nb_params < 1) {
// 		printf("hbm_read \n<port_count : Number of ports to use>\n");
// 		printf("<addr_offset_port : Address spacing between ports(default=0x800000)>\n");
// 		printf("<addr_base : Starting address for first port(default=0x0)>\n");
// 		printf("<num_bytes : Number of bytes to write(default=32)>\n");
// 		printf("<addr_length : Number of addresses after base(default=0xff)>\n");
// 		return;
// 	} 
// 	port_count = strtoul(params[0], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect port_count");
// 		return;
// 	}
// 	if (nb_params > 1) {
// 		addr_offset_port = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect addr_offset_port value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 2) {
// 		addr_base = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect addr_base value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 3) {
// 		num_bytes = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect num_bytes value");
// 			return;
// 		}
// 	}
// 	if (nb_params > 4) {
// 		addr_length = strtoul(params[1], &c, 0) != 0;
// 		if (*c != 0) {
// 			printf("Incorrect addr_length value");
// 			return;
// 		}
// 	}

// 	hbm_read(port_count, addr_offset_port, addr_base, num_bytes, addr_length);
// }
// define_command(hbm_read, hbm_read_handler, "hbm read test", HBM_CMDS);

/**
 * Command "hbm_gen_tester"
 * 
 * Run the generator once
 * 
*/
// #if defined(CSR_HBM_GENERATOR_BASE)
// static void hbm_gen_tester_handler(int nb_params, char **params)
// {
// 	char *c;
// 	int burst_length;
// 	// Random turned to 0
// 	if (nb_params < 1) {
// 		printf("hbm_gen <burst_length>");
// 		return;
// 	} 
// 	burst_length = strtoul(params[0], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect burst_length");
// 		return;
// 	}
// 	hbm_gen(burst_length);
// }
// define_command(hbm_gen, hbm_gen_tester_handler, "Single test for hbm writer", HBM_CMDS);
// #endif