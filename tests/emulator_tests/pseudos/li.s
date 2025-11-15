li $a0, 15
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

li $a0, -15
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

li $a0, 0x0000FFFF
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

li $a0, 0xFFFF0000
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

li $a0, 0
li $v0, 10
syscall