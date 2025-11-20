#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "mips_alu.h"
#include "mips_memory.h"
#include "mips_instructions.h"
#include "mips_os.h"

void add()
{
    int64_t t1 = (int64_t)(int32_t)EXE.rs;
    int64_t t2 = (int64_t)(int32_t)EXE.rt;
    int64_t result = t1 + t2;

    if (result > INT32_MAX || result < INT32_MIN)
    {
        trigger_trap(pc, TRAP_OVERFLOW);
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
    // TODO
    // no delay slot
    // no forbidden slot
    write_register(ra, pc);
    pc += EXE.offset;
}
void bc()
{
    // TODO
    // no delay slot
    // no forbidden slot
    pc += EXE.offset;
}
void beq()
{
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr();
    if (EXE.rs == EXE.rt){
        pc += branch_addr;
    }
}
void bgez()
{
    uint32_t branch_addr = get_branch_addr();
    if ((int32_t)EXE.rs >= 0){
        pc += branch_addr;
    }
}
void beqc()
{
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr();
    if (EXE.rs == EXE.rt){
        pc += branch_addr;
    }
}
void bnec()
{
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr();
    if (EXE.rs != EXE.rt){
        pc += branch_addr;
    }
}
void beqzc()
{
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr();
    if (EXE.rs == 0){
        pc += branch_addr;
    }
}
void bnezc()
{
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr();
    if (EXE.rs != 0){
        pc += branch_addr;
    }
}
void bgtz()
{
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr(); 
    if ((int32_t)EXE.rs > 0){
        pc += branch_addr;
    }
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
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr();
    if ((int32_t)EXE.rs <= 0){
        pc += branch_addr;
    }
}
void bltz()
{
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr();
    if ((int32_t)EXE.rs < 0){
        pc += branch_addr;
    }
}
void bne()
{
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr();
    if (EXE.rs != EXE.rt){
        pc += branch_addr;
    }
}
void bovc()
{
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr();
    int64_t t1 = (int64_t)(int32_t)EXE.rs;
    int64_t t2 = (int64_t)(int32_t)EXE.rt;
    int64_t result = t1 + t2;

    if (result > INT32_MAX || result < INT32_MIN)
    {
        pc += branch_addr;

    }
}
void bnvc()
{
    // TODO
    // check forbidden slots or someting
    uint32_t branch_addr = get_branch_addr();
    int64_t t1 = (int64_t)(int32_t)EXE.rs;
    int64_t t2 = (int64_t)(int32_t)EXE.rt;
    int64_t result = t1 + t2;

    if ( !(result > INT32_MAX || result < INT32_MIN) )
    {
        pc += branch_addr;
    }
}
void break_()
{
    trigger_trap(pc, TRAP_BREAKPOINT);
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
    // TODO Delay / Forbidden stuff
    uint32_t addr = EXE.address << 2;
    pc = addr;
}
void jal()
{
    // TODO Delay / Forbidden stuff
    write_register(ra, pc);
    uint32_t addr = EXE.address << 2;
    pc = addr;
}
void jalr()
{
    // TODO Delay / Forbidden stuff
    write_register(ra, pc);
    pc = EXE.rs;
}
void jialc()
{
    write_register(ra, pc);
    pc = EXE.immediate_se + EXE.rt;
}
void jic()
{

}
void jr()
{
    pc = EXE.rs;
}
void lb()
{

}
void lbu()
{

}
void lh()
{

}
void lhu()
{

}
void ll()
{

}
void lle()
{

}
void lsa()
{

}
void lw()
{

}
void lwpc()
{

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

}
void pref()
{

}
void rotr()
{

}
void rotrv()
{

}
void sb()
{

}
void sc()
{

}
void sce()
{

}
void sdbbp()
{

}
void seb()
{

}
void seh()
{

}
void seleqz()
{

}
void selnez()
{

}
void sh()
{

}
void sigrie()
{

}
void sll() 
{
    write_register(EXE.rd_id, EXE.rt << EXE.shamt);
}
void sllv()
{

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

}
void srl() 
{
    write_register(EXE.rd_id, EXE.rt >> EXE.shamt);
}
void srlv()
{

}
void sub()
{
    int64_t t1 = (int64_t)(int32_t)EXE.rs;
    int64_t t2 = (int64_t)(int32_t)EXE.rt;
    int64_t result = t1 - t2;

    if (result > INT32_MAX || result < INT32_MIN)
    {
        trigger_trap(pc, TRAP_OVERFLOW);
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

}
void sync()
{

}
void synci()
{

}
void syscall()
{
    trigger_trap(pc, 8);
    ID.noop = true;
}
void teq()
{

}
void tge()
{

}
void tgeu()
{

}
void tlt()
{

}
void tltu()
{

}
void tne()
{

}
void wsbh()
{

}
void xor()
{
    write_register(EXE.rd_id, EXE.rs ^ EXE.rt);
}
void xori()
{

}