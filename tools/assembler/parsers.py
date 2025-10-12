from tools.assembler.instruction import Instruction

def comma_separated_args(args:str) -> list[str]:
    return [w.strip() for w in args.strip().split(',')]

def parse_rd_rs_rt(args: str) -> Instruction:
    rd,rs,rt = comma_separated_args(args)
    return Instruction(rd=rd, rs=rs, rt=rt)

def parse_rd_rt_shamt(args: str) -> Instruction:
    rd,rt,shamt = comma_separated_args(args)
    return Instruction(rd=rd, rt=rt, shamt=shamt)
    
def parse_rd_rt_rs(args: str) -> Instruction:
    rd,rt,rs = comma_separated_args(args)
    return Instruction(rd=rd, rt=rt, rs=rs)

def parse_rs_rt(args: str) -> Instruction:
    rs,rt = comma_separated_args(args)
    return Instruction(rs=rs, rt=rt)

def parse_rd(args: str) -> Instruction:
    rd = comma_separated_args(args)[0]
    return Instruction(rd=rd)

def parse_rs(args: str) -> Instruction:
    rs = comma_separated_args(args)[0]
    return Instruction(rs=rs)

def parse_rd_rs(args: str) -> Instruction:
    rd, rs = comma_separated_args(args)
    return Instruction(rd=rd, rs=rs)

def parse_rt_rs_imm(args: str) -> Instruction:
    rt, rs, imm = comma_separated_args(args)
    return Instruction(rt=rt, rs=rs, immediate=imm)

def parse_rt_offset_rs(args: str) -> Instruction:
    rt, offset_rs = [w.strip() for w in args.strip().split(',')]
    offset,rs = [w.strip() for w in offset_rs.split('(')]
    rs = rs.split(')')[0].strip()
    return Instruction(rt=rt, immediate=offset, rs=rs)

def parse_rt_imm(args: str) -> Instruction:
    rt, imm = comma_separated_args(args)
    return Instruction(rt=rt, immediate=imm)

def parse_rs_rt_label(args: str) -> Instruction:
    # TODO
    return None

def parse_rs_label(args: str) -> Instruction:
    # TODO
    return None

def parse_label(args: str) -> Instruction:
    # TODO
    return None

# special cases
def parse_jalr(args: str) -> Instruction:
    args = comma_separated_args(args)
    if len(args) == 1:
        # implicit rd=$ra
        rs = args[0]
        rd = '$ra'
    else:
        rd, rs = args
    return Instruction(rs=rs, rd = rd)