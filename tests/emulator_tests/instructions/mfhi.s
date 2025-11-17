li $t0, 0
jal print_mfhi
nop

li $t0, -1
jal print_mfhi
nop

li $t0, 20
jal print_mfhi
nop

li $t0, 500
jal print_mfhi
nop

li $t0, 0xFFFFFFFE
jal print_mfhi
nop

j exit
nop

print_mfhi:
mthi $t0
mfhi $a0
li $v0, 1
syscall
li $v0, 11
li $a0, 10
syscall
jr $ra
nop

exit:
li $v0, 10
li $a0, 0
syscall
