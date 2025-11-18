li $t0, 0
li $t1, 0
jal slt_print
nop

li $t0, 500
li $t1, 0
jal slt_print
nop

li $t0, 0
li $t1, 500
jal slt_print
nop

li $t0, 0xFFFFFFFF
li $t1, 0xFFFFFFFF
jal slt_print
nop

li $t0, 0xFFFFFFFF
li $t1, 0x7FFFFFFF
jal slt_print
nop

li $t0, 0x7FFFFFFF
li $t1, 0xFFFFFFFF
jal slt_print
nop

li $t0, -30
li $t1, -100
jal slt_print
nop

li $t0, -100
li $t1, -30
jal slt_print
nop

exit:
li $v0, 10
li $a0, 0
syscall

slt_print:
slt $a0, $t0, $t1
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall
jr $ra
nop