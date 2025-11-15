li $t1, 10
addi $t0, $t1, 5
jal print_t0
nop

addi $t0, $t0, -5
jal print_t0
nop

li $a3, -500
addi $t0, $a3, -1000
jal print_t0
nop

addi $t0, $t0, 1000
jal print_t0
nop

li $t0, 0xFFFFFFFF
addi $t0, $t0, -1
jal print_t0
nop

jal exit
nop

print_t0:
move $a0, $t0
addi $v0, $zero, 1
syscall

addi $a0, $zero, 10
addi $v0, $zero, 11
syscall
jr $ra

exit:
addi $a0, $zero, 0
addi $v0, $zero, 10
syscall 
