#include "mips_pipeline.h"
#include "mips_alu.h"
#include "mips_memory.h"
#include "mips_instructions.h"

struct InstructionFetch IF;
struct InstructionDecode ID;
struct Execution EXE;
struct MemoryAccess MEM;
struct WriteBack WB;

bool exit_flag = false;
int exit_code = 0;

bool trap_pending = false;
uint32_t trap_cause = 0;
uint32_t trap_pc = 0;

void run_cycle()
{
    if (trap_pending && pipeline_empty())
    {
        execute_trap();
    }

    write_back();
    memory_access();
    execute_instruction();
    instruction_decode();
    instruction_fetch();
}

bool exited()
{
    return exit_flag;
}

void trigger_exit(int code){
    exit_code = code;
    exit_flag = true;
}

int get_exit_code()
{
    return exit_code;
}

void trigger_trap(int pc_store, int cause_code)
{
    trap_pc = pc_store;
    trap_cause = cause_code;
    trap_pending = true;
}

void execute_trap()
{
    // need more logic here to store state?
    cop0[cause] = trap_cause << 2;
    cop0[epc] = trap_pc;

    // more state stuff later
    pc = kernel_exception_handler_addr;

    trap_pending = false;
    trap_pc = 0;
    trap_cause = 0;
}

bool pipeline_empty()
{
    return (ID.noop && EXE.noop && MEM.noop && WB.noop);
}

void instruction_fetch()
{
    if (IF.forwarding.flag)
    {
        ID.forwarding.flag = true;
        ID.forwarding.register_id = IF.forwarding.register_id;
        ID.forwarding.register_val = IF.forwarding.register_val;
    }
    IF.forwarding.flag = false;

    if (exit_flag || trap_pending)
    {
        ID.noop = true;
        return;
    }

    if (pc >= pc_cap)
    {
        printf("Program Counter Overrun.\n");
        return;
    }

    uint32_t instruction = *access_mem_word(pc);
    if (needs_bubble(instruction))
    {
        ID.noop = true;
        return;
    }

    ID.instruction_word = instruction;
    pc += 4;
}

void instruction_decode()
{
    EXE.noop = ID.noop;

    uint32_t instruction = ID.instruction_word;
    EXE.op_code = (instruction >> 26) & 0b111111;
    EXE.rs_id = (instruction >> 21) & 0b11111;
    EXE.rt_id = (instruction >> 16) & 0b11111;
    EXE.rd_id = (instruction >> 11) & 0b11111;
    EXE.rs = registers[EXE.rs_id];
    EXE.rt = registers[EXE.rt_id];
    EXE.rd = registers[EXE.rd_id];
    EXE.shamt = (instruction >> 6) & 0b11111;
    EXE.funct = (instruction) & 0b111111;
    EXE.immediate_ze = (instruction) & 0xFFFF;
    EXE.immediate_se = (int32_t)(int16_t)((instruction) & 0xFFFF);
    EXE.address = (instruction & 0x03FFFFFF) | (pc & 0xF0000000);

    if (ID.forwarding.flag)
    {
        uint8_t fwd_reg_id = ID.forwarding.register_id;
        uint32_t fwd_reg_val = ID.forwarding.register_val;
        if (fwd_reg_id == EXE.rs_id)
        {
            EXE.rs = fwd_reg_val;
        }
        if (fwd_reg_id == EXE.rt_id)
        {
            EXE.rt = fwd_reg_val;
        }
        if (fwd_reg_id == EXE.rd_id)
        {
            // doesn't hurt if it's just overwritten anyways
            EXE.rd = fwd_reg_val;
        }
    }
    ID.forwarding.flag = false;
}

void execute_instruction()
{
    MEM.noop = EXE.noop;
    MEM.op_code = EXE.op_code;

    if (EXE.noop)
    {
        return;
    }

    if (is_r_instruction(EXE.op_code))
    {
        execute_r();
    }
    else if (is_j_instruction(EXE.op_code))
    {
        execute_j();
    }
    else if (is_i_instruction(EXE.op_code))
    {
        execute_i();
    }
    else if (is_em_syscall(EXE.op_code)){
        execute_em_syscall();
    }

    exe_forward();
}

void memory_access()
{
    WB.noop = MEM.noop;

    if (MEM.noop)
    {
        return;
    }

    WB.write_from_mem = false;
    WB.write_from_alu = false;
    WB.write_hilo = false;

    if (MEM.write_mem)
    {
        switch (MEM.op_code)
        {
        case OP_SB: sb(); break;
        case OP_SH: sh(); break;
        case OP_SW: sw(); break;
        }
    }
    else if (MEM.read_mem)
    {
        WB.write_from_mem = true;
        WB.register_to_write = MEM.register_to_write;
        switch (MEM.op_code)
        {
        case OP_LB:  lb(); break;
        case OP_LH:  lh(); break;
        case OP_LW:  lw(); break;
        case OP_LBU: lbu(); break;
        case OP_LHU: lhu(); break;
        }
    }
    else if (MEM.write_reg)
    {
        WB.write_from_alu = true;
        WB.register_to_write = MEM.register_to_write;
        WB.alu_out = MEM.alu_out;
    }
    else if (MEM.write_hilo)
    {
        WB.write_hilo = true;
        WB.hi = MEM.hi;
        WB.lo = MEM.lo;
    }

    mem_forward();
}

void write_back()
{
    if (WB.write_from_alu)
    {
        registers[WB.register_to_write] = WB.alu_out;
    }
    else if (WB.write_hilo)
    {
        hi = WB.hi;
        lo = WB.lo;
    }
    else if (WB.write_from_mem)
    {
        registers[WB.register_to_write] = WB.mem_out;
    }
}

void exe_forward() {
    // runs at the end of EXE (MEM populated with EXE results)
    if (MEM.write_reg) 
    {
        uint8_t id = MEM.register_to_write;
        uint32_t val = MEM.alu_out;

        IF.forwarding.flag = true;
        IF.forwarding.register_id = id;
        IF.forwarding.register_val = val;

        ID.forwarding.flag = true;
        ID.forwarding.register_id = id;
        ID.forwarding.register_val = val;
    }
}

void mem_forward(){
    // runs at end of MEM (WB populated with MEM results)
    // we only need to forward loads here, everything else is forwarded in EXE
    if (WB.write_from_mem){
        uint8_t id = WB.register_to_write;
        uint32_t val = WB.mem_out;

        IF.forwarding.flag = true;
        IF.forwarding.register_id = id;
        IF.forwarding.register_val = val;

        ID.forwarding.flag = true;
        ID.forwarding.register_id = id;
        ID.forwarding.register_val = val;
    }
}

bool needs_bubble(uint32_t instruction)
{
    // RAW dependency check (Read-After-Write):

    // Example:
    //               |-----------------------forwarded in MEM stage of 0x00
    // 0x00 | lw $dest_reg 0($addr_reg)
    // 0x04 | add $v0 $dest_reg $a1
    //                    |------------------needed in ID stage of 0x04
    // With noop:
    // IF  ID   EXE   MEM  WB
    // ..  0x4  noop  x0   ..
    //      ^-forward--<

    uint8_t op_code = (instruction >> 26) & 0b111111;
    uint8_t funct = (instruction) & 0b111111;
    uint8_t rs_id = (instruction >> 21) & 0b11111;
    uint8_t rt_id = (instruction >> 16) & 0b11111;

    if (reads_mem(EXE.op_code))
    {
        if (is_r_type(op_code) || is_i_type(op_code))
        {
            if (rs_id == EXE.rt_id || rt_id == EXE.rt_id)
            {
                return true;
            }
        }
    }
    else if (writes_hilo(EXE.op_code, EXE.funct) && reads_hilo(op_code, funct))
    {
        return true;
    }

    return false;
}