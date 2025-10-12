#pragma once

#include <stdbool.h>
#include <stdint.h>

// Instruction opcodes
#define OP_ZERO  0x00
#define OP_BEQ   0x04
#define OP_BNE   0x05
#define OP_BLEZ  0x06
#define OP_BGTZ  0x07
#define OP_ADDI  0x08
#define OP_ADDIU 0x09
#define OP_SLTI  0x0A
#define OP_SLTIU 0x0B
#define OP_ANDI  0x0C
#define OP_ORI   0x0D
#define OP_LUI   0x0F
#define OP_LB    0x20 
#define OP_LH    0x21 
#define OP_LW    0x23 
#define OP_LBU   0x24
#define OP_LHU   0x25
#define OP_SB    0x28
#define OP_SH    0x29
#define OP_SW    0x2B
#define OP_J     0x02
#define OP_JAL   0x03

// I-type funct codes
#define FUNC_SLL   0x00
#define FUNC_SRL   0x02
#define FUNC_SRA   0x03
#define FUNC_JR    0x08
#define FUNC_JALR  0x09
#define FUNC_MFHI  0x10
#define FUNC_MTHI  0x11
#define FUNC_MFLO  0x12
#define FUNC_MTLO  0x13
#define FUNC_MULT  0x18
#define FUNC_MULTU 0x19
#define FUNC_DIV   0x1A
#define FUNC_DIVU  0x1B
#define FUNC_ADD   0x20
#define FUNC_ADDU  0x21
#define FUNC_SUB   0x22
#define FUNC_SUBU  0x23
#define FUNC_AND   0x24
#define FUNC_OR    0x25
#define FUNC_XOR   0x26
#define FUNC_NOR   0x27
#define FUNC_SLT   0x2A
#define FUNC_SLTU  0x2B

bool is_r_type(uint8_t op_code);
bool is_j_type(uint8_t op_code);
bool is_i_type(uint8_t op_code);

bool reads_mem(uint8_t op_code);
bool writes_hilo(uint8_t op_code, uint8_t funct);
bool reads_hilo(uint8_t op_code, uint8_t funct);