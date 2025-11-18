li $t0, 1
srl $a0, $t0, 1
jal print_a0
nop

li $t0, 2
srl $a0, $t0, 1
jal print_a0
nop

li $t0, 0x7FFFFFFF
srl $a0, $t0, 31
jal print_a0
nop

li $t0, 0x80000000
srl $a0, $t0, 31
jal print_a0
nop

exit:
li $v0, 10
li $a0, 0
syscall

print_a0:
li $v0, 1
syscall
li $v0, 11
li $a0, 10
syscall
jr $ra
nop
