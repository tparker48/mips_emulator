.text
nop
nop
text_addr: 
nop

la $a0, text_addr
li $v0, 1
syscall
li $v0, 11
li $a0, 10
syscall

la $a0, data_addr
li $v0, 1
syscall
li $v0, 11
li $a0, 10
syscall

li $v0, 10
li $a0, 0
syscall

.data
.word 0
data_addr: .word 0
