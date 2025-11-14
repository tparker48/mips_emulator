# PRINT 5
addi $a0, $zero, 5
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
