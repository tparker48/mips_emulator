li $a0, 10
li $a1, 10
jal subu_a # 0
nop

li $a0, -10
li $a1, 11
jal subu_a # -21
nop

li $a0, 15
li $a1, -2
jal subu_a # 17
nop

li $a0, 0x4
li $a1, 0x2
jal subu_a # 2
nop

li $a0, 0b110110
li $a1, 0b000110
jal subu_a # 48
nop

li $a0, 0x7FFFFFFF
li $a1, -1
jal subu_a # Overflow -2147483648
nop

li $a0, 0x80000000
li $a1, 1
jal subu_a # Overflow 2147483647
nop

jal exit
nop

subu_a:
subu $a0, $a0, $a1
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