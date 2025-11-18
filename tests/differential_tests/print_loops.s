.data
header:         .asciiz "MIPS Giant Number Test\n\n"

sec1_label:     .asciiz "Section 1: Counting up from 0 to 19\n"
sec2_label:     .asciiz "\nSection 2: Counting down from 20 to 0\n"
sec3_label:     .asciiz "\nSection 3: Negative to Positive (-10 to 10)\n"
sec4_label:     .asciiz "\nSection 4: Table of i, i^2, i^3 for i = 0..10\n"
sec_done:       .asciiz "\nAll sections complete.\n"

comma_space:    .asciiz ", "
space:          .asciiz " "
arrow:          .asciiz " -> "
newline_str:    .asciiz "\n"

.text

main:
    la   $a0, header
    li   $v0, 4
    syscall

    la   $a0, sec1_label
    li   $v0, 4
    syscall

    li   $t0, 0 
    li   $t1, 20

sec1_loop:
    slt  $t2, $t0, $t1
    beq  $t2, $zero, sec1_done
    nop

    move $a0, $t0
    li   $v0, 1
    syscall

    li   $v0, 11
    li   $a0, 10
    syscall

    addi $t0, $t0, 1
    j    sec1_loop
    nop

sec1_done:
    la   $a0, sec2_label
    li   $v0, 4
    syscall

    li   $t0, 20 

sec2_loop:
    blt $t0, $zero, sec2_done

    move $a0, $t0
    li   $v0, 1
    syscall

    li   $v0, 11
    li   $a0, 10
    syscall

    addi $t0, $t0, -1
    j    sec2_loop
    nop

sec2_done:
    la   $a0, sec3_label
    li   $v0, 4
    syscall

    li   $t0, -10
    li   $t1, 11 

sec3_loop:
    beq  $t0, $t1, sec3_done
    nop

    move $a0, $t0
    li   $v0, 1
    syscall

    li   $v0, 11
    li   $a0, 10
    syscall

    addi $t0, $t0, 1
    j    sec3_loop
    nop

sec3_done:
    la   $a0, sec4_label
    li   $v0, 4
    syscall

    li   $s0, 0 
    li   $s1, 11

sec4_loop:
    beq  $s0, $s1, sec4_done
    nop

    move $t0, $s0

    move $a0, $t0
    jal  print_int
    nop

    la   $a0, comma_space
    jal  print_string
    nop


    mult  $t0, $t0
    mflo $t1
    move $a0, $t1
    jal  print_int
    nop

    la   $a0, comma_space
    jal  print_string
    nop

    mult  $t1, $t0
    mflo $t1
    move $a0, $t2
    jal  print_int
    nop

    jal  print_newline
    nop

    addi $s0, $s0, 1
    j    sec4_loop
    nop

sec4_done:
    la   $a0, sec_done
    li   $v0, 4
    syscall

    li   $v0, 10       
    syscall



print_string:
    li   $v0, 4
    syscall
    jr   $ra
    nop


print_int:
    li   $v0, 1
    syscall
    jr   $ra
    nop

print_char:
    li   $v0, 11
    syscall
    jr   $ra
    nop

print_newline:
    li   $v0, 11
    li   $a0, 10  
    syscall
    jr   $ra
    nop
