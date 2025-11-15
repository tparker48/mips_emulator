li $t0, 0xFFFFFFFF
andi $a0, $t0, 0x000
jal print_a0
nop

li $t0, 0x00000000
andi $a0, $t0, 0xFFFF
jal print_a0
nop

li $t0, 0xF0F0F0F0
andi $a0, $t0, 0x0F0F
jal print_a0
nop

li $t0, 0x0F0F0F0F
andi $a0, $t0, 0xF0F0
jal print_a0
nop

li $t0, 0xAAAAAAAA
andi $a0, $t0, 0x5555
jal print_a0
nop

li $t0, 0xFFFFFFFF
andi $a0, $t0, 0x5555
jal print_a0
nop

li $t0, 0xFFFFFFFF
andi $a0, $t0, 0xAAAA
jal print_a0
nop

li $t0, 0xFFFFFFFF
andi $a0, $t0, 0xFFFF
jal print_a0
nop

j exit
nop


print_a0:
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