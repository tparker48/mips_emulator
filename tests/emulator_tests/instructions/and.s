li $t0, 0xFFFFFFFF
li $t1, 0x0000000
jal and_print
nop

li $t0, 0x00000000
li $t1, 0xFFFFFFFF
jal and_print
nop

li $t0, 0xF0F0F0F0
li $t1, 0x0F0F0F0F
jal and_print
nop

li $t0, 0x0F0F0F0F
li $t1, 0xF0F0F0F0
jal and_print
nop

li $t0, 0xAAAAAAAA
li $t1, 0x55555555
jal and_print
nop

li $t0, 0xFFFFFFFF
li $t1, 0x55555555
jal and_print
nop

li $t0, 0xFFFFFFFF
li $t1, 0xAAAAAAAA
jal and_print
nop

li $t0, 0xFFFFFFFF
li $t1, 0xFFFFFFFF
jal and_print
nop

j exit
nop


and_print:
li $v0, 1
and $a0, $t0, $t1
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