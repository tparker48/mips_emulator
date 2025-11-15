#include "mips_os.h"
#include "mips_instructions.h"
#include "mips_registers.h"
#include "mips_memory.h"
#include "mips_pipeline.h"

void handle_syscall()
{
    switch (registers[v0])
    {
    case SYSCALL_PRINT_INT:
        printf("%d", (int32_t)registers[a0]);
        break;
    case SYSCALL_PRINT_FLOAT:
        printf("%f", (float)fregisters[f12]);
        break;
    case SYSCALL_PRINT_DOUBLE:
        double d;
        uint32_t hi = fregisters[f12]; // $f12
        uint32_t lo = fregisters[f13]; // $f13
        uint64_t combined = ((uint64_t)hi << 32) | lo;
        memcpy(&d, &combined, sizeof(double));
        printf("%f", d);
        break;
    case SYSCALL_PRINT_STRING:
        char* charptr = (char*)access_mem_bytes(registers[a0],1);
        printf("%s", charptr);
        break;
    case SYSCALL_READ_INT:
        break;
    case SYSCALL_READ_FLOAT:
        break;
    case SYSCALL_READ_DOUBLE:
        break;
    case SYSCALL_READ_STRING:
        break;
    case SYSCALL_SBRK:
        break;
    case SYSCALL_EXIT:
        uint32_t code = registers[a0]; 
        trigger_exit(code);
        break;
    case SYSCALL_PRINT_CHARACTER:
        char c = (char)registers[a0];
        printf("%c", c);
        break;
    case SYSCALL_READ_CHARACTER:
        break;
    case SYSCALL_OPEN_FILE:
        break;
    case SYSCALL_READ_FILE:
        break;
    case SYSCALL_WRITE_FILE:
        break;
    case SYSCALL_CLOSE_FILE:
        break;
    case SYSCALL_EXIT2:
        break;
    }
}