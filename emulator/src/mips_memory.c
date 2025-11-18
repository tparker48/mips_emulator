#include "mips_memory.h"
#include "mips_pipeline.h"

uint8_t text[sizeof(uint8_t) * TEXT_SIZE];
uint8_t static_data[sizeof(uint8_t) * STATIC_DATA_SIZE];
uint8_t stack[sizeof(uint8_t) * STACK_SIZE];
uint8_t heap[sizeof(uint8_t) * HEAP_SIZE];

uint8_t kernel[sizeof(uint8_t) * KERNEL_SIZE];
uint32_t kernel_start_addr;
uint32_t kernel_exception_vector_addr;

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
    else if (address >= KERNEL_START && address_end < KERNEL_START + KERNEL_SIZE)
    {
        return &kernel[address-KERNEL_START];
    }
    else
    {
        fprintf(stderr, "Segmentation fault at 0x%08x\n", address);
        exit(1);
    }
}

uint8_t read_mem_byte(uint32_t addr) {
    return *access_mem_bytes(addr, 1);
}

uint16_t read_mem_halfword(uint32_t addr) {
    return (read_mem_byte(addr) << 8) |
           (read_mem_byte(addr + 1));
}

uint32_t read_mem_word(uint32_t addr) {
    return (read_mem_byte(addr)     << 24) |
           (read_mem_byte(addr + 1) << 16) |
           (read_mem_byte(addr + 2) << 8)  |
            read_mem_byte(addr + 3);
}

void write_mem_byte(uint32_t addr, uint8_t byte) {
    *access_mem_bytes(addr, 1) = byte;
}

void write_mem_halfword(uint32_t addr, uint16_t v) {
    write_mem_byte(addr,     (v >> 8) & 0xFF);
    write_mem_byte(addr + 1,  v       & 0xFF);
}

void write_mem_word(uint32_t addr, uint32_t v) {
    write_mem_byte(addr,     (v >> 24) & 0xFF);
    write_mem_byte(addr + 1, (v >> 16) & 0xFF);
    write_mem_byte(addr + 2, (v >> 8)  & 0xFF);
    write_mem_byte(addr + 3,  v        & 0xFF);
}

uint8_t *access_mem_byte(uint32_t address){
    return access_mem_bytes(address, 1);
}

void sb()
{
    write_mem_byte(MEM.alu_out, MEM.reg_out);
}
void sh()
{
    write_mem_halfword(MEM.alu_out, MEM.reg_out);
}
void sw()
{
    write_mem_word(MEM.alu_out, MEM.reg_out);
}
void lb()
{
    WB.mem_out = (int8_t)read_mem_byte(MEM.alu_out);
}
void lh()
{
    WB.mem_out = (int16_t)read_mem_halfword(MEM.alu_out);
}
void lw()
{
    WB.mem_out = read_mem_word(MEM.alu_out);
}
void lbu()
{
    WB.mem_out = read_mem_byte(MEM.alu_out);
}
void lhu()
{
    WB.mem_out = read_mem_halfword(MEM.alu_out);
}