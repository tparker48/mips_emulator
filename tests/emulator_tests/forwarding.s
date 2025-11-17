addi $t0, $t0, 1
addi $t1, $t1, 5
add $a0, $t1, $t0

li $v0, 1
syscall

li $v0, 11
li $a0, 10
syscall

la $a1, my_byte
lb $t0, 0($a1)
addi $t1, $zero, 5
add $a0, $t1, $t0

li $v0, 1
syscall

li $v0, 11
li $a0, 10
syscall

lb $t0, 0($a1)
addi $t0, $zero, 5
add $a0, $zero, $t0

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