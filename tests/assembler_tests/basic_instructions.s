sll $t0, $t0, 1
srl $t1, $t1, 2
sra $t2, $t2, 3
jr $t0
jalr $t1
mfhi $t1
mthi $t1
mflo $t1
mtlo $t1
mult $v1, $a0
multu $v0, $a1
div $v1, $a0
divu $v0, $a1
add $v0, $a1, $a0
addu $v0, $a1, $a0
sub $v0, $a1, $a0
subu $v0, $a1, $a0
and $v0, $a1, $a0
or $v0, $a1, $a0
xor $v0, $a1, $a0
nor $v0, $a1, $a0
slt $v0, $a1, $a0
sltu $v0, $a1, $a0
addi $v0, $a1, 1
addiu $v0, $a1, 2
slti $v0, $a1, 3
sltiu $v0, $a1, 4
andi $v0, $a1, 5
ori $v0, $a1, 6
lui $v0, 100
lb $v0, 0x0($a0)
lh $v0, 0x1($a1)
lw $v0, 0x2($a2)
lbu $v0, 0x3($a3)
lhu $v0, 4($t0)
sb $v0, 5($t1)
sh $v0, 6($t2)
sw $v0, 7($t3)