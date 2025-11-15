# overflow causes exception, can't test both cases in a single test

li $a0, 0x80000000
li $a1, 1
jal sub_a # Overflow
nop

sub_a:
sub $a0, $a0, $a1
li $v0, 1
syscall

li $a0, 10
li $v0, 11
syscall

jr $ra
nop

