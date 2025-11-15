li $t1, 10
li $t2, 5
addu $t0, $t1, $t2
jal print_t0
nop

li $a0, 1
li $t2, -5
addu $t0, $a0, $t2
jal print_t0
nop

li $a3, -5
li $t2, 3
addu $t0, $a3, $t2
jal print_t0
nop

li $t2, 1
addu $t0, $t0, $t2
jal print_t0
nop

li $t0, 0xFFFFFFFE
addu $t0, $t0, $t2
jal print_t0
nop

jal overflow
nop

print_t0:
move $a0, $t0
addi $v0, $zero, 1
syscall

addi $a0, $zero, 10
addi $v0, $zero, 11
syscall
jr $ra
nop

overflow:
li $t0, 0x7FFFFFFF
li $t2, 1
addu $t0, $t0, $t2
jal print_t0
