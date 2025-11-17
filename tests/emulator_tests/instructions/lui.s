# lui 0 = 0
lui $a0, 0
jal call_lui 
nop

# lui 1 = 65536
lui $a0, 1
jal call_lui 
nop

# lui 2 = 131072
lui $a0, 2
jal call_lui 
nop

# lui 0xFFFF = -65536
lui $a0, 0xFFFF
jal call_lui 
nop

# lui 0x8000 = -2147483648
lui $a0, 0x8000
jal call_lui 
nop

jal exit
nop

call_lui:
li $v0, 1
syscall
li $v0, 11
li $a0, 10
syscall
jr $ra
nop

exit:
li $v0, 10
li $a0, 0
syscall