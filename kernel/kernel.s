.data
KERNEL_STACK_TOP:
    .word  0x7FFFFFFC

syscall_table:
    .word 0                # 0 (unused)
    .word print_int        # 1
    .word print_float      # 2
    .word print_double     # 3
    .word print_string     # 4
    .word read_int         # 5
    .word read_float       # 6
    .word read_double      # 7
    .word read_string      # 8
    .word sbrk             # 9
    .word exit             # 10
    .word print_character  # 11
    .word read_character   # 12
    .word open             # 13
    .word read_file        # 14
    .word write_file       # 15
    .word close            # 16
    .word exit2            # 17

.text
.globl _start
.globl exception_vector

# Kernel entry point
_start:
    la $sp, KERNEL_STACK_TOP
    la $t0, 0x00400000
    jr $t0
    nop

# Exception / Interrupt Vector
.align 7
exception_vector:
    mfc0 $t0, $13 # Cause
    mfc0 $t1, $14 # EPC
    srl  $t2, $t0, 2 # Extract exception code

    li $t3, 8 # syscall code
    beq $t2, $t3, handle_syscall
    nop

    # anything else...
    eret

handle_syscall:
    sll   $t1, $v0, 2
    la    $t2, syscall_table
    addu  $t2, $t2, $t1
    lw    $t3, 0($t2)
    jr    $t3
    nop

# reserved opcode 48 recognized by emulator as emulated syscall
# lower bits 5-0 represent syscall code
print_int:
    .word 0xC0000001
    eret
print_float:
    .word 0xC0000002
    eret
print_double:
    .word 0xC0000003
    eret
print_string:
    .word 0xC0000004
    eret
read_int:
    .word 0xC0000005
    eret
read_float:
    .word 0xC0000006
    eret
read_double:
    .word 0xC0000007
    eret
read_string:
    .word 0xC0000008
    eret
sbrk:
    .word 0xC0000009
    eret
exit:
    .word 0xC000000A
    eret
print_character:
    .word 0xC000000B
    eret
read_character:
    .word 0xC000000C
    eret
open:
    .word 0xC000000D
    eret
read_file:
    .word 0xC000000E
    eret
write_file:
    .word 0xC000000F
    eret
close:
    .word 0xC0000010
    eret
exit2:
    .word 0xC0000011
    eret