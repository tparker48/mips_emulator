li $t1, 10
li $t2, 3
div $t1, $t2
jal print_hilo
nop

li $t1, 3
li $t2, 3
div $t1, $t2
jal print_hilo
nop

li $t1, 3
li $t2, 0
div $t1, $t2
jal print_hilo
nop

li $t1, 3000
li $t2, 2
div $t1, $t2
jal print_hilo
nop

li $t1, -1
li $t2, 1
div $t1, $t2
jal print_hilo
nop

li $t1, -1
li $t2, 3000
div $t1, $t2
jal print_hilo
nop

li $t1, 5
li $t2, 3000
div $t1, $t2
jal print_hilo
nop

j exit



print_hilo:
li $v0, 1
mfhi $a0
syscall

li $v0, 11
li $a0, 10
syscall

li $v0, 1
mflo $a0
syscall

li $v0, 11
li $a0, 10
syscall

jr $ra
nop

exit:
li $v0, 10
li $a0, 0
syscall