#include <stdio.h>
#include <stdbool.h>

#include "mips_alu.h"
#include "mips_memory.h"
#include "mips_instructions.h"

void execute_r()
{
    switch (EXE.funct)
    {
    case FUNC_SLL: sll(); break;
    case FUNC_SRL: srl(); break;
    case FUNC_SRA: sra(); break;
    case FUNC_JR: jr(); break;
    case FUNC_JALR: jalr(); break;
    case FUNC_SYSCALL: syscall(); break;
    case FUNC_MFHI: mfhi(); break;
    case FUNC_MTHI: mthi(); break;
    case FUNC_MFLO: mflo(); break;
    case FUNC_MTLO: mtlo(); break;
    case FUNC_MULT: mult(); break;
    case FUNC_MULTU: multu(); break;
    case FUNC_DIV: div_(); break;
    case FUNC_DIVU: divu(); break;
    case FUNC_ADD: add(); break;
    case FUNC_ADDU: addu(); break;
    case FUNC_SUB: sub(); break;
    case FUNC_SUBU: subu(); break;
    case FUNC_AND: and(); break;
    case FUNC_OR: or(); break;
    case FUNC_XOR: xor(); break;
    case FUNC_NOR: nor(); break;
    case FUNC_SLT: slt(); break;
    case FUNC_SLTU: sltu(); break;
    }
}

void execute_i()
{
    switch (EXE.op_code)
    {
    case OP_BEQ:   beq(); break;
    case OP_BNE:   bne(); break;
    case OP_BLEZ:  blez(); break;
    case OP_BGTZ:  bgtz(); break;
    case OP_ADDI:  addi(); break;
    case OP_ADDIU: addiu(); break;
    case OP_SLTI:  slti(); break;
    case OP_SLTIU: sltiu(); break;
    case OP_ANDI:  andi(); break;
    case OP_ORI:   ori(); break;
    case OP_LUI:   lui(); break;
    case OP_LB: prepare_memory_read(); break;  // lb() called during MEM stage
    case OP_LH: prepare_memory_read(); break;  // lh() called during MEM stage
    case OP_LW: prepare_memory_read(); break;  // lw() called during MEM stage
    case OP_LBU: prepare_memory_read(); break;  // lbu() called during MEM stage
    case OP_LHU: prepare_memory_read(); break;  // lhu() called during MEM stage
    case OP_SB: sb(); break;
    case OP_SH: sh(); break;
    case OP_SW: sw(); break;
    }
}

void execute_j()
{
    uint8_t op_code = EXE.op_code;
    switch(op_code)
    {
    case OP_J: j(); break;
    case OP_JAL: jal(); break;
    }
}

// R
void sll() {

}
void srl() {

}
void sra() {

}
void jr() {
    pc = EXE.rs;
    MEM.noop = true;
}
void jalr() {
    write_register(ra, pc);
    pc = EXE.rs;
}
void syscall(){
    trigger_trap(pc, 8);
    MEM.noop = true;
    ID.noop = true;
}
void mfhi() {

}
void mthi() {

}
void mflo() {

}
void mtlo() {

}
void mult() {

}
void multu() {

}
void div_() {
    if (EXE.rt == 0){
        // divide by zero exception
        MEM.noop = true;
        return;
    }
    
}
void divu() {
    if (EXE.rt == 0)
    {
        // divide by zero exception
        MEM.noop = true;
        return;
    }

}
void add() {
    int32_t t1 = (int32_t)EXE.rs;
    int32_t t2 = (int32_t)EXE.rt;
    int32_t result = t1 + t2;
    if (t1 > 0 && t2 > 0 && result < 0)
    {
        // overflow exception
        MEM.noop = true;
        return;
    }
    if (t1 < 0 && t2 < 0 && result > 0)
    {
        // overflow exception
        MEM.noop = true;
        return;
    }
    write_register(EXE.rd_id, result);
}

void addu() {
    write_register(EXE.rd_id, EXE.rs + EXE.rt);
}
void sub() {
    int32_t t1 = (int32_t)EXE.rs;
    int32_t t2 = (int32_t)EXE.rt;
    int32_t result = t1-t2;
    if (t1 > 0 && t2 < 0 && result < 0){
        // overflow exception
        MEM.noop = true;
        return;
    }
    if (t1 < 0 && t2 > 0 && result > 0){
        // overflow exception
        MEM.noop = true;
        return;
    }
    
    write_register(EXE.rd_id, result);
}
void subu() {
    write_register(EXE.rd_id, EXE.rs - EXE.rt);
}
void and() {
    write_register(EXE.rd_id, EXE.rs & EXE.rt);
}
void or() {
    write_register(EXE.rd_id, EXE.rs | EXE.rt);
}
void xor() {
    write_register(EXE.rd_id, EXE.rs ^ EXE.rt);
}
void nor() {
    write_register(EXE.rd_id, ~(EXE.rs | EXE.rt));
}
void slt() {
    write_register(EXE.rt_id, (EXE.rs < EXE.immediate_se ? 1: 0));
}
void sltu() {
    write_register(EXE.rt_id, (EXE.rs < (uint32_t)EXE.immediate_se ? 1 : 0));
}

// I
void beq() {}
void bne() {}
void blez() {}
void bgtz() {}
void addi() {}
void addiu() {}
void slti() {}
void sltiu() {}
void andi() {}
void ori() {}
void lui() {}

// J
void j() {}
void jal() {}

// utils
void write_register(uint8_t reg_to_write, uint32_t out)
{
    MEM.write_reg = true;
    MEM.register_to_write = reg_to_write;
    MEM.alu_out = out;
}
void prepare_memory_read(){
    MEM.read_mem = true;
    MEM.alu_out = EXE.rs + EXE.immediate_se;
}
void prepare_memory_write()
{
    MEM.write_mem = true;
    MEM.reg_out = EXE.rt;
    MEM.alu_out = EXE.rs + EXE.immediate_se;
}
