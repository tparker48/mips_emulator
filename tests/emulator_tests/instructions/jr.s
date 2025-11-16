la $t0, intermediate
la $t1, intermediate2
la $t2, exit

addi $a0, $zero, 5
jr $t0
add $a0, $a0, $a0 # Executed (delay slot)
add $a0, $a0, $a0 # Not executed

intermediate2:
jr $t2
nop

exit:
addi $a0, $a0, 1
addi $v0, $zero, 1
syscall

# PRINT \n
addi $a0, $zero, 10
addi $v0, $zero, 11
syscall

# exit
addi $a0, $zero, 0
addi $v0, $zero, 10
syscall 

intermediate:
jr $t1
nop
