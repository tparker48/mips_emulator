li $zero, 10
li $at, 20
li $v1, 30
li $a1, 40
li $a2, 50
li $a3, 60
li $t0, 70
li $t1, 80
li $t2, 90
li $t3, 100
li $t4, 110
li $t5, 120
li $t6, 130
li $t7, 140
li $s0, 150
li $s1, 160
li $s2, 170
li $s3, 180
li $s4, 190
li $s5, 200
li $s6, 210
li $s7, 220
li $t8, 230
li $t9, 240
li $k0, 250
li $k1, 260
li $gp, 270
li $sp, 280
li $fp, 290

move $a0, $zero
jal print_a0
nop

move $a0, $at
jal print_a0
nop

move $a0, $v1
jal print_a0
nop

move $a0, $a1
jal print_a0
nop

move $a0, $a2
jal print_a0
nop

move $a0, $a3
jal print_a0
nop

move $a0, $t0
jal print_a0
nop

move $a0, $t1
jal print_a0
nop

move $a0, $t2
jal print_a0
nop

move $a0, $t3
jal print_a0
nop

move $a0, $t4
jal print_a0
nop

move $a0, $t5
jal print_a0
nop

move $a0, $t6
jal print_a0
nop

move $a0, $t7
jal print_a0
nop

move $a0, $s0
jal print_a0
nop

move $a0, $s1
jal print_a0
nop

move $a0, $s2
jal print_a0
nop

move $a0, $s3
jal print_a0
nop

move $a0, $s4
jal print_a0
nop

move $a0, $s5
jal print_a0
nop

move $a0, $s6
jal print_a0
nop

move $a0, $s7
jal print_a0
nop

move $a0, $t8
jal print_a0
nop

move $a0, $t9
jal print_a0
nop

move $a0, $k0
jal print_a0
nop

move $a0, $k1
jal print_a0
nop

move $a0, $gp
jal print_a0
nop

move $a0, $sp
jal print_a0
nop

move $a0, $fp
jal print_a0
nop


exit:
li $v0, 10
li $a0, 0
syscall

print_a0:
li $v0, 1
syscall
li $v0, 11
li $a0, 10
syscall
jr $ra