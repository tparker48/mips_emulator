li $t0, 0
move $a0, $t0
jal print_a0
nop

li $t0, 0xFFFFFFFF
move $a0, $t0
jal print_a0
nop

li $t0, 0x7FFFFFFF
move $a0, $t0
jal print_a0
nop

li $t0, 0x80000000
move $a0, $t0
jal print_a0
nop

exit:
li $v0, 10
li $a0, 0
syscall

print_a0:
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall
jr $ra
nop
