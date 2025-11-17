la $a1, my_byte
lb $t0, 0($a1) # ID -> EXE populated
addi $a0, $t0, 5 # IF
li $v0, 1
syscall

li $v0, 11
li $a0, 10
syscall

li $v0, 10
li $a0, 0
syscall


.data
my_byte: .byte 7