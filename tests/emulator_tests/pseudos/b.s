li $t0, 0
li $t1, 5

p1:
b p2
nop

p2:
b p3
nop

p3:
addi $t0, $t0, 1
b p4
nop

p4:
move $a0, $t0
li $v0, 1
syscall
li $v0, 11
li $a0, 10
syscall
b p5
nop

p5:
bge $t0, $t1, exit
nop
b p1
nop

exit:
li $v0, 10
li $a0, 0
syscall