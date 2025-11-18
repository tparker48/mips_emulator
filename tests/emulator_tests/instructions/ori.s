li $t0, 0xFFFFFFFF
ori $a0, $t0, 0xFFFF
jal print_a0
nop

li $t0, 0
ori $a0, $t0, 0xFFFF
jal print_a0
nop

li $t0, 0xFFFF
ori $a0, $t0, 0
jal print_a0
nop

li $t0, 0xAAAAAAAA
ori $a0, $t0, 0x5555
jal print_a0
nop

li $t0, 0
ori $a0, $t0, 0
jal print_a0
nop

li $t0, 1
ori $a0, $t0, 0
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
