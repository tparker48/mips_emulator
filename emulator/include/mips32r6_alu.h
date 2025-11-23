#pragma once

#include <stdbool.h>
#include <stdint.h>

void execute_instruction();

void execute_special();
void execute_regimm();
void execute_special3();
void execute_pcrel();
void execute_bshfl();

void execute_pop06();
void execute_pop07();
void execute_pop10();
void execute_pop26();
void execute_pop27();
void execute_pop30();
void execute_pop66();
void execute_pop76();
void sop30();
void sop31();
void sop32();
void sop33();
void execute_srl();
void execute_srlv();

// Bad encodings
void reserved_B();
void reserved_star();
void reserved_no_module();
void removed_r6();
void deprecated();
void execute_cop0();
void execute_cop1();
void execute_cop2();

// Helpers
bool reads_mem(uint8_t op_code);
