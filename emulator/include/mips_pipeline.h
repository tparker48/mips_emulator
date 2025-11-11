#pragma once

#include <stdint.h>
#include <stdbool.h>


// For Forwarding Registers
struct Forwarding
{
    bool flag;
    uint8_t register_id;
    uint32_t register_val;
};


// Pipeline Structs
struct InstructionFetch
{
    struct Forwarding forwarding;
};

struct InstructionDecode
{
    bool noop;
    uint32_t instruction_word;
    struct Forwarding forwarding;
};

struct Execution
{
    bool noop;
    uint8_t op_code;
    uint8_t shamt, funct;
    uint32_t rs, rt, rd;
    uint8_t rs_id, rt_id, rd_id;
    uint32_t immediate_ze;
    int32_t immediate_se;
    uint32_t address;
};

struct MemoryAccess
{
    bool noop;
    uint8_t op_code;
    uint32_t alu_out;
    uint32_t hi, lo;
    uint32_t reg_out;
    uint8_t register_to_write;
    bool write_reg;
    bool write_mem;
    bool read_mem;
    bool write_hilo;
};

struct WriteBack
{
    bool noop;
    uint32_t alu_out;
    uint32_t mem_out;
    uint32_t hi, lo;
    uint8_t register_to_write;
    bool write_from_mem;
    bool write_from_alu;
    bool write_hilo;
};

extern struct InstructionFetch IF;
extern struct InstructionDecode ID;
extern struct Execution EXE;
extern struct MemoryAccess MEM;
extern struct WriteBack WB;

// API
void run_cycle();
bool exited();
void trigger_exit(int code);
int get_exit_code();

// OS
void trigger_trap(int pc_store, int cause_code);
void execute_trap();
bool pipeline_empty();

// Pipeline steps
void instruction_fetch();
void instruction_decode();
void execute_instruction();
void memory_access();
void write_back();

// Hazard Dection & Control
void exe_forward();
void mem_forward();
bool needs_bubble(uint32_t instruction);
