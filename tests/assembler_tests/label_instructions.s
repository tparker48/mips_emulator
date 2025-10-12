#beq $v0, $v1, Label
#bne $v0, $v1, Label
#blez $v0, Label
#bgtz $v0, Label
#j label
#jal label
#Label:

#expected bin:
#10430005
#14430004
#18400003
#1c400002
#08100006
#0c100006