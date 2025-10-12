#include "mips_pipeline.h"
#include "mips_alu.h"
#include "mips_memory.h"
#include "mips_instructions.h"

struct InstructionDecode ID;
struct Execution EXE;
struct MemoryAccess MEM;
struct WriteBack WB;

bool exit_flag = false;

void run_cycle()
{
    write_back();
    memory_access();
    execute_instruction();
    instruction_decode();
    instruction_fetch();
}

bool should_exit()
{
    return exit_flag && WB.noop && MEM.noop && EXE.noop && ID.noop;
}

void instruction_fetch()
{
    if (exit_flag)
    {
        ID.noop = true;
        return;
    }

    if (pc >= pc_cap)
    {
        printf("Program Counter Overrun.\n");
        return;
    }

    ID.instruction_word = *(uint32_t *)(&text[pc]);
    pc += 4;
}

void instruction_decode()
{
    EXE.noop = ID.noop;
    if (ID.noop)
    {
        return;
    }
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
}

void execute_instruction()
{
    MEM.noop = EXE.noop;
    MEM.op_code = EXE.op_code;

    if (EXE.noop)
    {
        return;
    }

    if (is_r_type(EXE.op_code))
    {
        execute_r();
    }
    else if (is_j_type(EXE.op_code))
    {
        execute_j();
    }
    else
    {
        execute_i();
    }
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
