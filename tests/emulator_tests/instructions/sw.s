la $t5, my_word

li $t0, 0xFFFFFFFF
sw $t0, 0($t5)
jal load_and_print_bytes
nop

li $t0, 0x01020304
sw $t0, 0($t5)
jal load_and_print_bytes
nop

li $t0, 0x10203040
sw $t0, 0($t5)
jal load_and_print_bytes
nop

exit:
li $v0, 10
li $a0, 0
syscall

load_and_print_bytes:
lbu $a0, 0($t5)
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

lbu $a0, 1($t5)
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

lbu $a0, 2($t5)
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

lbu $a0, 3($t5)
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

# reset 0
li $t0, 0
sw $t0, 0($t5)

jr $ra
nop

.data
my_word: .word 0x00000000