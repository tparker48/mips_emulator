addi $a0, $zero, 5
j exit
add $a0, $a0, $a0 # Executed (delay slot)
add $a0, $a0, $a0 # Not executed

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
