#include "mips_em_syscalls.h"
#include "mips_instructions.h"
#include "mips_registers.h"
#include "mips_memory.h"

void handle_em_syscall(uint32_t em_syscall)
{
    switch (em_syscall)
    {
    case EM_SYSCALL_PRINT_INT:
        printf("%d", registers[a0]);
        break;
    case EM_SYSCALL_PRINT_FLOAT:
        printf("%f", fregisters[f12]);
        break;
    case EM_SYSCALL_PRINT_DOUBLE:
        double d;
        uint32_t hi = fregisters[f12]; // $f12
        uint32_t lo = fregisters[f13]; // $f13
        uint64_t combined = ((uint64_t)hi << 32) | lo;
        memcpy(&d, &combined, sizeof(double));
        printf("%f", d);
        break;
    case EM_SYSCALL_PRINT_STRING:
        char* charptr = (char*)access_mem_bytes(registers[a0],1);
        printf("%s", charptr);
        break;
    case EM_SYSCALL_READ_INT:
        break;
    case EM_SYSCALL_READ_FLOAT:
        break;
    case EM_SYSCALL_READ_DOUBLE:
        break;
    case EM_SYSCALL_READ_STRING:
        break;
    case EM_SYSCALL_SBRK:
        break;
    case EM_SYSCALL_EXIT:
        uint32_t code = registers[a0]; 
        trigger_exit(code);
        break;
    case EM_SYSCALL_PRINT_CHARACTER:
        char c = (char)registers[a0];
        printf("%c", c);
        break;
    case EM_SYSCALL_READ_CHARACTER:
        break;
    case EM_SYSCALL_OPEN:
        break;
    case EM_SYSCALL_READ_FILE:
        break;
    case EM_SYSCALL_WRITE_FILE:
        break;
    case EM_SYSCALL_CLOSE:
        break;
    case EM_SYSCALL_EXIT2:
        break;
    }
}