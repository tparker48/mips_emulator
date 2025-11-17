# 1x1 = 1
li $t0, 1
li $t1, 1
jal mprint
nop


# 1x0 = 0
li $t0, 1
li $t1, 0
jal mprint
nop

# 0x1 = 0
li $t0, 0
li $t1, 1
jal mprint
nop

# 30x4 = 120
li $t0, 30
li $t1, 4
jal mprint
nop

# 4 x 0x7FFF0000 = 0x0000007F 0xFF000000
# 0x 00000001 FFFFFFFC
li $t0, 4
li $t1, 0x7FFFFFFF
jal mprint
nop

# -1 * 1 = - 0xFFFFFFFF 0xFFFFFFFF
li $t0, -1
li $t1, 1
jal mprint
nop

j exit
nop


mprint:
mult $t0, $t1

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
nop
