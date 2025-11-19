#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define TEXT_START 0x00400000
#define TEXT_SIZE 2048

#define STATIC_DATA_START 0x10000000
#define STATIC_DATA_SIZE 4096

#define HEAP_START 0x10008000
#define HEAP_SIZE 2048

#define STACK_START 0x7FFFFFFC
#define STACK_SIZE 2048
#define STACK_BOTTOM (STACK_START - STACK_SIZE + 1)

#define KERNEL_START 0x80000000
#define KERNEL_SIZE 2048

extern uint8_t text[];
extern uint8_t static_data[];
extern uint8_t stack[];
extern uint8_t heap[];
extern uint8_t kernel[];

extern uint32_t kernel_start_addr;
extern uint32_t kernel_exception_vector_addr;

uint8_t read_mem_byte(uint32_t addr);
uint16_t read_mem_halfword(uint32_t addr);
uint32_t read_mem_word(uint32_t addr);

void write_mem_byte(uint32_t addr, uint8_t byte);
void write_mem_halfword(uint32_t addr, uint16_t v);
void write_mem_word(uint32_t addr, uint32_t v);

uint8_t* access_mem_bytes(uint32_t address, int len);

void sb();
void sh();
void sw();
void lb();
void lh();
void lw();
void lbu();
void lhu();