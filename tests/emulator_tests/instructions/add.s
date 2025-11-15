li $t1, 10
li $t2, 5
add $t0, $t1, $t2
jal print_t0
nop

li $t2, -5
add $t0, $t0, $t2
jal print_t0
nop

li $t2, -1000
li $a3, -500
add $t0, $a3, $t2
jal print_t0
nop

li $t2, 1000
add $t0, $t0, $t2
jal print_t0
nop

li $t2, -1
li $t0, 0xFFFFFFFF
add $t0, $t0, $t2
jal print_t0
nop

jal overflow
nop

print_t0:
move $a0, $t0
li $v0, 1
syscall

li $a0, 10
li $v0, 11
syscall
jr $ra
nop

overflow:
li $t2, 1 
li $t0, 0x7FFFFFFF
add $t0, $t0, $t2
