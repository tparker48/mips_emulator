li $t0, 0
jal print_mflo
nop

li $t0, -1
jal print_mflo
nop

li $t0, 20
jal print_mflo
nop

li $t0, 500
jal print_mflo
nop

li $t0, 0xFFFFFFFE
jal print_mflo
nop

j exit
nop

print_mflo:
mtlo $t0
mflo $a0
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
