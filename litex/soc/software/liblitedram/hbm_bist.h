// This file is Copyright (c) 2018-2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#ifndef __HBM_SDRAM_BIST_H
#define __HBM_SDRAM_BIST_H

void hbm_gen(uint32_t burst_length);
void hbm_write_fsm(uint32_t data, uint32_t address, uint32_t strb);
void hbm_read_fsm(uint32_t address);

#endif /* __HBM_SDRAM_BIST_H */