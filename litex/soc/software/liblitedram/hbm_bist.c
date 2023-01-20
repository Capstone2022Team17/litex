
#include <generated/csr.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <uart.h>
#include <time.h>
#include <console.h>

#include <liblitedram/hbm_bist.h>

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

void whatstateamI() {
    if (hbm_4_waitinstruction_fsm_read()) {printf("FSM: Wait instruction\n"); }
    if (hbm_4_prepwritecommand_fsm_read()) {printf("FSM: Stage 1 write\n"); }
    // if (hbm_4_prepwrite_fsm_read()) {printf("FSM: Stage 2 write\n"); }
    if (hbm_4_prepwriteresponse_fsm_read()) {printf("FSM: Stage 3 write\n"); }
    if (hbm_4_donewrite_fsm_read()) {printf("FSM: Done with write\n"); }
    if (hbm_4_prepreadcommand_fsm_read()) {printf("FSM: Stage 1 read\n"); }
    if (hbm_4_prepread_fsm_read()) {printf("FSM: Stage 2 read\n"); }
    if (hbm_4_doneread_fsm_read()) {printf("FSM: Done with read\n"); }
}

void hbm_write_fsm(uint32_t data, uint32_t address, uint32_t strb) {
    hbm_4_acknowledge_readwrite_write(0);
    hbm_4_address_readwrite_write(address);
    hbm_4_data_writein_write(data);
    hbm_4_strb_readwrite_write(strb);
    cdelay(100);
    printf("Set up variables, csrs: data: %08x, address: %08x, strb: %08x\n", hbm_4_data_writein_read(), hbm_4_address_readwrite_read(), hbm_4_strb_readwrite_read());
    printf("Started perform write, waiting for response\n");
    hbm_4_perform_write_write(1);
    cdelay(100);
    printf("Value of exec_write_done = %x\n", hbm_4_exec_write_done_read());
    printf("Waiting...\n");
    whatstateamI();
    while (hbm_4_exec_write_done_read() == 0) {
        whatstateamI();
        cdelay(500);
    }
    printf("Done\n");
    printf("Write response: %x\n", hbm_4_write_resp_read());
    whatstateamI();
    hbm_4_perform_write_write(0);
    hbm_4_acknowledge_readwrite_write(1);
    whatstateamI();
}

void hbm_read_fsm(uint32_t address) {
    hbm_4_acknowledge_readwrite_write(0);
    hbm_4_address_readwrite_write(address);
    cdelay(100);
    printf("Set up variables, csrs: address: %08x\n", hbm_4_address_readwrite_read());
    printf("Started perform write, waiting for response\n");
    hbm_4_perform_read_write(1);
    cdelay(100);
    printf("Value of exec_read_done = %x\n", hbm_4_exec_read_done_read());
    printf("Waiting...\n");
    whatstateamI();
    while (hbm_4_exec_read_done_read() == 0) {
        whatstateamI();
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
    whatstateamI();
    hbm_4_perform_read_write(0);
    hbm_4_acknowledge_readwrite_write(1);
    whatstateamI();
}