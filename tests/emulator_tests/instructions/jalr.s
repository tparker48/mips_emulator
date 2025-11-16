la $t0, print_123
la $t1, exit

# PRINT 0
addi $a0, $zero, 0
addi $v0, $zero, 1
jalr $t0
syscall # delay slot, still executed

# PRINT 456
addi $a0, $zero, 456
addi $v0, $zero, 1
syscall

# PRINT \n
addi $a0, $zero, 10
addi $v0, $zero, 11
syscall

jalr $t1
nop

print_123:
addi $a0, $zero, 123
addi $v0, $zero, 1
syscall
# return
jalr $ra


exit:
addi $a0, $zero, 0
addi $v0, $zero, 10
syscall