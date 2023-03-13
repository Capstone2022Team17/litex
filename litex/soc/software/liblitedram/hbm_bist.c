
#include <generated/csr.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <uart.h>
#include <time.h>
#include <console.h>

#include <liblitedram/hbm_bist.h>

#define TOTAL_PORTS 32

__attribute__((unused)) static void cdelay(int i)
{
#ifndef CONFIG_BIOS_NO_DELAYS
	while(i > 0) {
		__asm__ volatile(CONFIG_CPU_NOP);
		i--;
	}
#endif
}


#if defined(CSR_HBM_GENERATOR_BASE)

#define HBM_TEST_BASE 0x00000000
#define SDRAM_TEST_DATA_BYTES 0

void hbm_gen(uint32_t burst_length) {
    printf("Running 1, sdram_test_data_bytes: %d\n", SDRAM_TEST_DATA_BYTES);
    hbm_generator_reset_write(1);
    printf("Running 2\n");
    hbm_generator_reset_write(0);
    printf("Running 3\n");
    hbm_generator_random_write(1);
    printf("Running 4\n");
    hbm_generator_base_write(HBM_TEST_BASE);
    printf("Running 5\n");
    hbm_generator_end_write(HBM_TEST_BASE + burst_length);
    printf("Running 6\n");
    hbm_generator_length_write(burst_length);
    printf("Running 7\n");
    cdelay(100);
    hbm_generator_start_write(1);
    printf("Running 8, hbm generator done: %d\n", hbm_generator_done_read());
    while (hbm_generator_done_read() == 0);
    printf("Done\n");
    
}

#endif

// void whatstateamI() {
//     if (hbm_4_waitinstruction_fsm_read()) {printf("FSM: Wait instruction\n"); }
//     if (hbm_4_prepwritecommand_fsm_read()) {printf("FSM: Stage 1 write\n"); }
//     // if (hbm_4_prepwrite_fsm_read()) {printf("FSM: Stage 2 write\n"); }
//     if (hbm_4_prepwriteresponse_fsm_read()) {printf("FSM: Stage 3 write\n"); }
//     if (hbm_4_donewrite_fsm_read()) {printf("FSM: Done with write\n"); }
//     if (hbm_4_prepreadcommand_fsm_read()) {printf("FSM: Stage 1 read\n"); }
//     if (hbm_4_prepread_fsm_read()) {printf("FSM: Stage 2 read\n"); }
//     if (hbm_4_doneread_fsm_read()) {printf("FSM: Done with read\n"); }
// }

void hbm_write_fsm(uint32_t data1, uint32_t data2, uint32_t data3, uint32_t data4, uint32_t data5, uint32_t data6, uint32_t data7, uint32_t data8, uint32_t address, uint32_t strb) {
    hbm_4_acknowledge_readwrite_write(0);
    hbm_4_address_readwrite_write(address);
    // hbm_4_data_writein1_write(data1);
    // hbm_4_data_writein2_write(data2);
    // hbm_4_data_writein3_write(data3);
    // hbm_4_data_writein4_write(data4);
    // hbm_4_data_writein5_write(data5);
    // hbm_4_data_writein6_write(data6);
    // hbm_4_data_writein7_write(data7);
    // hbm_4_data_writein8_write(data8);
    //hbm_4_strb_readwrite_write(strb);
    cdelay(100);
    //printf("Set up variables, csrs: data: %08x, %08x, %08x, %08x, %08x, %08x, %08x, %08x, address: %08x, strb: %08x\n", hbm_4_data_writein8_read(), hbm_4_data_writein7_read(), hbm_4_data_writein6_read(), hbm_4_data_writein5_read(), hbm_4_data_writein4_read(), hbm_4_data_writein3_read(), hbm_4_data_writein2_read(), hbm_4_data_writein1_read(),hbm_4_address_readwrite_read(), hbm_4_strb_readwrite_read());
    printf("Started perform write, waiting for response\n");
    hbm_4_perform_write_write(1);
    cdelay(100);
    printf("Value of exec_write_done = %x\n", hbm_4_exec_write_done_read());
    printf("Waiting...\n");
    while (hbm_4_exec_write_done_read() == 0) {
        cdelay(500);
    }
    printf("Done\n");
    printf("Write response: %x\n", hbm_4_write_resp_read());
    hbm_4_perform_write_write(0);
    hbm_4_acknowledge_readwrite_write(1);
}

void hbm_read_fsm(uint32_t address) {
    hbm_4_acknowledge_readwrite_write(0);
    hbm_4_address_readwrite_write(address);
    cdelay(100);
    //printf("Set up variables, csrs: address: %08x\n", hbm_4_address_readwrite_read());
    printf("Started perform write, waiting for response\n");
    hbm_4_perform_read_write(1);
    cdelay(100);
    printf("Value of exec_read_done = %x\n", hbm_4_exec_read_done_read());
    printf("Waiting...\n");
    while (hbm_4_exec_read_done_read() == 0) {
        cdelay(500);
    }
    printf("Done\n");
    printf("Data: %08x %08x %08x %08x %08x %08x %08x %08x\n", 
    hbm_4_data_readout8_read(),
    hbm_4_data_readout7_read(),
    hbm_4_data_readout6_read(),
    hbm_4_data_readout5_read(),
    hbm_4_data_readout4_read(),
    hbm_4_data_readout3_read(),
    hbm_4_data_readout2_read(),
    hbm_4_data_readout1_read());
    printf("Read response: %x\n", hbm_4_read_resp_read());
    hbm_4_perform_read_write(0);
    hbm_4_acknowledge_readwrite_write(1);
}

// create your function prototype, which all functions must use
typedef void(*VoidFuncInt)(uint32_t);

// create your function prototype, which all functions must use
typedef uint32_t(*IntFuncVoid)(void);

// create the array of pointers to actual functions
VoidFuncInt acknowledge_readwrite[] = {
                        hbm_0_acknowledge_readwrite_write,
                        hbm_1_acknowledge_readwrite_write,
                        hbm_2_acknowledge_readwrite_write,
                        hbm_3_acknowledge_readwrite_write,

                        hbm_4_acknowledge_readwrite_write, 
                        hbm_5_acknowledge_readwrite_write, 
                        hbm_6_acknowledge_readwrite_write,
                        hbm_7_acknowledge_readwrite_write,
                        hbm_8_acknowledge_readwrite_write,
                        hbm_9_acknowledge_readwrite_write,
                        hbm_10_acknowledge_readwrite_write,
                        hbm_11_acknowledge_readwrite_write,
                        hbm_12_acknowledge_readwrite_write,
                        hbm_13_acknowledge_readwrite_write,
                        hbm_14_acknowledge_readwrite_write,
                        hbm_15_acknowledge_readwrite_write,
                        hbm_16_acknowledge_readwrite_write,
                        hbm_17_acknowledge_readwrite_write,
                        hbm_18_acknowledge_readwrite_write,
                        hbm_19_acknowledge_readwrite_write,
                        hbm_20_acknowledge_readwrite_write,
                        hbm_21_acknowledge_readwrite_write,
                        hbm_22_acknowledge_readwrite_write,
                        hbm_23_acknowledge_readwrite_write,
                        hbm_24_acknowledge_readwrite_write,
                        hbm_25_acknowledge_readwrite_write,
                        hbm_26_acknowledge_readwrite_write,
                        hbm_27_acknowledge_readwrite_write,
                        hbm_28_acknowledge_readwrite_write,
                        hbm_29_acknowledge_readwrite_write,
                        hbm_30_acknowledge_readwrite_write,
                        hbm_31_acknowledge_readwrite_write};

VoidFuncInt burst_length_write[] = {
                        hbm_0_burst_len_write,
                        hbm_1_burst_len_write,
                        hbm_2_burst_len_write,
                        hbm_3_burst_len_write,
                        hbm_4_burst_len_write,
                        hbm_5_burst_len_write,
                        hbm_6_burst_len_write,
                        hbm_7_burst_len_write,
                        hbm_8_burst_len_write,
                        hbm_9_burst_len_write,
                        hbm_10_burst_len_write,
                        hbm_11_burst_len_write,
                        hbm_12_burst_len_write,
                        hbm_13_burst_len_write,
                        hbm_14_burst_len_write,
                        hbm_15_burst_len_write,
                        hbm_16_burst_len_write,
                        hbm_17_burst_len_write,
                        hbm_18_burst_len_write,
                        hbm_19_burst_len_write,
                        hbm_20_burst_len_write,
                        hbm_21_burst_len_write,
                        hbm_22_burst_len_write,
                        hbm_23_burst_len_write,
                        hbm_24_burst_len_write,
                        hbm_25_burst_len_write,
                        hbm_26_burst_len_write,
                        hbm_27_burst_len_write,
                        hbm_28_burst_len_write,
                        hbm_29_burst_len_write,
                        hbm_30_burst_len_write,
                        hbm_31_burst_len_write};

VoidFuncInt address_readwrite[] = {
                        hbm_0_address_readwrite_write, 
                        hbm_1_address_readwrite_write, 
                        hbm_2_address_readwrite_write, 
                        hbm_3_address_readwrite_write, 

                        hbm_4_address_readwrite_write, 
                        hbm_5_address_readwrite_write, 
                        hbm_6_address_readwrite_write,
                        hbm_7_address_readwrite_write,
                        hbm_8_address_readwrite_write,
                        hbm_9_address_readwrite_write,
                        hbm_10_address_readwrite_write,
                        hbm_11_address_readwrite_write,
                        hbm_12_address_readwrite_write,
                        hbm_13_address_readwrite_write,
                        hbm_14_address_readwrite_write,
                        hbm_15_address_readwrite_write,
                        hbm_16_address_readwrite_write,
                        hbm_17_address_readwrite_write,
                        hbm_18_address_readwrite_write,
                        hbm_19_address_readwrite_write,
                        hbm_20_address_readwrite_write,
                        hbm_21_address_readwrite_write,
                        hbm_22_address_readwrite_write,
                        hbm_23_address_readwrite_write,
                        hbm_24_address_readwrite_write,
                        hbm_25_address_readwrite_write,
                        hbm_26_address_readwrite_write,
                        hbm_27_address_readwrite_write,
                        hbm_28_address_readwrite_write,
                        hbm_29_address_readwrite_write,
                        hbm_30_address_readwrite_write,
                        hbm_31_address_readwrite_write};

VoidFuncInt perform_write[] = {
                        hbm_0_perform_write_write, 
                        hbm_1_perform_write_write, 
                        hbm_2_perform_write_write, 
                        hbm_3_perform_write_write, 

                        hbm_4_perform_write_write, 
                        hbm_5_perform_write_write, 
                        hbm_6_perform_write_write,
                        hbm_7_perform_write_write,
                        hbm_8_perform_write_write,
                        hbm_9_perform_write_write,
                        hbm_10_perform_write_write,
                        hbm_11_perform_write_write,
                        hbm_12_perform_write_write,
                        hbm_13_perform_write_write,
                        hbm_14_perform_write_write,
                        hbm_15_perform_write_write,
                        hbm_16_perform_write_write,
                        hbm_17_perform_write_write,
                        hbm_18_perform_write_write,
                        hbm_19_perform_write_write,
                        hbm_20_perform_write_write,
                        hbm_21_perform_write_write,
                        hbm_22_perform_write_write,
                        hbm_23_perform_write_write,
                        hbm_24_perform_write_write,
                        hbm_25_perform_write_write,
                        hbm_26_perform_write_write,
                        hbm_27_perform_write_write,
                        hbm_28_perform_write_write,
                        hbm_29_perform_write_write,
                        hbm_30_perform_write_write,
                        hbm_31_perform_write_write};

VoidFuncInt perform_read[] = {
                        hbm_0_perform_read_write,
                        hbm_1_perform_read_write,
                        hbm_2_perform_read_write,
                        hbm_3_perform_read_write,

                        hbm_4_perform_read_write, 
                        hbm_5_perform_read_write, 
                        hbm_6_perform_read_write,
                        hbm_7_perform_read_write,
                        hbm_8_perform_read_write,
                        hbm_9_perform_read_write,
                        hbm_10_perform_read_write,
                        hbm_11_perform_read_write,
                        hbm_12_perform_read_write,
                        hbm_13_perform_read_write,
                        hbm_14_perform_read_write,
                        hbm_15_perform_read_write,
                        hbm_16_perform_read_write,
                        hbm_17_perform_read_write,
                        hbm_18_perform_read_write,
                        hbm_19_perform_read_write,
                        hbm_20_perform_read_write,
                        hbm_21_perform_read_write,
                        hbm_22_perform_read_write,
                        hbm_23_perform_read_write,
                        hbm_24_perform_read_write,
                        hbm_25_perform_read_write,
                        hbm_26_perform_read_write,
                        hbm_27_perform_read_write,
                        hbm_28_perform_read_write,
                        hbm_29_perform_read_write,
                        hbm_30_perform_read_write,
                        hbm_31_perform_read_write};

IntFuncVoid exec_write_done[] = {
                        hbm_0_exec_write_done_read,
                        hbm_1_exec_write_done_read,
                        hbm_2_exec_write_done_read,
                        hbm_3_exec_write_done_read,

                        hbm_4_exec_write_done_read,
                        hbm_5_exec_write_done_read, 
                        hbm_6_exec_write_done_read,
                        hbm_7_exec_write_done_read,
                        hbm_8_exec_write_done_read,
                        hbm_9_exec_write_done_read,
                        hbm_10_exec_write_done_read,
                        hbm_11_exec_write_done_read,
                        hbm_12_exec_write_done_read,
                        hbm_13_exec_write_done_read,
                        hbm_14_exec_write_done_read,
                        hbm_15_exec_write_done_read,
                        hbm_16_exec_write_done_read,
                        hbm_17_exec_write_done_read,
                        hbm_18_exec_write_done_read,
                        hbm_19_exec_write_done_read,
                        hbm_20_exec_write_done_read,
                        hbm_21_exec_write_done_read,
                        hbm_22_exec_write_done_read,
                        hbm_23_exec_write_done_read,
                        hbm_24_exec_write_done_read,
                        hbm_25_exec_write_done_read,
                        hbm_26_exec_write_done_read,
                        hbm_27_exec_write_done_read,
                        hbm_28_exec_write_done_read,
                        hbm_29_exec_write_done_read,
                        hbm_30_exec_write_done_read,
                        hbm_31_exec_write_done_read};

IntFuncVoid exec_read_done[] = {
                        hbm_0_exec_read_done_read,
                        hbm_1_exec_read_done_read,
                        hbm_2_exec_read_done_read,
                        hbm_3_exec_read_done_read,

                        hbm_4_exec_read_done_read,
                        hbm_5_exec_read_done_read, 
                        hbm_6_exec_read_done_read,
                        hbm_7_exec_read_done_read,
                        hbm_8_exec_read_done_read,
                        hbm_9_exec_read_done_read,
                        hbm_10_exec_read_done_read,
                        hbm_11_exec_read_done_read,
                        hbm_12_exec_read_done_read,
                        hbm_13_exec_read_done_read,
                        hbm_14_exec_read_done_read,
                        hbm_15_exec_read_done_read,
                        hbm_16_exec_read_done_read,
                        hbm_17_exec_read_done_read,
                        hbm_18_exec_read_done_read,
                        hbm_19_exec_read_done_read,
                        hbm_20_exec_read_done_read,
                        hbm_21_exec_read_done_read,
                        hbm_22_exec_read_done_read,
                        hbm_23_exec_read_done_read,
                        hbm_24_exec_read_done_read,
                        hbm_25_exec_read_done_read,
                        hbm_26_exec_read_done_read,
                        hbm_27_exec_read_done_read,
                        hbm_28_exec_read_done_read,
                        hbm_29_exec_read_done_read,
                        hbm_30_exec_read_done_read,
                        hbm_31_exec_read_done_read};

IntFuncVoid data_read[] = {
                        hbm_0_data_readout8_read, hbm_0_data_readout7_read, hbm_0_data_readout6_read, hbm_0_data_readout5_read, hbm_0_data_readout4_read, hbm_0_data_readout3_read, hbm_0_data_readout2_read, hbm_0_data_readout1_read,
                        hbm_1_data_readout8_read, hbm_1_data_readout7_read, hbm_1_data_readout6_read, hbm_1_data_readout5_read, hbm_1_data_readout4_read, hbm_1_data_readout3_read, hbm_1_data_readout2_read, hbm_1_data_readout1_read,
                        hbm_2_data_readout8_read, hbm_2_data_readout7_read, hbm_2_data_readout6_read, hbm_2_data_readout5_read, hbm_2_data_readout4_read, hbm_2_data_readout3_read, hbm_2_data_readout2_read, hbm_2_data_readout1_read,
                        hbm_3_data_readout8_read, hbm_3_data_readout7_read, hbm_3_data_readout6_read, hbm_3_data_readout5_read, hbm_3_data_readout4_read, hbm_3_data_readout3_read, hbm_3_data_readout2_read, hbm_3_data_readout1_read,

                        hbm_4_data_readout8_read, hbm_4_data_readout7_read, hbm_4_data_readout6_read, hbm_4_data_readout5_read, hbm_4_data_readout4_read, hbm_4_data_readout3_read, hbm_4_data_readout2_read, hbm_4_data_readout1_read,
                        hbm_5_data_readout8_read, hbm_5_data_readout7_read, hbm_5_data_readout6_read, hbm_5_data_readout5_read, hbm_5_data_readout4_read, hbm_5_data_readout3_read, hbm_5_data_readout2_read, hbm_5_data_readout1_read,
                        hbm_6_data_readout8_read, hbm_6_data_readout7_read, hbm_6_data_readout6_read, hbm_6_data_readout5_read, hbm_6_data_readout4_read, hbm_6_data_readout3_read, hbm_6_data_readout2_read, hbm_6_data_readout1_read,
                        hbm_7_data_readout8_read, hbm_7_data_readout7_read, hbm_7_data_readout6_read, hbm_7_data_readout5_read, hbm_7_data_readout4_read, hbm_7_data_readout3_read, hbm_7_data_readout2_read, hbm_7_data_readout1_read,
                        hbm_8_data_readout8_read, hbm_8_data_readout7_read, hbm_8_data_readout6_read, hbm_8_data_readout5_read, hbm_8_data_readout4_read, hbm_8_data_readout3_read, hbm_8_data_readout2_read, hbm_8_data_readout1_read,
                        hbm_9_data_readout8_read, hbm_9_data_readout7_read, hbm_9_data_readout6_read, hbm_9_data_readout5_read, hbm_9_data_readout4_read, hbm_9_data_readout3_read, hbm_9_data_readout2_read, hbm_9_data_readout1_read,
                        hbm_10_data_readout8_read, hbm_10_data_readout7_read, hbm_10_data_readout6_read, hbm_10_data_readout5_read, hbm_10_data_readout4_read, hbm_10_data_readout3_read, hbm_10_data_readout2_read, hbm_10_data_readout1_read,
                        hbm_11_data_readout8_read, hbm_11_data_readout7_read, hbm_11_data_readout6_read, hbm_11_data_readout5_read, hbm_11_data_readout4_read, hbm_11_data_readout3_read, hbm_11_data_readout2_read, hbm_11_data_readout1_read,
                        hbm_12_data_readout8_read, hbm_12_data_readout7_read, hbm_12_data_readout6_read, hbm_12_data_readout5_read, hbm_12_data_readout4_read, hbm_12_data_readout3_read, hbm_12_data_readout2_read, hbm_12_data_readout1_read,
                        hbm_13_data_readout8_read, hbm_13_data_readout7_read, hbm_13_data_readout6_read, hbm_13_data_readout5_read, hbm_13_data_readout4_read, hbm_13_data_readout3_read, hbm_13_data_readout2_read, hbm_13_data_readout1_read,
                        hbm_14_data_readout8_read, hbm_14_data_readout7_read, hbm_14_data_readout6_read, hbm_14_data_readout5_read, hbm_14_data_readout4_read, hbm_14_data_readout3_read, hbm_14_data_readout2_read, hbm_14_data_readout1_read,
                        hbm_15_data_readout8_read, hbm_15_data_readout7_read, hbm_15_data_readout6_read, hbm_15_data_readout5_read, hbm_15_data_readout4_read, hbm_15_data_readout3_read, hbm_15_data_readout2_read, hbm_15_data_readout1_read,
                        hbm_16_data_readout8_read, hbm_16_data_readout7_read, hbm_16_data_readout6_read, hbm_16_data_readout5_read, hbm_16_data_readout4_read, hbm_16_data_readout3_read, hbm_16_data_readout2_read, hbm_16_data_readout1_read,
                        hbm_17_data_readout8_read, hbm_17_data_readout7_read, hbm_17_data_readout6_read, hbm_17_data_readout5_read, hbm_17_data_readout4_read, hbm_17_data_readout3_read, hbm_17_data_readout2_read, hbm_17_data_readout1_read,
                        hbm_18_data_readout8_read, hbm_18_data_readout7_read, hbm_18_data_readout6_read, hbm_18_data_readout5_read, hbm_18_data_readout4_read, hbm_18_data_readout3_read, hbm_18_data_readout2_read, hbm_18_data_readout1_read,
                        hbm_19_data_readout8_read, hbm_19_data_readout7_read, hbm_19_data_readout6_read, hbm_19_data_readout5_read, hbm_19_data_readout4_read, hbm_19_data_readout3_read, hbm_19_data_readout2_read, hbm_19_data_readout1_read,
                        hbm_20_data_readout8_read, hbm_20_data_readout7_read, hbm_20_data_readout6_read, hbm_20_data_readout5_read, hbm_20_data_readout4_read, hbm_20_data_readout3_read, hbm_20_data_readout2_read, hbm_20_data_readout1_read,
                        hbm_21_data_readout8_read, hbm_21_data_readout7_read, hbm_21_data_readout6_read, hbm_21_data_readout5_read, hbm_21_data_readout4_read, hbm_21_data_readout3_read, hbm_21_data_readout2_read, hbm_21_data_readout1_read,
                        hbm_22_data_readout8_read, hbm_22_data_readout7_read, hbm_22_data_readout6_read, hbm_22_data_readout5_read, hbm_22_data_readout4_read, hbm_22_data_readout3_read, hbm_22_data_readout2_read, hbm_22_data_readout1_read,
                        hbm_23_data_readout8_read, hbm_23_data_readout7_read, hbm_23_data_readout6_read, hbm_23_data_readout5_read, hbm_23_data_readout4_read, hbm_23_data_readout3_read, hbm_23_data_readout2_read, hbm_23_data_readout1_read,
                        hbm_24_data_readout8_read, hbm_24_data_readout7_read, hbm_24_data_readout6_read, hbm_24_data_readout5_read, hbm_24_data_readout4_read, hbm_24_data_readout3_read, hbm_24_data_readout2_read, hbm_24_data_readout1_read,
                        hbm_25_data_readout8_read, hbm_25_data_readout7_read, hbm_25_data_readout6_read, hbm_25_data_readout5_read, hbm_25_data_readout4_read, hbm_25_data_readout3_read, hbm_25_data_readout2_read, hbm_25_data_readout1_read,
                        hbm_26_data_readout8_read, hbm_26_data_readout7_read, hbm_26_data_readout6_read, hbm_26_data_readout5_read, hbm_26_data_readout4_read, hbm_26_data_readout3_read, hbm_26_data_readout2_read, hbm_26_data_readout1_read,
                        hbm_27_data_readout8_read, hbm_27_data_readout7_read, hbm_27_data_readout6_read, hbm_27_data_readout5_read, hbm_27_data_readout4_read, hbm_27_data_readout3_read, hbm_27_data_readout2_read, hbm_27_data_readout1_read,
                        hbm_28_data_readout8_read, hbm_28_data_readout7_read, hbm_28_data_readout6_read, hbm_28_data_readout5_read, hbm_28_data_readout4_read, hbm_28_data_readout3_read, hbm_28_data_readout2_read, hbm_28_data_readout1_read,
                        hbm_29_data_readout8_read, hbm_29_data_readout7_read, hbm_29_data_readout6_read, hbm_29_data_readout5_read, hbm_29_data_readout4_read, hbm_29_data_readout3_read, hbm_29_data_readout2_read, hbm_29_data_readout1_read,
                        hbm_30_data_readout8_read, hbm_30_data_readout7_read, hbm_30_data_readout6_read, hbm_30_data_readout5_read, hbm_30_data_readout4_read, hbm_30_data_readout3_read, hbm_30_data_readout2_read, hbm_30_data_readout1_read,
                        hbm_31_data_readout8_read, hbm_31_data_readout7_read, hbm_31_data_readout6_read, hbm_31_data_readout5_read, hbm_31_data_readout4_read, hbm_31_data_readout3_read, hbm_31_data_readout2_read, hbm_31_data_readout1_read};

// Run all state machines for all hbm ports once to write and read.
void hbm_test(uint32_t address, uint32_t addr_offset, uint32_t burst_length) {

    // Perform Write
    for (int i = 0; i < sizeof(acknowledge_readwrite) / sizeof(*acknowledge_readwrite); i++)
    {
        acknowledge_readwrite[i](0);
    }
    printf("Ack done\n");

    for (int i = 0; i < sizeof(burst_length_write) / sizeof(*burst_length_write); i++)
    {
        burst_length_write[i](burst_length);
    }
    printf("burst done\n");

    for (int i = 0; i < sizeof(address_readwrite) / sizeof(*address_readwrite); i++)
    {
        address_readwrite[i](address);
        address += addr_offset;
    }

    printf("Address done\n");

    printf("Started perform write, waiting for response\n");
    for (int i = 0; i < sizeof(perform_write) / sizeof(*perform_write); i++)
    {
        perform_write[i](1);
    }

    printf("perf done\n");

    int func_running_flag = 0;
    while (func_running_flag < TOTAL_PORTS) {
        func_running_flag = 0;
        for (int i = 0; i < sizeof(exec_write_done) / sizeof(*exec_write_done); i++)
        {
            func_running_flag += exec_write_done[i]();
        }
    }

    printf("Done\n");

    // Restore state machine back to wait state
    for (int i = 0; i < sizeof(perform_write) / sizeof(*perform_write); i++)
    {
        perform_write[i](0);
    }

    printf("Perf2 done\n");

    for (int i = 0; i < sizeof(acknowledge_readwrite) / sizeof(*acknowledge_readwrite); i++)
    {
        acknowledge_readwrite[i](1);
    }

    printf("ack done\n");

    // Delay 500 ms
    cdelay(500);

    // Perform read
    for (int i = 0; i < sizeof(acknowledge_readwrite) / sizeof(*acknowledge_readwrite); i++)
    {
        acknowledge_readwrite[i](0);
    }

    printf("ack3 done\n");

    printf("Started perform read, waiting for response\n");
    for (int i = 0; i < sizeof(perform_read) / sizeof(*perform_read); i++)
    {
        perform_read[i](1);
    }

    printf("perform read done\n");

    func_running_flag = 0;
    while (func_running_flag < TOTAL_PORTS) {
        func_running_flag = 0;
        for (int i = 0; i < sizeof(exec_read_done) / sizeof(*exec_read_done); i++)
        {
            func_running_flag += exec_read_done[i]();
        }
    }

    printf("Done\n");

    int index = 0;
    for (int i = 0; i < sizeof(data_read) / sizeof(*data_read); i += 8)
    {
        printf("Data for hbm%d: %08x %08x %08x %08x %08x %08x %08x %08x\n",
        index, 
        data_read[i](),
        data_read[i + 1](),
        data_read[i + 2](),
        data_read[i + 3](),
        data_read[i + 4](),
        data_read[i + 5](),
        data_read[i + 6](),
        data_read[i + 7]());
        index++;
    }
    printf("Data read done\n");

    // Restore state machine back to wait state
    for (int i = 0; i < sizeof(perform_read) / sizeof(*perform_read); i++)
    {
        perform_read[i](0);
    }
    for (int i = 0; i < sizeof(acknowledge_readwrite) / sizeof(*acknowledge_readwrite); i++)
    {
        acknowledge_readwrite[i](1);
    }

    printf("\nEnd of program\n");
    
}