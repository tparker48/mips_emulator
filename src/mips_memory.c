#include "mips_memory.h"
#include "mips_pipeline.h"

uint8_t text[sizeof(uint8_t) * 2048];
uint8_t static_data[sizeof(uint8_t) * 4096];
uint8_t stack[sizeof(uint8_t) * STACK_SIZE];
uint8_t heap[sizeof(uint8_t) * 2048];

uint8_t *access_mem_bytes(uint32_t address, int len)
{
    uint32_t address_end = address + len - 1;
    if (address_end < address)
    {
        fprintf(stderr, "Segmentation fault at 0x%08x + %d\n", address, len);
        exit(1);
    }

    if (address >= TEXT_START && address_end < TEXT_START + TEXT_SIZE)
    {
        return &text[address - TEXT_START];
    }
    else if (address >= STATIC_DATA_START && address_end < STATIC_DATA_START + STATIC_DATA_SIZE)
    {
        return &static_data[address - STATIC_DATA_START];
    }
    else if (address >= HEAP_START && address_end < HEAP_START + HEAP_SIZE)
    {
        return &heap[address - HEAP_START];
    }
    else if (address >= STACK_BOTTOM && address_end <= STACK_START)
    {
        // stack grows downward
        return &stack[STACK_START - address];
    }
    else
    {
        fprintf(stderr, "Segmentation fault at 0x%08x\n", address);
        exit(1);
    }
}

uint8_t *access_mem_byte(uint32_t address){
    return access_mem_bytes(address, 1);
}
uint16_t *access_mem_halfword(uint32_t address){
    return (uint16_t *)access_mem_bytes(address, 2);
}
uint32_t *access_mem_word(uint32_t address){
    return (uint32_t*)access_mem_bytes(address, 4);
}

void sb()
{
    *access_mem_byte(MEM.alu_out) = (uint8_t)MEM.reg_out;
}
void sh()
{
    *access_mem_halfword(MEM.alu_out) = (uint16_t)MEM.reg_out;
}
void sw()
{
    *access_mem_word(MEM.alu_out) = (uint32_t)MEM.reg_out;
}
void lb()
{
    WB.mem_out = (uint32_t)(int32_t)(int8_t)*access_mem_byte(MEM.alu_out);
}
void lh()
{
    WB.mem_out = (uint32_t)(int32_t)(int16_t)*access_mem_halfword(MEM.alu_out);
}
void lw()
{
    WB.mem_out = *access_mem_word(MEM.alu_out);
}
void lbu()
{
    WB.mem_out = (uint32_t)*access_mem_byte(MEM.alu_out);
}
void lhu()
{
    WB.mem_out = (uint32_t)*access_mem_halfword(MEM.alu_out);
}