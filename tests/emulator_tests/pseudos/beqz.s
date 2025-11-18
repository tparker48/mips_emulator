test1:
li $t0, 0
beqz $t0, branch
nop
j exit
nop

test2:
li $t0, 1
beqz $t0, branch
nop

test3:
li $t0, -1
beqz $t0, branch
nop

exit:
li $a0, 0
li $v0, 10
syscall

branch:
li $a0, 1
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall
b test2
nop
