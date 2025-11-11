# A very ugly file to test out lexing/parsing

add $t1, $t0, $a0
addi $t1, $a0, 0x100
lb $a0, 8($t0)
# comment
la $a1, my_label # comment
.word my_label, 1, 0x500, 0b1010
.space 3 #65536
my_label:
# add # this won't be caught until encoding
.data
my_data_label:
.word 15, 0x500, 0b10001, my_data_label
.space 3
.align 2
.byte 1
.asciiz "test"