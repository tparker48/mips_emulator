li $t0, 0
li $t1, 0
jal sltu_print
nop

li $t0, 500
li $t1, 0
jal sltu_print
nop

li $t0, 0
li $t1, 500
jal sltu_print
nop

li $t0, 0xFFFFFFFF
li $t1, 0xFFFFFFFF
jal sltu_print
nop

li $t0, 0xFFFFFFFF
li $t1, 0x7FFFFFFF
jal sltu_print
nop

li $t0, 0x7FFFFFFF
li $t1, 0xFFFFFFFF
jal sltu_print
nop

li $t0, -30
li $t1, -100
jal sltu_print
nop

li $t0, -100
li $t1, -30
jal sltu_print
nop

exit:
li $v0, 10
li $a0, 0
syscall

sltu_print:
sltu $a0, $t0, $t1
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall
jr $ra
nop