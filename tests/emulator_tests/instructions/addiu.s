li $t1, 10
addiu $t0, $t1, 5
jal print_t0
nop

li $a0, 1
addiu $t0, $a0, -5
jal print_t0
nop

li $a3, -5
addiu $t0, $a3, 3
jal print_t0
nop

addiu $t0, $t0, 1
jal print_t0
nop

li $t0, 0xFFFFFFFE
addiu $t0, $t0, 1
jal print_t0
nop

li $t0, 0
addiu $t0, $t0, 0x0001FFFF
jal print_t0
nop

li $t0, 0
addiu $t0, $t0, 0x11110000
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
addiu $t0, $t0, 1
jal print_t0
