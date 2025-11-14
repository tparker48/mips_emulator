dummyLabel:
move $a1, $a0
neg $a1, $a0
not $a1, $a0
clear $t0
la $t0, dummyLabel
li $t0, 0x11114444
b dummyLabel
blt $t0, $t1, dummyLabel
bgt $t0, $t1, dummyLabel
ble $t0, $t1, dummyLabel
bge $t0, $t1, dummyLabel
beqz $t0, dummyLabel
bnez $t0, dummyLabel
push $t0
pop $t0
nop
