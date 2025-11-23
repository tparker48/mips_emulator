#include <stdio.h>
#include <limits.h>

#include "mips32r6_instructions.h"
#include "mips_registers.h"
#include "mips_pipeline.h"
#include "mips_memory.h"
#include "mips_os.h"

void add()
{
    int64_t t1 = (int64_t)(int32_t)EXE.rs;
    int64_t t2 = (int64_t)(int32_t)EXE.rt;
    int64_t result = t1 + t2;

    if (result > INT32_MAX || result < INT32_MIN)
    {
        trigger_trap(pc, EXCEPT_OVERFLOW);
        return;
    }
    write_register(EXE.rd_id, (int32_t)result);
}
void addiu()
{
    uint32_t result = (uint32_t)EXE.rs + (uint32_t)EXE.immediate_se;
    write_register(EXE.rt_id, result);
}
void addiupc()
{
    write_register(EXE.rs_id, pc + EXE.immediate_se);
}
void addu()
{
    uint32_t result = EXE.rs + EXE.rt;
    write_register(EXE.rd_id, result);
}
void align()
{
    uint32_t offset = 8 * EXE.bp;
    write_register(EXE.rd_id, (
        (EXE.rt << offset) | (EXE.rs >> (32-offset))
    ));
}
void aluipc()
{
    write_register(EXE.rs_id, ~0x0FFFF & (pc + EXE.immediate_sll16));
}
void and()
{
    write_register(EXE.rd_id, EXE.rs & EXE.rt);
}
void andi()
{
   write_register(EXE.rt_id, EXE.rs & EXE.immediate_ze ); 
}
void aui()
{
    write_register(EXE.rt_id, EXE.rs + EXE.immediate_sll16);
}
void auipc()
{
    write_register(EXE.rs_id, pc + EXE.immediate_sll16);
}
void balc()
{
    write_register(ra, pc);
    unconditional_branch(EXE.offset26);
}
void bc()
{
    unconditional_branch(EXE.offset16 << 2);
}
void beq()
{
    legacy_branch(EXE.rs == EXE.rt, EXE.offset16 << 2);
}
void bgez()
{
    legacy_branch((int32_t)EXE.rs >= 0, EXE.offset16 << 2);
}
void beqc()
{
    compact_branch(EXE.rs == EXE.rt, EXE.offset16 << 2);
}
void bnec()
{
    compact_branch(EXE.rs != EXE.rt, EXE.offset16 << 2);
}
void beqzc()
{
    compact_branch(EXE.rs == 0, EXE.offset21 << 2);
}
void bnezc()
{
    compact_branch(EXE.rs != 0, EXE.offset21 << 2);
}
void bgtz()
{
    legacy_branch((int32_t)EXE.rs > 0, EXE.offset16 << 2);
}
void bitswap()
{
    uint32_t swapped_output = 0;
    for (int i = 0; i < 4; i++){
        uint8_t byte = (EXE.rt >> i*8) & 0xFF;
        byte = ((byte & 0xF0) >> 4) | ((byte & 0x0F) << 4);
        byte = ((byte & 0xCC) >> 2) | ((byte & 0x33) << 2);
        byte = ((byte & 0xAA) >> 1) | ((byte & 0x55) << 1);
        swapped_output |= (byte << i*8);
    }
    write_register(EXE.rd_id, swapped_output);
}
void blez()
{
    legacy_branch((int32_t)EXE.rs <= 0, EXE.offset16 << 2);
}
void bltz()
{
    legacy_branch((int32_t)EXE.rs < 0, EXE.offset16 << 2);
}
void bne()
{
    legacy_branch(EXE.rs != EXE.rt, EXE.offset16 << 2);
}
void bovc()
{
    int64_t t1 = (int64_t)(int32_t)EXE.rs;
    int64_t t2 = (int64_t)(int32_t)EXE.rt;
    int64_t result = t1 + t2;
    compact_branch(result > INT32_MAX || result < INT32_MIN, EXE.offset16 << 2);
}
void bnvc()
{
    int64_t t1 = (int64_t)(int32_t)EXE.rs;
    int64_t t2 = (int64_t)(int32_t)EXE.rt;
    int64_t result = t1 + t2;
    compact_branch(!(result > INT32_MAX || result < INT32_MIN), EXE.offset16 << 2);
}
void break_()
{
    trigger_trap(pc, EXCEPT_BREAKPOINT);
}
void clo()
{
    uint32_t rs = EXE.rs;
    uint32_t leading_ones = 0;
    for (int i = 0; i < 32; i++){
        uint8_t bit = (rs >> (31-i)) & 1;
        if (bit == 1) {
            leading_ones++;
        }
        else {
            break;
        }
    }
    write_register(EXE.rd_id, leading_ones);
}
void clz()
{
    uint32_t rs = EXE.rs;
    uint32_t leading_zeros = 0;
    for (int i = 0; i < 32; i++){
        uint8_t bit = (rs >> (31-i)) & 1;
        if (bit == 0) {
            leading_zeros++;
        }
        else {
            break;
        }
    }
    write_register(EXE.rd_id, leading_zeros);
}
void div_()
{
    int32_t t1 = (int32_t)EXE.rs;
    int32_t t2 = (int32_t)EXE.rt;

    if (t2 == 0)
    {
        // divide by zero exception
        return;
    }

    write_register(EXE.rd_id, t1/t2);
}
void mod()
{
    int32_t t1 = (int32_t)EXE.rs;
    int32_t t2 = (int32_t)EXE.rt;
    write_register(EXE.rd_id, t1%t2);
}
void divu()
{
    // lo = rs/rt, hi = rs%rt
    uint32_t t1 = (uint32_t)EXE.rs;
    uint32_t t2 = (uint32_t)EXE.rt;

    if (t2 == 0){
        // divide by zero exception
        return; 
    }
    write_register(EXE.rd_id, t1/t2);
}
void modu()
{
    // lo = rs/rt, hi = rs%rt
    uint32_t t1 = (uint32_t)EXE.rs;
    uint32_t t2 = (uint32_t)EXE.rt;
    write_register(EXE.rd_id, t1%t2);
}
void ehb()
{
    // noop
}
void ext()
{
    uint8_t size = EXE.msb+1;
    uint8_t pos = EXE.lsb;
    uint32_t mask = 0xFFFFFFFF;
    if (size < 32){
        mask = ~((0xFFFFFFFF) << size);
    }
    write_register(EXE.rd_id, (EXE.rs >> (pos)) & mask);
}
void ins()
{
    uint8_t pos = EXE.lsb;
    uint8_t size = EXE.msb-pos+1;
    uint32_t mask = 0xFFFFFFFF;
    if (size < 32){
        mask = ~((0xFFFFFFFF) << size);
    }
    uint32_t extracted = EXE.rs & mask;
    uint32_t result = (EXE.rt & ~(mask << pos)) | (extracted << pos);
    write_register(EXE.rt_id, result);
}
void j()
{
    uint32_t addr = EXE.address << 2;
    legacy_jump(addr);
}
void jal()
{
    write_register(ra, pc);
    uint32_t addr = EXE.address << 2;
    legacy_jump(addr);
}
void jalr()
{
    write_register(EXE.rd_id, pc);
    legacy_jump(EXE.rs);
}
void jialc()
{
    write_register(ra, pc);
    compact_jump(EXE.immediate_se + EXE.rt);
}
void jic()
{
    compact_jump(EXE.immediate_se + EXE.rt);
}
void lb()
{
    prepare_memory_read(LOAD_BYTE, EXE.rs + EXE.immediate_se, EXE.rt_id);
}
void lbu()
{
    prepare_memory_read(LOAD_BYTE_UNSIGNED, EXE.rs + EXE.immediate_se, EXE.rt_id);
}
void lh()
{
    prepare_memory_read(LOAD_HALF, EXE.rs + EXE.immediate_se, EXE.rt_id);
}
void lhu()
{
    prepare_memory_read(LOAD_HALF_UNSIGNED, EXE.rs + EXE.immediate_se, EXE.rt_id);
}
void ll()
{
    // TODO MEM
    // TODO atomic?
}
void lsa()
{
    write_register(EXE.rd_id, (EXE.rs << (EXE.bp+1)) + EXE.rt);
}
void lw()
{
    prepare_memory_read(LOAD_WORD, EXE.rs + EXE.immediate_se, EXE.rt_id);
}
void lwpc()
{
    prepare_memory_read(LOAD_WORD, EXE.pc + EXE.offset19, EXE.rs_id);
}
void mul()
{
    int64_t t1 = (int64_t)(int32_t)EXE.rs;
    int64_t t2 = (int64_t)(int32_t)EXE.rt;
    int64_t result = t1*t2;
    write_register(EXE.rd_id, (uint32_t)result);
}
void muh()
{
    int64_t t1 = (int64_t)(int32_t)EXE.rs;
    int64_t t2 = (int64_t)(int32_t)EXE.rt;
    int64_t result = t1*t2;
    write_register(EXE.rd_id, (uint32_t)(result>>32));
}
void mulu()
{
    uint64_t t1 = (uint64_t)EXE.rs;
    uint64_t t2 = (uint64_t)EXE.rt;
    uint64_t result = t1*t2;
    write_register(EXE.rd_id, (uint32_t)result);
}
void muhu()
{
    uint64_t t1 = (uint64_t)EXE.rs;
    uint64_t t2 = (uint64_t)EXE.rt;
    uint64_t result = t1*t2;
    write_register(EXE.rd_id, (uint32_t)(result>>32));
}
void nor()
{
    write_register(EXE.rd_id, ~(EXE.rs | EXE.rt));
}
void or()
{
    write_register(EXE.rd_id, EXE.rs | EXE.rt);
}
void ori()
{
    write_register(EXE.rt_id, EXE.rs | EXE.immediate_ze ); 
}
void pause()
{
    // nop
}
void pref()
{
    // nop
}
void rotr()
{
    uint32_t result = (EXE.rt >> EXE.shamt | EXE.rt << (32-EXE.shamt));
    write_register(EXE.rd_id, result);
}
void rotrv()
{
    uint8_t shamt = EXE.rs & 0b11111;
    uint32_t result = (EXE.rt >> shamt | EXE.rt << (32-shamt));
    write_register(EXE.rd_id, result);
}
void sb()
{
    prepare_memory_write(STORE_BYTE, EXE.rs + EXE.immediate_se, EXE.rt);
}
void sc()
{
    // TODO MEM
    // TODO atomic?
}
void sdbbp()
{
    trigger_trap(pc, EXCEPT_BREAKPOINT);
}
void seb()
{
    write_register(EXE.rd_id, (uint32_t)(int32_t)(int8_t)EXE.rt);
}
void seh()
{
    write_register(EXE.rd_id, (uint32_t)(int32_t)(int16_t)EXE.rt);
}
void seleqz()
{
    write_register(EXE.rd_id, EXE.rt != 0 ? 0 : EXE.rs);
}
void selnez()
{
    write_register(EXE.rd_id, EXE.rt == 0 ? 0 : EXE.rs);
}
void sh()
{
    prepare_memory_write(STORE_HALF, EXE.rs + EXE.immediate_se, EXE.rt);
}
void sigrie()
{
    trigger_trap(pc, EXCEPT_RESERVED_INSTRUCTION);
}
void sll() 
{
    write_register(EXE.rd_id, EXE.rt << EXE.shamt);
}
void sllv()
{
    if (EXE.rs < 32){
        write_register(EXE.rd_id, EXE.rt << EXE.rs);
    }
    else{
        write_register(EXE.rd_id, 0);
    }
}
void slt()
{
    write_register(EXE.rd_id, ((int32_t)EXE.rs < (int32_t)EXE.rt ? 1: 0));
}
void slti()
{
    write_register(EXE.rt_id, ((int32_t)EXE.rs < EXE.immediate_se ? 1: 0));
}
void sltiu()
{
    write_register(EXE.rt_id, (EXE.rs < (uint32_t)EXE.immediate_se ? 1: 0));
}
void sltu()
{
    write_register(EXE.rd_id, (EXE.rs < EXE.rt ? 1: 0));
}
void sra()
{
    int32_t val = (int32_t)EXE.rt;
    uint32_t result = ((uint32_t)val) >> EXE.shamt;

    if (val < 0){
        result |= ~((~0u) >> EXE.shamt);
    }

    write_register(EXE.rd_id, result);
}
void srav()
{
    int32_t val = (int32_t)EXE.rt;
    uint8_t shamt = EXE.rs & 0b11111;
    uint32_t result = ((uint32_t)val) >> shamt;

    if (val < 0){
        result |= ~((0xFFFFFFFFu) >> shamt);
    }

    write_register(EXE.rd_id, result);
}
void srl() 
{
    write_register(EXE.rd_id, EXE.rt >> EXE.shamt);
}
void srlv()
{
    uint8_t shamt = EXE.rs & 0b11111;
    write_register(EXE.rd_id, EXE.rt >> shamt);
}
void sub()
{
    int64_t t1 = (int64_t)(int32_t)EXE.rs;
    int64_t t2 = (int64_t)(int32_t)EXE.rt;
    int64_t result = t1 - t2;

    if (result > INT32_MAX || result < INT32_MIN)
    {
        trigger_trap(pc, EXCEPT_OVERFLOW);
        return;
    }
    write_register(EXE.rd_id, (int32_t)result);
}
void subu()
{
    uint32_t result = EXE.rs - EXE.rt;
    write_register(EXE.rd_id, result);
}
void sw()
{
    prepare_memory_write(STORE_WORD, EXE.rs + EXE.immediate_se, EXE.rt);
}
void sync()
{
    // nop
}
void synci()
{
    // nop
}
void syscall()
{
    trigger_trap(pc, EXCEPT_SYSCALL);
    ID.noop = true;
}
void teq()
{
    if (EXE.rs == EXE.rt) {
        trigger_trap(pc, EXCEPT_TRAP);
    }
}
void tge()
{
    if ((int32_t)EXE.rs >= (int32_t)EXE.rt) {
        trigger_trap(pc, EXCEPT_TRAP);
    }
}
void tgeu()
{
    if (EXE.rs >= EXE.rt) {
        trigger_trap(pc, EXCEPT_TRAP);
    }
}
void tlt()
{
    if ((int32_t)EXE.rs < (int32_t)EXE.rt) {
        trigger_trap(pc, EXCEPT_TRAP);
    }
}
void tltu()
{
    if (EXE.rs < EXE.rt) {
        trigger_trap(pc, EXCEPT_TRAP);
    }
}
void tne()
{
    if (EXE.rs != EXE.rt) {
        trigger_trap(pc, EXCEPT_TRAP);
    }
}
void wsbh()
{
    uint32_t result =
        ((EXE.rt & 0x00FF00FF) << 8)  |
        ((EXE.rt & 0xFF00FF00) >> 8);
    write_register(EXE.rd_id, result);
}
void xor()
{
    write_register(EXE.rd_id, EXE.rs ^ EXE.rt);
}
void xori()
{
    write_register(EXE.rt_id, EXE.rs ^ EXE.immediate_ze);
}

// Helpers
void write_register(uint8_t reg_to_write, uint32_t out)
{
    if (reg_to_write == 0){
        return;
    }

    MEM.noop = false;
    MEM.write_reg = true;
    MEM.register_to_write = reg_to_write;
    MEM.alu_out = out;
}

void prepare_memory_read(enum MemoryOperation op, uint32_t addr, uint8_t destination_register)
{
    if (destination_register == 0){
        return;
    }

    MEM.noop = false;
    MEM.operation = op;
    MEM.read_mem = true;
    MEM.alu_out = addr;
    MEM.register_to_write = destination_register;
}
void prepare_memory_write(enum MemoryOperation op, uint32_t addr, uint32_t value)
{
    MEM.noop = false;
    MEM.write_mem = true;
    MEM.alu_out = addr;
    MEM.reg_out = value;
}

void unconditional_branch(uint32_t addr) {
    pc += addr;
    ID.noop = true;
}

void compact_branch(bool cond, uint32_t addr) {
    if (cond){
        pc += addr;
        ID.noop = true;
    }
    else{
        ID.forbidden_slot = true;
    }
}

void legacy_branch(bool cond, uint32_t addr) {
    if (cond){
        pc += addr;
    }
    ID.delay_slot = true;
}

void legacy_jump(uint32_t addr){
    pc = addr;
    ID.delay_slot = true;
}

void compact_jump(uint32_t addr) {
    pc = addr;
    ID.noop = true;
}