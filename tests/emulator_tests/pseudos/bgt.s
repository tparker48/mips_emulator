test1:
li $t0, -1
li $t1, 0
bgt $t0,  $t1, branch1
nop

test2:
li $t0, 1
li $t1, 0
bgt $t0, $t1, branch2
nop

test3:
li $t0, 0
li $t1, 0
bgt $t0, $t1, branch3
nop

exit:
li $a0, 0
li $v0, 10
syscall

branch1:
li $a0, 1
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall
b test2
nop

branch2:
li $a0, 2
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall
b test3
nop

branch3:
li $a0, 3
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall
b exit
nop