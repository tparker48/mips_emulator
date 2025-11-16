.text
la $t0, test_byte

li $t1, 0x55
sw $t1, 0($t0)
lbu $a0, 0($t0)
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

li $t1, 0xF00
sw $t1, 0($t0)
lbu $a0, 0($t0)
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

li $t1, 0xFFFFFFFF
sb $t1, 0($t0)
lbu $a0, 0($t0)
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

exit:
li $a0, 0
li $v0, 10
syscall

.data
test_byte: .byte 0xAA