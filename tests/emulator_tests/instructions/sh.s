la $t5, my_word

jal load_and_print_bytes
nop

li $t0, 0xFFFF
sh $t0, 0($t5)
jal load_and_print_bytes
nop

li $t0, 0xFFFF
sh $t0, 2($t5)
jal load_and_print_bytes
nop


li $t0, 1
sh $t0, 0($t5)
li $t0, 2
sh $t0, 2($t5)
jal load_and_print_bytes
nop

li $t0, -1
sh $t0, 0($t5)
li $t0, -2
sh $t0, 2($t5)
jal load_and_print_bytes
nop

exit:
li $v0, 10
li $a0, 0
syscall

load_and_print_bytes:
lhu $a0, 0($t5)
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

lhu $a0, 2($t5)
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
my_word: .word 0xFFFFFFFF