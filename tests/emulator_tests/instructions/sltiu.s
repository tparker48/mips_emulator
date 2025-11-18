li $t0, 0
sltiu $a0, $t0, 0
jal slt_print
nop

li $t0, 500
sltiu $a0, $t0, 0
jal slt_print
nop

li $t0, 0
sltiu $a0, $t0, 500
jal slt_print
nop

li $t0, 0xFFFFFFFF
sltiu $a0, $t0, 0xFFFF
jal slt_print
nop

li $t0, 0xFFFFFFFF
sltiu $a0, $t0, 0x7FFF
jal slt_print
nop

li $t0, 0x7FFFFFFF
sltiu $a0, $t0, 0xFFFF
jal slt_print
nop

li $t0, -30
sltiu $a0, $t0, -100
jal slt_print
nop

li $t0, -100
sltiu $a0, $t0, -30
jal slt_print
nop

exit:
li $v0, 10
li $a0, 0
syscall

slt_print:
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall
jr $ra
nop