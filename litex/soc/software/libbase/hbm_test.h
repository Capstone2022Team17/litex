// This file is Copyright (c) 2018-2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#ifndef __HBM_SDRAM_TEST_H
#define __HBM_SDRAM_TEST_H

// void hbm_gen(uint32_t burst_length);
// void hbm_write_fsm(uint32_t data1, uint32_t data2, uint32_t data3, uint32_t data4, uint32_t data5, uint32_t data6, uint32_t data7, uint32_t data8, uint32_t address, uint32_t strb);
// void hbm_read_fsm(uint32_t address);
// void hbm_test(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask);
// void hbm_read(uint32_t portCount, uint32_t addr_offset_port, uint32_t addr_base, uint32_t num_bytes, uint32_t addr_length);
// void hbm_write(uint32_t portCount, uint32_t addr_offset_port, uint32_t addr_base, uint32_t num_bytes, uint32_t addr_length, uint32_t pattern, uint32_t random_flag);
// void hbm_test(uint32_t portCount, uint32_t addr_offset_port, uint32_t addr_base, uint32_t num_bytes, uint32_t addr_length, uint32_t pattern, uint32_t random_flag);

void hbm_set_writing_ports(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask, uint32_t delay_cycles, uint32_t data_pattern);
void hbm_set_reading_ports(uint32_t address, uint32_t addr_offset, uint32_t num_bytes, uint32_t ports_mask, uint32_t delay_cycles);
void hbm_test_start(void);
void hbm_test_finish(void);
void hbm_bandwidth(void);
// void hbm_data_dependent_bandwidth(void);
// void hbm_cycles_dependent_bandwidth(void);

#endif /* __HBM_SDRAM_BIST_H */