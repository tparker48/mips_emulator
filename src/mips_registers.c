#include "mips_registers.h"
#include "mips_pipeline.h"

uint32_t registers[32];
uint32_t fregisters[32];
uint32_t cop0[32];
uint32_t hi, lo;
uint32_t pc, pc_cap;
