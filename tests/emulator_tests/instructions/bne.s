j test1
nop

before:
li $a0, 123
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall
j test2
nop

test1:
li $t0, 0x55555555
li $t1, 0xAAAAAAAA
bne $t0, $t1, before
nop

test2:
li $t0, 0xAAAAAAAA
li $t1, 0x55555555
bne $t0, $t1, after
nop

test3:
li $t0, 0x55555555
li $t1, 0x55555555
bne $t0, $t1, unreached 
nop
j exit
nop

unreached:
li $v0, 10
li $a0, 1
syscall

after:
li $a0, 456
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall
j test3
nop

exit:
li $a0, 0
li $v0, 10
syscall