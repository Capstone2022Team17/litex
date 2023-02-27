// This file is Copyright (c) 2018-2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#ifndef __HBM_SDRAM_BIST_H
#define __HBM_SDRAM_BIST_H

void hbm_gen(uint32_t burst_length);
void hbm_write_fsm(uint32_t data1, uint32_t data2, uint32_t data3, uint32_t data4, uint32_t data5, uint32_t data6, uint32_t data7, uint32_t data8, uint32_t address, uint32_t strb);
void hbm_read_fsm(uint32_t address);
void hbm_test(uint32_t address, uint32_t addr_offset);

#endif /* __HBM_SDRAM_BIST_H */