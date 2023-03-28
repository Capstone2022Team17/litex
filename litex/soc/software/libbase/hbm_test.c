
#include <generated/csr.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <uart.h>
#include <time.h>
#include <console.h>

#include <libbase/hbm_test.h>

#define TOTAL_PORTS 32

// For settings:
# define OPTION_WRITE 1
# define OPTION_READ 0
# define BEATS_PER_BURST 16
# define BYTES_PER_BEAT 32



__attribute__((unused)) static void cdelay(int i)
{
#ifndef CONFIG_BIOS_NO_DELAYS
	while(i > 0) {
		__asm__ volatile(CONFIG_CPU_NOP);
		i--;
	}
#endif
}

void set_burst_enable_csrs(uint32_t num_bytes, uint32_t ports_mask, uint32_t address, uint32_t addr_offset);


// #if defined(CSR_HBM_GENERATOR_BASE)

// #define HBM_TEST_BASE 0x00000000
// #define SDRAM_TEST_DATA_BYTES 0

// void hbm_gen(uint32_t burst_length) {
//     printf("Running 1, sdram_test_data_bytes: %d\n", SDRAM_TEST_DATA_BYTES);
//     hbm_generator_reset_write(1);
//     printf("Running 2\n");
//     hbm_generator_reset_write(0);
//     printf("Running 3\n");
//     hbm_generator_random_write(1);
//     printf("Running 4\n");
//     hbm_generator_base_write(HBM_TEST_BASE);
//     printf("Running 5\n");
//     hbm_generator_end_write(HBM_TEST_BASE + burst_length);
//     printf("Running 6\n");
//     hbm_generator_length_write(burst_length);
//     printf("Running 7\n");
//     cdelay(100);
//     hbm_generator_start_write(1);
//     printf("Running 8, hbm generator done: %d\n", hbm_generator_done_read());
//     while (hbm_generator_done_read() == 0);
//     printf("Done\n");
    
// }

// #endif

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

// void hbm_write_fsm(uint32_t data1, uint32_t data2, uint32_t data3, uint32_t data4, uint32_t data5, uint32_t data6, uint32_t data7, uint32_t data8, uint32_t address, uint32_t strb) {
//     hbm_4_acknowledge_readwrite_write(0);
//     hbm_4_address_readwrite_write(address);
//     // hbm_4_data_writein1_write(data1);
//     // hbm_4_data_writein2_write(data2);
//     // hbm_4_data_writein3_write(data3);
//     // hbm_4_data_writein4_write(data4);
//     // hbm_4_data_writein5_write(data5);
//     // hbm_4_data_writein6_write(data6);
//     // hbm_4_data_writein7_write(data7);
//     // hbm_4_data_writein8_write(data8);
//     //hbm_4_strb_readwrite_write(strb);
//     cdelay(100);
//     //printf("Set up variables, csrs: data: %08x, %08x, %08x, %08x, %08x, %08x, %08x, %08x, address: %08x, strb: %08x\n", hbm_4_data_writein8_read(), hbm_4_data_writein7_read(), hbm_4_data_writein6_read(), hbm_4_data_writein5_read(), hbm_4_data_writein4_read(), hbm_4_data_writein3_read(), hbm_4_data_writein2_read(), hbm_4_data_writein1_read(),hbm_4_address_readwrite_read(), hbm_4_strb_readwrite_read());
//     printf("Started perform write, waiting for response\n");
//     hbm_4_perform_write_write(1);
//     cdelay(100);
//     printf("Value of exec_done = %x\n", hbm_4_exec_done_read());
//     printf("Waiting...\n");
//     while (hbm_4_exec_done_read() == 0) {
//         cdelay(500);
//     }
//     printf("Done\n");
//     printf("Write response: %x\n", hbm_4_write_resp_read());
//     hbm_4_perform_write_write(0);
//     hbm_4_acknowledge_readwrite_write(1);
// }

// void hbm_read_fsm(uint32_t address) {
//     commonRegs_ports_mask_write(5);
//     hbm_4_acknowledge_readwrite_write(0);
//     hbm_4_address_readwrite_write(address);
//     hbm_4_burst_len_write(0);
//     cdelay(100);
//     //printf("Set up variables, csrs: address: %08x\n", hbm_4_address_readwrite_read());
//     hbm_4_port_settings_write(OPTION_READ);

//     printf("Started perform read, waiting for response\n");
//     commonRegs_start_write(1);
    
//     cdelay(100);
//     printf("Value of exec_read_done = %x\n", hbm_4_exec_read_done_read());
//     printf("Waiting...\n");
//     while (hbm_4_exec_read_done_read() == 0) {
//         cdelay(500);
//     }
//     printf("Done\n");
//     printf("Data: %08x %08x %08x %08x %08x %08x %08x %08x\n", 
//     hbm_4_data_readout8_read(),
//     hbm_4_data_readout7_read(),
//     hbm_4_data_readout6_read(),
//     hbm_4_data_readout5_read(),
//     hbm_4_data_readout4_read(),
//     hbm_4_data_readout3_read(),
//     hbm_4_data_readout2_read(),
//     hbm_4_data_readout1_read());
//     //printf("Read response: %x\n", hbm_4_read_resp_read());
//     commonRegs_start_write(0);
//     hbm_4_acknowledge_readwrite_write(1);
// }



typedef void(*VoidFuncInt)(uint32_t);


typedef uint32_t(*IntFuncVoid)(void);

VoidFuncInt set_delay[] = {
                        hbm_0_delay_ctr_max_write,
                        hbm_1_delay_ctr_max_write,
                        hbm_2_delay_ctr_max_write,
                        hbm_3_delay_ctr_max_write,
                        hbm_4_delay_ctr_max_write,
                        hbm_5_delay_ctr_max_write,
                        hbm_6_delay_ctr_max_write,
                        hbm_7_delay_ctr_max_write,
                        hbm_8_delay_ctr_max_write,
                        hbm_9_delay_ctr_max_write,
                        hbm_10_delay_ctr_max_write,
                        hbm_11_delay_ctr_max_write,
                        hbm_12_delay_ctr_max_write,
                        hbm_13_delay_ctr_max_write,
                        hbm_14_delay_ctr_max_write,
                        hbm_15_delay_ctr_max_write,
                        hbm_16_delay_ctr_max_write,
                        hbm_17_delay_ctr_max_write,
                        hbm_18_delay_ctr_max_write,
                        hbm_19_delay_ctr_max_write,
                        hbm_20_delay_ctr_max_write,
                        hbm_21_delay_ctr_max_write,
                        hbm_22_delay_ctr_max_write,
                        hbm_23_delay_ctr_max_write,
                        hbm_24_delay_ctr_max_write,
                        hbm_25_delay_ctr_max_write,
                        hbm_26_delay_ctr_max_write,
                        hbm_27_delay_ctr_max_write,
                        hbm_28_delay_ctr_max_write,
                        hbm_29_delay_ctr_max_write,
                        hbm_30_delay_ctr_max_write,
                        hbm_31_delay_ctr_max_write,
};

IntFuncVoid delay_state_fsm[] = {
                        hbm_0_delay_state_fsm_read,
                        hbm_1_delay_state_fsm_read,
                        hbm_2_delay_state_fsm_read,
                        hbm_3_delay_state_fsm_read,
                        hbm_4_delay_state_fsm_read,
                        hbm_5_delay_state_fsm_read,
                        hbm_6_delay_state_fsm_read,
                        hbm_7_delay_state_fsm_read,
                        hbm_8_delay_state_fsm_read,
                        hbm_9_delay_state_fsm_read,
                        hbm_10_delay_state_fsm_read,
                        hbm_11_delay_state_fsm_read,
                        hbm_12_delay_state_fsm_read,
                        hbm_13_delay_state_fsm_read,
                        hbm_14_delay_state_fsm_read,
                        hbm_15_delay_state_fsm_read,
                        hbm_16_delay_state_fsm_read,
                        hbm_17_delay_state_fsm_read,
                        hbm_18_delay_state_fsm_read,
                        hbm_19_delay_state_fsm_read,
                        hbm_20_delay_state_fsm_read,
                        hbm_21_delay_state_fsm_read,
                        hbm_22_delay_state_fsm_read,
                        hbm_23_delay_state_fsm_read,
                        hbm_24_delay_state_fsm_read,
                        hbm_25_delay_state_fsm_read,
                        hbm_26_delay_state_fsm_read,
                        hbm_27_delay_state_fsm_read,
                        hbm_28_delay_state_fsm_read,
                        hbm_29_delay_state_fsm_read,
                        hbm_30_delay_state_fsm_read,
                        hbm_31_delay_state_fsm_read,
};

IntFuncVoid total_writes_len[] = {
                        hbm_0_total_writes_read,
                        hbm_1_total_writes_read,
                        hbm_2_total_writes_read,
                        hbm_3_total_writes_read,
                        hbm_4_total_writes_read,
                        hbm_5_total_writes_read,
                        hbm_6_total_writes_read,
                        hbm_7_total_writes_read,
                        hbm_8_total_writes_read,
                        hbm_9_total_writes_read,
                        hbm_10_total_writes_read,
                        hbm_11_total_writes_read,
                        hbm_12_total_writes_read,
                        hbm_13_total_writes_read,
                        hbm_14_total_writes_read,
                        hbm_15_total_writes_read,
                        hbm_16_total_writes_read,
                        hbm_17_total_writes_read,
                        hbm_18_total_writes_read,
                        hbm_19_total_writes_read,
                        hbm_20_total_writes_read,
                        hbm_21_total_writes_read,
                        hbm_22_total_writes_read,
                        hbm_23_total_writes_read,
                        hbm_24_total_writes_read,
                        hbm_25_total_writes_read,
                        hbm_26_total_writes_read,
                        hbm_27_total_writes_read,
                        hbm_28_total_writes_read,
                        hbm_29_total_writes_read,
                        hbm_30_total_writes_read,
                        hbm_31_total_writes_read,
};

IntFuncVoid total_reads_len[] = {
                        hbm_0_total_reads_read,
                        hbm_1_total_reads_read,
                        hbm_2_total_reads_read,
                        hbm_3_total_reads_read,
                        hbm_4_total_reads_read,
                        hbm_5_total_reads_read,
                        hbm_6_total_reads_read,
                        hbm_7_total_reads_read,
                        hbm_8_total_reads_read,
                        hbm_9_total_reads_read,
                        hbm_10_total_reads_read,
                        hbm_11_total_reads_read,
                        hbm_12_total_reads_read,
                        hbm_13_total_reads_read,
                        hbm_14_total_reads_read,
                        hbm_15_total_reads_read,
                        hbm_16_total_reads_read,
                        hbm_17_total_reads_read,
                        hbm_18_total_reads_read,
                        hbm_19_total_reads_read,
                        hbm_20_total_reads_read,
                        hbm_21_total_reads_read,
                        hbm_22_total_reads_read,
                        hbm_23_total_reads_read,
                        hbm_24_total_reads_read,
                        hbm_25_total_reads_read,
                        hbm_26_total_reads_read,
                        hbm_27_total_reads_read,
                        hbm_28_total_reads_read,
                        hbm_29_total_reads_read,
                        hbm_30_total_reads_read,
                        hbm_31_total_reads_read,
};

VoidFuncInt last_burst_len[] = {
                        hbm_0_last_burst_len_write,
                        hbm_1_last_burst_len_write,
                        hbm_2_last_burst_len_write,
                        hbm_3_last_burst_len_write,
                        hbm_4_last_burst_len_write,
                        hbm_5_last_burst_len_write,
                        hbm_6_last_burst_len_write,
                        hbm_7_last_burst_len_write,
                        hbm_8_last_burst_len_write,
                        hbm_9_last_burst_len_write,
                        hbm_10_last_burst_len_write,
                        hbm_11_last_burst_len_write,
                        hbm_12_last_burst_len_write,
                        hbm_13_last_burst_len_write,
                        hbm_14_last_burst_len_write,
                        hbm_15_last_burst_len_write,
                        hbm_16_last_burst_len_write,
                        hbm_17_last_burst_len_write,
                        hbm_18_last_burst_len_write,
                        hbm_19_last_burst_len_write,
                        hbm_20_last_burst_len_write,
                        hbm_21_last_burst_len_write,
                        hbm_22_last_burst_len_write,
                        hbm_23_last_burst_len_write,
                        hbm_24_last_burst_len_write,
                        hbm_25_last_burst_len_write,
                        hbm_26_last_burst_len_write,
                        hbm_27_last_burst_len_write,
                        hbm_28_last_burst_len_write,
                        hbm_29_last_burst_len_write,
                        hbm_30_last_burst_len_write,
                        hbm_31_last_burst_len_write,
};

VoidFuncInt burst_quantity_write[] = {
                        hbm_0_burst_quantity_write,
                        hbm_1_burst_quantity_write,
                        hbm_2_burst_quantity_write,
                        hbm_3_burst_quantity_write,
                        hbm_4_burst_quantity_write,
                        hbm_5_burst_quantity_write,
                        hbm_6_burst_quantity_write,
                        hbm_7_burst_quantity_write,
                        hbm_8_burst_quantity_write,
                        hbm_9_burst_quantity_write,
                        hbm_10_burst_quantity_write,
                        hbm_11_burst_quantity_write,
                        hbm_12_burst_quantity_write,
                        hbm_13_burst_quantity_write,
                        hbm_14_burst_quantity_write,
                        hbm_15_burst_quantity_write,
                        hbm_16_burst_quantity_write,
                        hbm_17_burst_quantity_write,
                        hbm_18_burst_quantity_write,
                        hbm_19_burst_quantity_write,
                        hbm_20_burst_quantity_write,
                        hbm_21_burst_quantity_write,
                        hbm_22_burst_quantity_write,
                        hbm_23_burst_quantity_write,
                        hbm_24_burst_quantity_write,
                        hbm_25_burst_quantity_write,
                        hbm_26_burst_quantity_write,
                        hbm_27_burst_quantity_write,
                        hbm_28_burst_quantity_write,
                        hbm_29_burst_quantity_write,
                        hbm_30_burst_quantity_write,
                        hbm_31_burst_quantity_write,
};

IntFuncVoid burst_quantity_read[] = {
                        hbm_0_burst_quantity_read,
                        hbm_1_burst_quantity_read,
                        hbm_2_burst_quantity_read,
                        hbm_3_burst_quantity_read,
                        hbm_4_burst_quantity_read,
                        hbm_5_burst_quantity_read,
                        hbm_6_burst_quantity_read,
                        hbm_7_burst_quantity_read,
                        hbm_8_burst_quantity_read,
                        hbm_9_burst_quantity_read,
                        hbm_10_burst_quantity_read,
                        hbm_11_burst_quantity_read,
                        hbm_12_burst_quantity_read,
                        hbm_13_burst_quantity_read,
                        hbm_14_burst_quantity_read,
                        hbm_15_burst_quantity_read,
                        hbm_16_burst_quantity_read,
                        hbm_17_burst_quantity_read,
                        hbm_18_burst_quantity_read,
                        hbm_19_burst_quantity_read,
                        hbm_20_burst_quantity_read,
                        hbm_21_burst_quantity_read,
                        hbm_22_burst_quantity_read,
                        hbm_23_burst_quantity_read,
                        hbm_24_burst_quantity_read,
                        hbm_25_burst_quantity_read,
                        hbm_26_burst_quantity_read,
                        hbm_27_burst_quantity_read,
                        hbm_28_burst_quantity_read,
                        hbm_29_burst_quantity_read,
                        hbm_30_burst_quantity_read,
                        hbm_31_burst_quantity_read,
};

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

IntFuncVoid get_num_ticks[] = {
                        hbm_0_ticks_read,
                        hbm_1_ticks_read,
                        hbm_2_ticks_read,
                        hbm_3_ticks_read,
                        hbm_4_ticks_read,
                        hbm_5_ticks_read,
                        hbm_6_ticks_read,
                        hbm_7_ticks_read,
                        hbm_8_ticks_read,
                        hbm_9_ticks_read,
                        hbm_10_ticks_read,
                        hbm_11_ticks_read,
                        hbm_12_ticks_read,
                        hbm_13_ticks_read,
                        hbm_14_ticks_read,
                        hbm_15_ticks_read,
                        hbm_16_ticks_read,
                        hbm_17_ticks_read,
                        hbm_18_ticks_read,
                        hbm_19_ticks_read,
                        hbm_20_ticks_read,
                        hbm_21_ticks_read,
                        hbm_22_ticks_read,
                        hbm_23_ticks_read,
                        hbm_24_ticks_read,
                        hbm_25_ticks_read,
                        hbm_26_ticks_read,
                        hbm_27_ticks_read,
                        hbm_28_ticks_read,
                        hbm_29_ticks_read,
                        hbm_30_ticks_read,
                        hbm_31_ticks_read};

VoidFuncInt port_settings[] = {
                        hbm_0_port_settings_write,
                        hbm_1_port_settings_write,
                        hbm_2_port_settings_write,
                        hbm_3_port_settings_write,
                        hbm_4_port_settings_write,
                        hbm_5_port_settings_write,
                        hbm_6_port_settings_write,
                        hbm_7_port_settings_write,
                        hbm_8_port_settings_write,
                        hbm_9_port_settings_write,
                        hbm_10_port_settings_write,
                        hbm_11_port_settings_write,
                        hbm_12_port_settings_write,
                        hbm_13_port_settings_write,
                        hbm_14_port_settings_write,
                        hbm_15_port_settings_write,
                        hbm_16_port_settings_write,
                        hbm_17_port_settings_write,
                        hbm_18_port_settings_write,
                        hbm_19_port_settings_write,
                        hbm_20_port_settings_write,
                        hbm_21_port_settings_write,
                        hbm_22_port_settings_write,
                        hbm_23_port_settings_write,
                        hbm_24_port_settings_write,
                        hbm_25_port_settings_write,
                        hbm_26_port_settings_write,
                        hbm_27_port_settings_write,
                        hbm_28_port_settings_write,
                        hbm_29_port_settings_write,
                        hbm_30_port_settings_write,
                        hbm_31_port_settings_write};

IntFuncVoid exec_done[] = {
                        hbm_0_exec_done_read,
                        hbm_1_exec_done_read,
                        hbm_2_exec_done_read,
                        hbm_3_exec_done_read,

                        hbm_4_exec_done_read,
                        hbm_5_exec_done_read, 
                        hbm_6_exec_done_read,
                        hbm_7_exec_done_read,
                        hbm_8_exec_done_read,
                        hbm_9_exec_done_read,
                        hbm_10_exec_done_read,
                        hbm_11_exec_done_read,
                        hbm_12_exec_done_read,
                        hbm_13_exec_done_read,
                        hbm_14_exec_done_read,
                        hbm_15_exec_done_read,
                        hbm_16_exec_done_read,
                        hbm_17_exec_done_read,
                        hbm_18_exec_done_read,
                        hbm_19_exec_done_read,
                        hbm_20_exec_done_read,
                        hbm_21_exec_done_read,
                        hbm_22_exec_done_read,
                        hbm_23_exec_done_read,
                        hbm_24_exec_done_read,
                        hbm_25_exec_done_read,
                        hbm_26_exec_done_read,
                        hbm_27_exec_done_read,
                        hbm_28_exec_done_read,
                        hbm_29_exec_done_read,
                        hbm_30_exec_done_read,
                        hbm_31_exec_done_read};

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

void set_burst_enable_csrs(uint32_t num_bytes, uint32_t ports_mask, uint32_t address, uint32_t addr_offset) {

    //int port_quantity = 0;

    // Enable certain ports to start
    commonRegs_ports_mask_write(ports_mask | commonRegs_ports_mask_read());
    
    // Calculate number of transactions with the number of bytes.
    uint64_t num_bursts = (uint64_t)((uint64_t)((num_bytes + (BYTES_PER_BEAT - 1)) / BYTES_PER_BEAT) + (BEATS_PER_BURST - 1)) / BEATS_PER_BURST;

    // Calculate the number of beats for the last transaction with the number of bytes.
    uint32_t last_burst_beats = ((num_bytes + (BYTES_PER_BEAT - 1)) / BYTES_PER_BEAT) % BEATS_PER_BURST;

    // Set the number of beats to 16 for each transaction in every port (except the last).
    for (int i = 0; i < sizeof(burst_length_write) / sizeof(*burst_length_write); i++)
    {
        burst_length_write[i](BEATS_PER_BURST);
    }

    // Set the number of transactions for every used port.
    for (int i = 0; i < sizeof(burst_quantity_write) / sizeof(*burst_quantity_write); i++)
    {
        if (ports_mask & (0x00000001 << i)) {
            burst_quantity_write[i](num_bursts);
        }
    }

    // Set the number of beats for the last transaction in every used port.
    for (int i = 0; i < sizeof(last_burst_len) / sizeof(*last_burst_len); i++) 
    {
        if (ports_mask & (0x00000001 << i)) {
            last_burst_len[i](last_burst_beats);
        }
    }

    // Set the base address for every port (address = first used port, address + offset = second, etc.)
    for (int i = 0; i < sizeof(address_readwrite) / sizeof(*address_readwrite); i++)
    {
        if (ports_mask & (0x00000001 << i)) {
            address_readwrite[i](address);
            address += addr_offset;
        }
    }

    // Return the number of used ports.
    return;
}

void hbm_set_reading_ports(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask, uint32_t delay_cycles) {

    // Sets csrs and returns the number of used ports for reading.
    set_burst_enable_csrs(num_bytes, ports_mask, address, addr_offset);

    // Set the setting of all ports to option read, and number of cycles to delay.
    for (int i = 0; i < sizeof(port_settings) / sizeof(*port_settings); i++)
    {
        if (ports_mask & (0x00000001 << i)) {
            port_settings[i](OPTION_READ);
            set_delay[i](delay_cycles);
        }
    }

}

void hbm_set_writing_ports(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask, uint32_t delay_cycles, uint32_t data_pattern) {
    
    // Set pattern of what to write
    commonRegs_data_pattern_write(data_pattern);

    // Sets csrs and returns the number of used ports for writing.
    set_burst_enable_csrs(num_bytes, ports_mask, address, addr_offset);

    // Set the setting of all ports to option write, and number of cycles to delay.
    for (int i = 0; i < sizeof(port_settings) / sizeof(*port_settings); i++)
    {
        if (ports_mask & (0x00000001 << i)) {
            port_settings[i](OPTION_WRITE);
            set_delay[i](delay_cycles);
        }
    }

}

void hbm_test_start(void) {
    
    // Start all hbm fsms at once.
    commonRegs_start_write(1);

    printf("Started hbm ports\n");

}

void hbm_test_finish(void) {
    
    // Start all hbm fsms at once.
    commonRegs_start_write(0);

    // Confirm that all ports have made it to the WAIT state.
    int func_running_flag = 0;
    while (func_running_flag < TOTAL_PORTS) {
        func_running_flag = 0;
        for (int i = 0; i < sizeof(exec_done) / sizeof(*exec_done); i++)
        {
            func_running_flag += exec_done[i]();
        }
    }

    printf("Stopped all hbm ports\n");
}

void hbm_bandwidth(void) {

    // Pause all state machines, check that they are all paused or in wait state (exec_done)
    commonRegs_delay_force_write(1);

    // Make sure all running ports have paused.
    int func_pause_flag = 0;
    while (func_pause_flag < TOTAL_PORTS) {
        func_pause_flag = 0;
        for (int i = 0; i < sizeof(delay_state_fsm) / sizeof(*delay_state_fsm); i++) {
            if (delay_state_fsm[i]()) {
                func_pause_flag++;
            } else if (exec_done[i]()) {
                func_pause_flag++;
            }
        }
    }

    // Begin to write all necessary data.
    printf("Port #   CLK-CYCCNT    WRITES     READS     W-BW(MiB)     R-BW(MiB)\n");

    int numticks = 0;
    uint64_t wbandwidth = 0;
    uint64_t rbandwidth = 0;
    for (uint8_t i = 0; i < sizeof(get_num_ticks) / sizeof(*get_num_ticks); i++) {
        numticks = get_num_ticks[i]();
        if (!exec_done[i]()) {
            if (numticks != 0) {
                wbandwidth = (uint64_t)((total_writes_len[i]() * BYTES_PER_BEAT * (CONFIG_CLOCK_FREQUENCY / 1048576))) / numticks;
                rbandwidth = (uint64_t)((total_reads_len[i]() * BYTES_PER_BEAT * (CONFIG_CLOCK_FREQUENCY / 1048576)) / numticks);
                printf("%6u %11u %9u %9u %13llu %13llu\n", i, numticks, total_writes_len[i](), total_reads_len[i](), wbandwidth, rbandwidth);
            } else {
                printf("%6u %11u %9u %9u --            --             \n", i, numticks, total_writes_len[i](), total_reads_len[i]());
            }
        } else {
            printf("%6u (Not running)\n", i);
        }
    }

    // Unpause running ports.
    commonRegs_delay_force_write(0);
}

// void hbm_data_dependent_bandwidth(void) {
//     //Calculate bandwidth
//     uint64_t numticks = 0;
//     uint64_t bandwidth = 0;
//     for (int i = 0; i < sizeof(get_num_ticks) / sizeof(*get_num_ticks); i++)
//     {
//         numticks = get_num_ticks[i]();
//         if (numticks != 0) {
//             bandwidth = (uint64_t)((burst_quantity_read[i]() * BEATS_PER_BURST * BYTES_PER_BEAT * (CONFIG_CLOCK_FREQUENCY / 1048576)) / numticks); // 1048576 = 1024 * 1024
//             printf("Number of Bursts: %u\n", burst_quantity_read[i]());
//             printf("Number of Ticks: %llu\n", numticks);
//             printf("Bandwidth for port %u: %llu MiB/s\n\n", i, bandwidth);
//         }
//     }
// }

// void hbm_cycles_dependent_bandwidth(void) {
//     uint64_t write_bandwidth = 0;
//     uint64_t read_bandwidth = 0;

//     printf("Port     W-BW(MiB) R-BW(MiB)\n");
//     for (int i = 0; i < sizeof(total_writes_len) / sizeof(*total_writes_len); i++) {
//         write_bandwidth = (uint64_t)((total_writes_len[i]() * BYTES_PER_BEAT * (CONFIG_CLOCK_FREQUENCY / 1048576)) / 10000); // 1048576 = 1024 * 1024
//         read_bandwidth = (uint64_t)((total_reads_len[i]() * BYTES_PER_BEAT * (CONFIG_CLOCK_FREQUENCY / 1048576)) / 10000); // 1048576 = 1024 * 1024
//         printf("%9u %9llu %9llu\n", i, write_bandwidth, read_bandwidth);
//     }
// }

// void hbm_set_write_read_ports(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask, uint32_t delay_cycles) {
    
//     // Sets csrs and returns the number of used ports for writing, then reading.
//     int num_ports = set_burst_enable_csrs(num_bytes, ports_mask, address, addr_offset);

//     // Set the setting of all ports to option write, and number of cycles to delay.
//     for (int i = 0; i < sizeof(port_settings) / sizeof(*port_settings); i++)
//     {
//         if (ports_mask & (0x00000001 << i)) {
//             port_settings[i](OPTION_WRITE);
//             set_delay[i](delay_cycles);
//         }
//     }
    
// }

//////////////////////////////////////////////////////////////////////////////
// Start of newer functions
//////////////////////////////////////////////////////////////////////////////

// void set_burst_enable_csrs(uint32_t num_bytes, uint32_t ports_mask, uint32_t address, uint32_t addr_offset) {

//     //int port_quantity = 0;

//     // Enable certain ports to start
//     commonRegs_ports_mask_write(ports_mask | commonRegs_ports_mask_read());
    
//     // Calculate number of transactions with the number of bytes.
//     uint64_t num_bursts = (uint64_t)((uint64_t)((num_bytes + (BYTES_PER_BEAT - 1)) / BYTES_PER_BEAT) + (BEATS_PER_BURST - 1)) / BEATS_PER_BURST;

//     // Calculate the number of beats for the last transaction with the number of bytes.
//     uint32_t last_burst_beats = ((num_bytes + (BYTES_PER_BEAT - 1)) / BYTES_PER_BEAT) % BEATS_PER_BURST;

//     // Set the number of beats to 16 for each transaction in every port (except the last).
//     for (int i = 0; i < sizeof(burst_length_write) / sizeof(*burst_length_write); i++)
//     {
//         burst_length_write[i](BEATS_PER_BURST);
//     }

//     // Set the number of transactions for every used port.
//     for (int i = 0; i < sizeof(burst_quantity_write) / sizeof(*burst_quantity_write); i++)
//     {
//         if (ports_mask & (0x00000001 << i)) {
//             burst_quantity_write[i](num_bursts);
//         }
//     }

//     // Set the number of beats for the last transaction in every used port.
//     for (int i = 0; i < sizeof(last_burst_len) / sizeof(*last_burst_len); i++) 
//     {
//         if (ports_mask & (0x00000001 << i)) {
//             last_burst_len[i](last_burst_beats);
//         }
//     }

//     // Set the base address for every port (address = first used port, address + offset = second, etc.)
//     for (int i = 0; i < sizeof(address_readwrite) / sizeof(*address_readwrite); i++)
//     {
//         if (ports_mask & (0x00000001 << i)) {
//             address_readwrite[i](address);
//             address += addr_offset;
//         }
//     }

//     // Return the number of used ports.
//     return;
// }

// void hbm_set_reading_ports(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask, uint32_t delay_cycles) {

//     // Sets csrs and returns the number of used ports for reading.
//     set_burst_enable_csrs(num_bytes, ports_mask, address, addr_offset);

//     // Set the setting of all ports to option read, and number of cycles to delay.
//     for (int i = 0; i < sizeof(port_settings) / sizeof(*port_settings); i++)
//     {
//         if (ports_mask & (0x00000001 << i)) {
//             port_settings[i](OPTION_READ);
//             set_delay[i](delay_cycles);
//         }
//     }

// }

// void hbm_set_writing_ports(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask, uint32_t delay_cycles, uint32_t data_pattern) {
    
//     // Set pattern of what to write
//     commonRegs_data_pattern_write(data_pattern);

//     // Sets csrs and returns the number of used ports for writing.
//     set_burst_enable_csrs(num_bytes, ports_mask, address, addr_offset);

//     // Set the setting of all ports to option write, and number of cycles to delay.
//     for (int i = 0; i < sizeof(port_settings) / sizeof(*port_settings); i++)
//     {
//         if (ports_mask & (0x00000001 << i)) {
//             port_settings[i](OPTION_WRITE);
//             set_delay[i](delay_cycles);
//         }
//     }

// }

// void hbm_test_start(void) {
    
//     // Start all hbm fsms at once.
//     commonRegs_start_write(1);

//     printf("Started hbm ports\n");

// }

// void hbm_test_finish(void) {
    
//     // Start all hbm fsms at once.
//     commonRegs_start_write(0);

//     // Confirm that all ports have made it to the WAIT state.
//     int func_running_flag = 0;
//     while (func_running_flag < TOTAL_PORTS) {
//         func_running_flag = 0;
//         for (int i = 0; i < sizeof(exec_done) / sizeof(*exec_done); i++)
//         {
//             func_running_flag += exec_done[i]();
//         }
//     }

//     printf("Stopped all hbm ports\n");
// }

// // void hbm_set_write_read_ports(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask, uint32_t delay_cycles) {
    
// //     // Sets csrs and returns the number of used ports for writing, then reading.
// //     int num_ports = set_burst_enable_csrs(num_bytes, ports_mask, address, addr_offset);

// //     // Set the setting of all ports to option write, and number of cycles to delay.
// //     for (int i = 0; i < sizeof(port_settings) / sizeof(*port_settings); i++)
// //     {
// //         if (ports_mask & (0x00000001 << i)) {
// //             port_settings[i](OPTION_WRITE);
// //             set_delay[i](delay_cycles);
// //         }
// //     }
    
// // }




// //////////////////////////////////////////////////////////////////////////////
// // Fix and add
// //////////////////////////////////////////////////////////////////////////////

// void hbm_data_dependent_bandwidth(void) {
//     //Calculate bandwidth
//     uint64_t numticks = 0;
//     uint64_t bandwidth = 0;
//     for (int i = 0; i < sizeof(get_num_ticks) / sizeof(*get_num_ticks); i++)
//     {
//         numticks = get_num_ticks[i]();
//         if (numticks != 0) {
//             bandwidth = (uint64_t)((burst_quantity_read[i]() * BEATS_PER_BURST * BYTES_PER_BEAT * (CONFIG_CLOCK_FREQUENCY / 1048576)) / numticks); // 1048576 = 1024 * 1024
//             printf("Number of Bursts: %u\n", burst_quantity_read[i]());
//             printf("Number of Ticks: %llu\n", numticks);
//             printf("Bandwidth for port %u: %llu MiB/s\n\n", i, bandwidth);
//         }
//     }
// }

// void hbm_cycles_dependent_bandwidth(void) {
//     uint64_t write_bandwidth = 0;
//     uint64_t read_bandwidth = 0;

//     printf("Port     W-BW(MiB) R-BW(MiB)\n");
//     for (int i = 0; i < sizeof(total_writes_len) / sizeof(*total_writes_len); i++) {
//         write_bandwidth = (uint64_t)((total_writes_len[i]() * BYTES_PER_BEAT * (CONFIG_CLOCK_FREQUENCY / 1048576)) / 10000); // 1048576 = 1024 * 1024
//         read_bandwidth = (uint64_t)((total_reads_len[i]() * BYTES_PER_BEAT * (CONFIG_CLOCK_FREQUENCY / 1048576)) / 10000); // 1048576 = 1024 * 1024
//         printf("%9u %9llu %9llu\n", i, write_bandwidth, read_bandwidth);
//     }
// }

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// End of new functions
////////////////////////////////////////////////////////////////////////////

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

////////////////////////////////////////////////////////////////////////////
// Older code
////////////////////////////////////////////////////////////////////////////



// void hbm_test(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask) {

//     uint64_t num_bursts = (uint64_t)((uint64_t)((num_bytes + (BYTES_PER_BEAT - 1)) / BYTES_PER_BEAT) + (BEATS_PER_BURST - 1)) / BEATS_PER_BURST;

//     uint32_t last_burst_beats = ((num_bytes + (BYTES_PER_BEAT - 1)) / BYTES_PER_BEAT) % BEATS_PER_BURST;

//     commonRegs_ports_mask_write(ports_mask);

//     int port_quantity = 0;

//     for (int i = 0; i < 32; i++) {
//         if (ports_mask & (0x00000001 << i)) {
//             port_quantity++;
//         }
//     }

//     // Setting key values for Write/Read Commands
//     for (int i = 0; i < sizeof(burst_length_write) / sizeof(*burst_length_write); i++)
//     {
//         burst_length_write[i](BEATS_PER_BURST);
//     }

//     for (int i = 0; i < sizeof(burst_quantity_write) / sizeof(*burst_quantity_write); i++)
//     {
//         burst_quantity_write[i](num_bursts);
//     }

//     for (int i = 0; i < sizeof(last_burst_len) / sizeof(*last_burst_len); i++) 
//     {
//         last_burst_len[i](last_burst_beats);
//     }

//     printf("burst done\n");

//     // Preparing Write Command
//     for (int i = 0; i < sizeof(address_readwrite) / sizeof(*address_readwrite); i++)
//     {
//         address_readwrite[i](address);
//         address += addr_offset;
//     }

//     printf("Address done\n");

//     for (int i = 0; i < sizeof(port_settings) / sizeof(*port_settings); i++)
//     {
//         port_settings[i](OPTION_WRITE);
//     }

//     commonRegs_start_write(1);
//     printf("Started perform write, waiting for response\n");

//     int func_running_flag = 0;
//     while (func_running_flag < port_quantity) {
//         func_running_flag = 0;
//         for (int i = 0; i < sizeof(exec_done) / sizeof(*exec_done); i++)
//         {
//             func_running_flag += exec_done[i]();
            
//         }
//     }

//     printf("Done Write\n");

//     //Calculate bandwidth
//     uint64_t numticks = 0;
//     uint64_t bandwidth = 0;
//     for (int i = 0; i < sizeof(get_num_ticks) / sizeof(*get_num_ticks); i++)
//     {
//         numticks = get_num_ticks[i]();
//         if (numticks != 0) {
//             bandwidth = (uint64_t)((num_bursts * BEATS_PER_BURST * BYTES_PER_BEAT * (CONFIG_CLOCK_FREQUENCY / 1048576)) / numticks); // 1048576 = 1024 * 1024
//             printf("Number of Bursts: %llu\n", num_bursts);
//             printf("Number of Ticks: %llu\n", numticks);
//             printf("Bandwidth for port %u: %llu MiB/s\n\n", i, bandwidth);
//         }
//     }

//     // Restore state machine back to wait state
//     commonRegs_start_write(0);

//     // Going back to "Wait" state
//     while (func_running_flag != 0) {
//         func_running_flag = 0;
//         for (int i = 0; i < sizeof(exec_done) / sizeof(*exec_done); i++)
//         {
//             func_running_flag += exec_done[i]();
//         }
//     }

//     // Preparing Reading Command
//     for (int i = 0; i < sizeof(port_settings) / sizeof(*port_settings); i++)
//     {
//         port_settings[i](OPTION_READ);
//     }

//     printf("Started perform read, waiting for response\n");
//     commonRegs_start_write(1);

//     while (func_running_flag < port_quantity) {
//         func_running_flag = 0;
//         for (int i = 0; i < sizeof(exec_done) / sizeof(*exec_done); i++)
//         {
//             func_running_flag += exec_done[i]();
//         }
//         // printf("Val of running flag: %d\n", func_running_flag);
//     }

//     printf("Done Read\n");

//     // Calculate bandwidth
//     numticks = 0;
//     bandwidth = 0;
//     for (int i = 0; i < sizeof(get_num_ticks) / sizeof(*get_num_ticks); i++)
//     {
//         numticks = get_num_ticks[i]();
//         if (numticks != 0) {
//             bandwidth = (uint64_t)((num_bursts * BEATS_PER_BURST * BYTES_PER_BEAT * (CONFIG_CLOCK_FREQUENCY / 1048576)) / numticks); // 1048576 = 1024 * 1024
//             printf("Number of Bursts: %llu\n", num_bursts);
//             printf("Number of Ticks: %llu\n", numticks);
//             printf("Bandwidth for port %u: %llu MiB/s\n\n", i, bandwidth);
//         }
//     }

//     // Restore state machine back to wait state
//     commonRegs_start_write(0);

//     while (func_running_flag != 0) {
//         func_running_flag = 0;
//         for (int i = 0; i < sizeof(exec_done) / sizeof(*exec_done); i++)
//         {
//             func_running_flag += exec_done[i]();
//         }
//     }

//     // printf("Perf2 done\n");


//     // // for (int i = 0; i < sizeof(acknowledge_readwrite) / sizeof(*acknowledge_readwrite); i++)
//     // // {
//     // //     acknowledge_readwrite[i](1);
//     // // }

//     // // printf("ack done\n");

//     // // Delay 500 ms
//     // cdelay(500);

//     // // // Perform read
//     // // for (int i = 0; i < sizeof(acknowledge_readwrite) / sizeof(*acknowledge_readwrite); i++)
//     // // {
//     // //     acknowledge_readwrite[i](0);
//     // // }

//     // // printf("ack3 done\n");

//     // printf("Started perform read, waiting for response\n");
//     // for (int i = 0; i < sizeof(perform_read) / sizeof(*perform_read); i++)
//     // {
//     //     perform_read[i](1);
//     // }

//     // printf("perform read done\n");

//     // func_running_flag = 0;
//     // while (func_running_flag < TOTAL_PORTS) {
//     //     func_running_flag = 0;
//     //     for (int i = 0; i < sizeof(exec_read_done) / sizeof(*exec_read_done); i++)
//     //     {
//     //         func_running_flag += exec_read_done[i]();
//     //     }
//     // }

//     // printf("Done\n");

//     // // Restore state machine back to wait state
//     // for (int i = 0; i < sizeof(perform_read) / sizeof(*perform_read); i++)
//     // {
//     //     perform_read[i](0);
//     // }

//     // // for (int i = 0; i < sizeof(acknowledge_readwrite) / sizeof(*acknowledge_readwrite); i++)
//     // // {
//     // //     acknowledge_readwrite[i](1);
//     // // }

//     printf("\nEnd of program\n");
    
// }

////////////////////////////////////////////////////////////////////////////////



