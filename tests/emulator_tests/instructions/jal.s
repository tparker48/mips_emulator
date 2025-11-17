# PRINT 0
addi $a0, $zero, 0
addi $v0, $zero, 1
jal print_123
syscall # delay slot, still executed

# PRINT 456
addi $a0, $zero, 456
addi $v0, $zero, 1
syscall

# PRINT \n
addi $a0, $zero, 10
addi $v0, $zero, 11
syscall

jal empty
nop

jal exit
nop

print_123:
addi $a0, $zero, 123
addi $v0, $zero, 1
syscall
# return
jr $ra
nop

empty:
jr $ra
nop

exit:
addi $a0, $zero, 0
addi $v0, $zero, 10
syscall