// SPDX-License-Identifier: BSD-Source-Code

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libbase/memtest.h>

#include <generated/soc.h>
#include <generated/csr.h>
#include <generated/mem.h>
#include <libbase/i2c.h>

#include <liblitedram/sdram.h>
#include <liblitedram/sdram_spd.h>
#include <liblitedram/bist.h>
#include <liblitedram/hbm_bist.h>

#include "../command.h"
#include "../helpers.h"

/**
 * Command "sdram_init"
 *
 * Initialize SDRAM (Init + Calibration)
 *
 */
#if defined(CSR_SDRAM_BASE)
define_command(sdram_init, sdram_init, "Initialize SDRAM (Init + Calibration)", LITEDRAM_CMDS);
#endif

/**
 * Command "sdram_cal"
 *
 * Calibrate SDRAM
 *
 */
#if defined(CSR_SDRAM_BASE) && defined(CSR_DDRPHY_BASE)
static void sdram_cal_handler(int nb_params, char **params)
{
	sdram_software_control_on();
	sdram_leveling();
	sdram_software_control_off();
}
define_command(sdram_cal, sdram_cal_handler, "Calibrate SDRAM", LITEDRAM_CMDS);
#endif

/**
 * Command "sdram_test"
 *
 * Test SDRAM
 *
 */
#if defined(CSR_SDRAM_BASE)
static void sdram_test_handler(int nb_params, char **params)
{
	memtest((unsigned int *)MAIN_RAM_BASE, MAIN_RAM_SIZE/32);
}
define_command(sdram_test, sdram_test_handler, "Test SDRAM", LITEDRAM_CMDS);
#endif

/**
 * Command *read_hbm_test*
 * 
 * Run a read to hbm once
 * 
*/

#if defined(CSR_HBM_4_BASE)
static void hbm_read_hbm_test(int nb_params, char **params) 
{
	char *c;
	int address;
	if (nb_params < 1) {
		printf("hbm_read <address>");
		return;
	}
	address = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect address");
		return;
	}

	hbm_read_fsm(address);
}
define_command(hbm_read_fsm, hbm_read_hbm_test, "Read once to hbm", LITEDRAM_CMDS);
#endif

/**
 * Command *write_hbm_test"
 * 
 * Run a write to hbm once
 * 
 *
*/
#if defined(CSR_HBM_4_BASE)
static void hbm_test_readwrite(int nb_params, char **params)
{
	char *c;
	int address;
	int addr_offset;
	// int data1 = 0;
	// int data2 = 0;
	// int data3 = 0;
	// int data4 = 0;
	// int data5 = 0;
	// int data6 = 0;
	// int data7 = 0;
	// int data8 = 0;
	int strb = 0xffffffff;
	// Random turned to 0
	if (nb_params < 2) {
		printf("hbm_write_fsm <address> <addr_offset> [strb=0xffffffff(default)]");
		return;
	} 
	address = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect address");
		return;
	}
	addr_offset = strtoul(params[1], &c, 0);
	if (*c != 0) {
		printf("Incorrect address offset");
		return;
	}
	// data6 = strtoul(params[1], &c, 0);
	// if (*c != 0) {
	// 	printf("Incorrect data1");
	// 	return;
	// }
	// data5 = strtoul(params[2], &c, 0);
	// if (*c != 0) {
	// 	printf("Incorrect data2");
	// 	return;
	// }
	// data4 = strtoul(params[3], &c, 0);
	// if (*c != 0) {
	// 	printf("Incorrect data3");
	// 	return;
	// }
	// data3 = strtoul(params[4], &c, 0);
	// if (*c != 0) {
	// 	printf("Incorrect data4");
	// 	return;
	// }
	// data2 = strtoul(params[5], &c, 0);
	// if (*c != 0) {
	// 	printf("Incorrect data5");
	// 	return;
	// }
	// data1 = strtoul(params[6], &c, 0);
	// if (*c != 0) {
	// 	printf("Incorrect data6");
	// 	return;
	// }
	

	if (nb_params > 2) {
		strb = strtoul(params[2], &c, 0);
		if (*c != 0) {
			printf("Incorrect strb");
			return;
		}
	}
	hbm_test(address, addr_offset);
}
define_command(hbm_test, hbm_test_readwrite, "Write once to hbm", LITEDRAM_CMDS);
#endif

// /**
//  * Command *write_hbm_test"
//  * 
//  * Run a write to hbm once
//  * 
//  *
// */
// #if defined(CSR_HBM_4_BASE)
// static void hbm_writeread_hbm_test(int nb_params, char **params)
// {
// 	char *c;
// 	int address;
// 	int data1 = 0;
// 	int data2 = 0;
// 	int data3 = 0;
// 	int data4 = 0;
// 	int data5 = 0;
// 	int data6 = 0;
// 	int data7 = 0;
// 	int data8 = 0;
// 	int strb = 0xf;
// 	// Random turned to 0
// 	if (nb_params < 2) {
// 		printf("hbm_write <address> <data x 8> [strb=0xf(default)]");
// 		return;
// 	} 
// 	address = strtoul(params[0], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect address");
// 		return;
// 	}
// 	data1 = strtoul(params[1], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect data1");
// 		return;
// 	}
// 	data2 = strtoul(params[2], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect data2");
// 		return;
// 	}
// 	data3 = strtoul(params[3], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect data3");
// 		return;
// 	}
// 	data4 = strtoul(params[4], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect data4");
// 		return;
// 	}
// 	data5 = strtoul(params[5], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect data5");
// 		return;
// 	}
// 	data6 = strtoul(params[6], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect data6");
// 		return;
// 	}
// 	data7 = strtoul(params[7], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect data7");
// 		return;
// 	}
// 	data8 = strtoul(params[8], &c, 0);
// 	if (*c != 0) {
// 		printf("Incorrect data8");
// 		return;
// 	}

// 	if (nb_params > 2) {
// 		strb = strtoul(params[9], &c, 0);
// 		if (*c != 0) {
// 			printf("Incorrect strb");
// 			return;
// 		}
// 	}
// 	hbm_writeread_fsm(data1, data2, data3, data4, data5, data6, data7, data8, address, strb);
// }
// define_command(hbm_writeread_fsm, hbm_writeread_hbm_test, "Write once to hbm", LITEDRAM_CMDS);
// #endif

/**
 * Command "hbm_gen_tester"
 * 
 * Run the generator once
 * 
*/
#if defined(CSR_HBM_GENERATOR_BASE)
static void hbm_gen_tester_handler(int nb_params, char **params)
{
	char *c;
	int burst_length;
	// Random turned to 0
	if (nb_params < 1) {
		printf("hbm_gen <burst_length>");
		return;
	} 
	burst_length = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect burst_length");
		return;
	}
	hbm_gen(burst_length);
}
define_command(hbm_gen, hbm_gen_tester_handler, "Single test for hbm writer", LITEDRAM_CMDS);
#endif


/**
 * Command "sdram_bist"
 *
 * Run SDRAM Build-In Self-Test
 *
 */
#if defined(CSR_SDRAM_GENERATOR_BASE) && defined(CSR_SDRAM_CHECKER_BASE)
static void sdram_bist_handler(int nb_params, char **params)
{
	char *c;
	int burst_length;
	int random;
	if (nb_params < 2) {
		printf("sdram_bist <burst_length> <random>");
		return;
	}
	burst_length = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect burst_length");
		return;
	}
	random = strtoul(params[1], &c, 0);
	if (*c != 0) {
		printf("Incorrect random");
		return;
	}
	sdram_bist(burst_length, random);
}
define_command(sdram_bist, sdram_bist_handler, "Run SDRAM Build-In Self-Test", LITEDRAM_CMDS);
#endif

#ifdef CSR_DDRPHY_RDPHASE_ADDR
/**
 * Command "sdram_force_rdphase"
 *
 * Force read phase
 *
 */
static void sdram_force_rdphase_handler(int nb_params, char **params)
{
	char *c;
	int phase;
	if (nb_params < 1) {
		printf("sdram_force_rdphase <phase>");
		return;
	}
	phase = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect phase");
		return;
	}
	printf("Forcing read phase to %d\n", phase);
	ddrphy_rdphase_write(phase);
}
define_command(sdram_force_rdphase, sdram_force_rdphase_handler, "Force read phase", LITEDRAM_CMDS);
#endif

#ifdef CSR_DDRPHY_WRPHASE_ADDR
/**
 * Command "sdram_force_wrphase"
 *
 * Force write phase
 *
 */
static void sdram_force_wrphase_handler(int nb_params, char **params)
{
	char *c;
	int phase;
	if (nb_params < 1) {
		printf("sdram_force_wrphase <phase>");
		return;
	}
	phase = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect phase");
		return;
	}
	printf("Forcing write phase to %d\n", phase);
	ddrphy_wrphase_write(phase);
}
define_command(sdram_force_wrphase, sdram_force_wrphase_handler, "Force write phase", LITEDRAM_CMDS);
#endif

#ifdef CSR_DDRPHY_CDLY_RST_ADDR

/**
 * Command "sdram_rst_cmd_delay"
 *
 * Reset write leveling Cmd delay
 *
 */
#if defined(CSR_SDRAM_BASE) && defined(CSR_DDRPHY_BASE)
static void sdram_rst_cmd_delay_handler(int nb_params, char **params)
{
	sdram_software_control_on();
	sdram_write_leveling_rst_cmd_delay(1);
	sdram_software_control_off();
}
define_command(sdram_rst_cmd_delay, sdram_rst_cmd_delay_handler, "Reset write leveling Cmd delay", LITEDRAM_CMDS);
#endif

/**
 * Command "sdram_force_cmd_delay"
 *
 * Force write leveling Cmd delay
 *
 */
#if defined(CSR_SDRAM_BASE) && defined(CSR_DDRPHY_BASE)
static void sdram_force_cmd_delay_handler(int nb_params, char **params)
{
	char *c;
	int taps;
	if (nb_params < 1) {
		printf("sdram_force_cmd_delay <taps>");
		return;
	}
	taps = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect taps");
		return;
	}
	sdram_software_control_on();
	sdram_write_leveling_force_cmd_delay(taps, 1);
	sdram_software_control_off();
}
define_command(sdram_force_cmd_delay, sdram_force_cmd_delay_handler, "Force write leveling Cmd delay", LITEDRAM_CMDS);
#endif

#endif

#ifdef CSR_DDRPHY_WDLY_DQ_RST_ADDR

/**
 * Command "sdram_rst_dat_delay"
 *
 * Reset write leveling Dat delay
 *
 */
#if defined(CSR_SDRAM_BASE) && defined(CSR_DDRPHY_BASE)
static void sdram_rst_dat_delay_handler(int nb_params, char **params)
{
	char *c;
	int module;
	if (nb_params < 1) {
		printf("sdram_rst_dat_delay <module>");
		return;
	}
	module = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect module");
		return;
	}
	sdram_software_control_on();
	sdram_write_leveling_rst_dat_delay(module, 1);
	sdram_software_control_off();
}
define_command(sdram_rst_dat_delay, sdram_rst_dat_delay_handler, "Reset write leveling Dat delay", LITEDRAM_CMDS);
#endif

/**
 * Command "sdram_force_dat_delay"
 *
 * Force write leveling Dat delay
 *
 */
#if defined(CSR_SDRAM_BASE) && defined(CSR_DDRPHY_BASE)
static void sdram_force_dat_delay_handler(int nb_params, char **params)
{
	char *c;
	int module;
	int taps;
	if (nb_params < 2) {
		printf("sdram_force_dat_delay <module> <taps>");
		return;
	}
	module = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect module");
		return;
	}
	taps = strtoul(params[1], &c, 0);
	if (*c != 0) {
		printf("Incorrect taps");
		return;
	}
	sdram_software_control_on();
	sdram_write_leveling_force_dat_delay(module, taps, 1);
	sdram_software_control_off();
}
define_command(sdram_force_dat_delay, sdram_force_dat_delay_handler, "Force write leveling Dat delay", LITEDRAM_CMDS);
#endif

/**
 * Command "sdram_rst_bitslip"
 *
 * Reset write leveling Bitslip
 *
 */
#if defined(CSR_SDRAM_BASE) && defined(CSR_DDRPHY_BASE)
static void sdram_rst_bitslip_handler(int nb_params, char **params)
{
	char *c;
	int module;
	if (nb_params < 1) {
		printf("sdram_rst_bitslip <module>");
		return;
	}
	module = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect module");
		return;
	}
	sdram_software_control_on();
	sdram_write_leveling_rst_bitslip(module, 1);
	sdram_software_control_off();
}
define_command(sdram_rst_bitslip, sdram_rst_bitslip_handler, "Reset write leveling Bitslip", LITEDRAM_CMDS);
#endif

/**
 * Command "sdram_force_bitslip"
 *
 * Force write leveling Bitslip
 *
 */
#if defined(CSR_SDRAM_BASE) && defined(CSR_DDRPHY_BASE)
static void sdram_force_bitslip_handler(int nb_params, char **params)
{
	char *c;
	int module;
	int bitslip;
	if (nb_params < 2) {
		printf("sdram_force_bitslip <module> <bitslip>");
		return;
	}
	module = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect module");
		return;
	}
	bitslip = strtoul(params[1], &c, 0);
	if (*c != 0) {
		printf("Incorrect bitslip");
		return;
	}
	sdram_software_control_on();
	sdram_write_leveling_force_bitslip(module, bitslip, 1);
	sdram_software_control_off();
}
define_command(sdram_force_bitslip, sdram_force_bitslip_handler, "Force write leveling Bitslip", LITEDRAM_CMDS);
#endif

#endif

/**
 * Command "sdram_mr_write"
 *
 * Write SDRAM Mode Register
 *
 */
#if defined(CSR_SDRAM_BASE)
static void sdram_mr_write_handler(int nb_params, char **params)
{
	char *c;
	uint8_t reg;
	uint16_t value;

	if (nb_params < 2) {
		printf("sdram_mr_write <reg> <value>");
		return;
	}
	reg = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect reg");
		return;
	}
	value = strtoul(params[1], &c, 0);
	if (*c != 0) {
		printf("Incorrect value");
		return;
	}
	sdram_software_control_on();
	printf("Writing 0x%04x to MR%d\n", value, reg);
	sdram_mode_register_write(reg, value);
	sdram_software_control_off();
}
define_command(sdram_mr_write, sdram_mr_write_handler, "Write SDRAM Mode Register", LITEDRAM_CMDS);
#endif

/**
 * Command "sdram_spd"
 *
 * Read contents of SPD EEPROM memory.
 * SPD address is a 3-bit address defined by the pins A0, A1, A2.
 *
 */
#ifdef CONFIG_HAS_I2C
static void sdram_spd_handler(int nb_params, char **params)
{
	char *c;
	unsigned char spdaddr;
	unsigned char buf[SDRAM_SPD_SIZE];
	int len = sizeof(buf);
	bool send_stop = true;

	if (nb_params < 1) {
		printf("sdram_spd <spdaddr> [<send_stop>]");
		return;
	}

	spdaddr = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect address");
		return;
	}
	if (spdaddr > 0b111) {
		printf("SPD EEPROM max address is 0b111 (defined by A0, A1, A2 pins)");
		return;
	}

	if (nb_params > 1) {
		send_stop = strtoul(params[1], &c, 0) != 0;
		if (*c != 0) {
			printf("Incorrect send_stop value");
			return;
		}
	}

	if (!sdram_read_spd(spdaddr, 0, buf, (uint16_t)len, send_stop)) {
		printf("Error when reading SPD EEPROM");
		return;
	}

	dump_bytes((unsigned int *) buf, len, 0);

#ifdef SPD_BASE
	{
		int cmp_result;
		cmp_result = memcmp(buf, (void *) SPD_BASE, SPD_SIZE);
		if (cmp_result == 0) {
			printf("Memory contents matches the data used for gateware generation\n");
		} else {
			printf("\nWARNING: memory differs from the data used during gateware generation:\n");
			dump_bytes((void *) SPD_BASE, SPD_SIZE, 0);
		}
	}
#endif
}
define_command(sdram_spd, sdram_spd_handler, "Read SDRAM SPD EEPROM", LITEDRAM_CMDS);
#endif

#ifdef SDRAM_DEBUG
define_command(sdram_debug, sdram_debug, "Run SDRAM debug tests", LITEDRAM_CMDS);
#endif
