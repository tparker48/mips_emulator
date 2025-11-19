li $t0, 0
not $a0, $t0
jal print_a0
nop

li $t0, 0xFFFFFFFF
not $a0, $t0
jal print_a0
nop

li $t0, 0xAAAAAAAA
li $t1, 0x55555555
not $t0, $t0
subu $a0, $t0, $t1
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
