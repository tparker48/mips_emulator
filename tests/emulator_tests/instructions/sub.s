li $a0, 10
li $a1, 10
jal sub_a # 0
nop

li $a0, -10
li $a1, 11
jal sub_a # -21
nop

li $a0, 15
li $a1, -2
jal sub_a # 17
nop

li $a0, 0x4
li $a1, 0x2
jal sub_a # 2
nop

li $a0, 0b110110
li $a1, 0b000110
jal sub_a # 48
nop

li $a0, 0x7FFFFFFF
li $a1, -1
jal sub_a # Overflow -2147483648
nop

jal exit
nop

sub_a:
sub $a0, $a0, $a1
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