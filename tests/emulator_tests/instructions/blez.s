j test1
nop

cond1:
li $a0, 1
li $v0, 1
syscall
j test2
nop

cond2:
li $a0, 2
li $v0, 1
syscall
j test3
nop

cond3:
li $a0, 3
li $v0, 1
syscall
j test4
nop

test1:
li $t0, 0
blez $t0, cond1
nop

test2:
li $t0, 1
blez $t0, cond2
nop
li $t0, 0x7fffffff
blez $t0, cond2
nop

test3:
li $t0, -1
blez $t0, cond3
nop

test4:
j exit
nop

exit:
li $v0, 11
li $a0, 10
syscall

li $v0, 10
li $a0, 0
syscall