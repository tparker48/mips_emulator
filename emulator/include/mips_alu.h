#pragma once

#include <stdint.h>

#include "mips_pipeline.h"
#include "mips_registers.h"


void execute_r();
void execute_i();
void execute_j();

// R-type instructions
void sll();
void srl();
void sra();
void jr();
void jalr();
void syscall();
void mfhi();
void mthi();
void mflo();
void mtlo();
void mult();
void multu();
void div_();
void divu();
void add();
void addu();
void sub();
void subu();
void and ();
void or();
void xor(); 
void nor();
void slt();
void sltu();

// I-type instructions
void beq();
void bne();
void blez();
void bgtz();
void addi();
void addiu();
void slti();
void sltiu();
void andi();
void ori();
void lui();

// J-type instructions
void j();
void jal();

// Utils
void write_register(uint8_t reg_to_write, uint32_t out);
void prepare_memory_read();
void prepare_memory_write();
