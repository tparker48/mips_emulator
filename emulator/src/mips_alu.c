#include <stdio.h>
#include <stdbool.h>

#include "mips_alu.h"
#include "mips_memory.h"
#include "mips_instructions.h"
#include "mips_os.h"

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
    write_register(EXE.rd_id, EXE.rt << EXE.shamt);
}
void srl() {
    write_register(EXE.rd_id, EXE.rt >> EXE.shamt);
}
void sra() {
    int32_t val = (int32_t)EXE.rt;
    uint32_t result = ((uint32_t)val) >> EXE.shamt;

    if (val < 0){
        result |= ~((~0u) >> EXE.shamt);
    }

    write_register(EXE.rd_id, result);
}
void jr() {
    pc = EXE.rs;
}
void jalr() {
    write_register(ra, pc);
    pc = EXE.rs;
}
void syscall(){
    trigger_trap(pc, 8);
    ID.noop = true;
}
void mfhi() {
    write_register(EXE.rd, hi);
}
void mthi() {
    write_hilo(EXE.rs, lo);
}
void mflo() {
    write_register(EXE.rd, lo);
}
void mtlo() {
    write_hilo(hi, EXE.rs);
}
void mult() {
    // hilo = rs * rt
    int64_t t1 = (int64_t)EXE.rs;
    int64_t t2 = (int64_t)EXE.rt;
    int64_t result = t1*t2;
    write_hilo((uint32_t)(result>>32), (uint32_t)result);
}
void multu() {
    // hilo = rs * rt
    uint64_t t1 = (uint64_t)EXE.rs;
    uint64_t t2 = (uint64_t)EXE.rt;
    uint64_t result = t1*t2;
    write_hilo((uint32_t)(result>>32), (uint32_t)result);
}
void div_() {
    // lo = rs/rt, hi = rs%rt
    int32_t t1 = (int32_t)EXE.rs;
    int32_t t2 = (int32_t)EXE.rt;

    if (t2 == 0){
        // divide by zero exception
        return;
    }
    write_hilo(t1%t2, t1/t2);
}
void divu() {
    // lo = rs/rt, hi = rs%rt
    uint32_t t1 = (uint32_t)EXE.rs;
    uint32_t t2 = (uint32_t)EXE.rt;

    if (t2 == 0){
        // divide by zero exception
        return; 
    }
    write_hilo(t1%t2, t1/t2);
}
void add() {
    int32_t t1 = (int32_t)EXE.rs;
    int32_t t2 = (int32_t)EXE.rt;
    int32_t result = t1 + t2;
    if ((t1 > 0 && t2 > 0 && result < 0) ||
        (t1 < 0 && t2 < 0 && result > 0))
    {
        trigger_trap(pc, TRAP_OVERFLOW);
        return;
    }
    write_register(EXE.rd_id, result);
}

void addu() {
    uint32_t result = EXE.rs + EXE.rt;
    if (result < EXE.rs){
        trigger_trap(pc, TRAP_OVERFLOW);
        return;
    }
    write_register(EXE.rd_id, result);
}
void sub() {
    int32_t t1 = (int32_t)EXE.rs;
    int32_t t2 = (int32_t)EXE.rt;
    int32_t result = t1-t2;
    if ((t1 > 0 && t2 < 0 && result < 0) ||
        (t1 < 0 && t2 > 0 && result > 0))
    {
        trigger_trap(pc, TRAP_OVERFLOW);
        return;
    }
    write_register(EXE.rd_id, result);
}
void subu() {
    if (EXE.rs < EXE.rt){
        trigger_trap(pc, TRAP_OVERFLOW);
        return;
    }
    uint32_t result = EXE.rs - EXE.rt;
    write_register(EXE.rd_id, result);
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
    write_register(EXE.rd_id, ((int32_t)EXE.rs < (int32_t)EXE.rt ? 1: 0));
}
void sltu() {
    write_register(EXE.rd_id, (EXE.rs < EXE.rt ? 1: 0));
}

// I
void beq() {
    uint32_t branch_addr = (uint32_t)EXE.immediate_se << 2;
    if (EXE.rs == EXE.rt){
        pc += branch_addr;
    }
}
void bne() {
    uint32_t branch_addr = (uint32_t)EXE.immediate_se << 2;
    if (EXE.rs != EXE.rt){
        pc += branch_addr;
    }
}
void blez() {
    uint32_t branch_addr = (uint32_t)EXE.immediate_se << 2;
    if (EXE.rs <= 0){
        pc += branch_addr;
    }
}
void bgtz() {
    uint32_t branch_addr = (uint32_t)EXE.immediate_se << 2;
    if ((int32_t)EXE.rs > 0){
        pc += branch_addr;
    }
}
void addi() {
    int32_t t1 = (int32_t)EXE.rs;
    int32_t t2 = EXE.immediate_se;
    int32_t result = t1 + t2;

    //printf("%d+%d = %d -> %d\n", t1,t2,result, EXE.rt_id);

    if ((t1 > 0 && t2 > 0 && result < 0) ||
        (t1 < 0 && t2 < 0 && result > 0))
    {
        trigger_trap(pc, TRAP_OVERFLOW);
        return;
    }
    write_register(EXE.rt_id, result);
}
void addiu() {
    uint32_t result = EXE.rs + (uint32_t)EXE.immediate_se;
    if (result < EXE.rs){
        trigger_trap(pc, TRAP_OVERFLOW);
        return;
    }
    write_register(EXE.rt_id, result);
}
void slti() {
    write_register(EXE.rt_id, ((int32_t)EXE.rs < EXE.immediate_se ? 1: 0));
}
void sltiu() {
    write_register(EXE.rt_id, (EXE.rs < (uint32_t)EXE.immediate_se ? 1: 0));
}
void andi() {
   write_register(EXE.rt_id, EXE.rs & EXE.immediate_ze ); 
}
void ori() {
    write_register(EXE.rt_id, EXE.rs | EXE.immediate_ze ); 
}
void lui() {
    write_register(EXE.rt_id, EXE.rs | ((uint32_t)EXE.immediate_se)>>16 ); 
}

// J
void j() {
    uint32_t addr = EXE.address << 2;
    pc = addr;
}
void jal() {
    write_register(ra, pc);
    uint32_t addr = EXE.address << 2;
    pc = addr;
}

// utils
void write_register(uint8_t reg_to_write, uint32_t out)
{
    MEM.noop = false;
    MEM.write_reg = true;
    MEM.register_to_write = reg_to_write;
    MEM.alu_out = out;
}

void write_hilo(uint32_t new_hi, uint32_t new_lo)
{
    MEM.noop = false;
    MEM.write_hilo = true;
    MEM.hi = new_hi;
    MEM.lo = new_lo;
}

void prepare_memory_read(){
    MEM.noop = false;
    MEM.read_mem = true;
    MEM.alu_out = EXE.rs + EXE.immediate_se;
}
void prepare_memory_write()
{
    MEM.noop = false;
    MEM.write_mem = true;
    MEM.reg_out = EXE.rt;
    MEM.alu_out = EXE.rs + EXE.immediate_se;
}
