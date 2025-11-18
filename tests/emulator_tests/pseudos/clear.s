li $a0, 0xFFFFFFFF
clear $a0
jal print_a0
nop

li $a0, 0xAAAAAAAA
clear $a0
jal print_a0
nop

li $a0, 0x00000001
clear $a0
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