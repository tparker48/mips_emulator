li $t0, 0xFFFFFFFF
li $t1, 0xFFFFFFFF
jal norprint
nop

li $t0, 0
li $t1, 0xFFFFFFFF
jal norprint
nop

li $t0, 0xFFFFFFFF
li $t1, 0
jal norprint
nop

li $t0, 0xAAAAAAAA
li $t1, 0x55555555
jal norprint
nop

li $t0, 0
li $t1, 0
jal norprint
nop

li $t0, 1
li $t1, 0
jal norprint
nop

li $t0, 0xFFFFFFFE
li $t1, 0x00000000
jal norprint
nop

j exit
nop

norprint:
nor $a0, $t0, $t1
li $v0, 1
syscall

li $a0, 10
li $v0, 11
syscall

jr $ra
nop

exit:
li $v0, 10
li $a0, 0
syscall