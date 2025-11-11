from typing import Final, Callable
from enum import Enum
import struct

class Registers(Enum):
    zero = 0
    at = 1
    v0 = 2
    v1 = 3
    a0 = 4
    a1 = 5
    a2 = 6
    a3 = 7
    t0 = 8
    t1 = 9
    t2 = 10
    t3 = 11
    t4 = 12
    t5 = 13
    t6 = 14
    t7 = 15
    s0 = 16
    s1 = 17
    s2 = 18
    s3 = 19
    s4 = 20
    s5 = 21
    s6 = 22
    s7 = 23
    t8 = 24
    t9 = 25
    k0 = 26
    k1 = 27
    gp = 28
    sp = 29
    fp = 30
    ra = 31


     
def build_instruction(ir_data: dict, rd=None, rs=None, rt=None, shamt=0, immediate=0, address=0) -> bytes:
    op_code = int(ir_data['op']) if 'op' in ir_data else 0
    funct = int(ir_data['funct']) if ir_data['funct'] else 0

    def reg_map(reg: str) -> int:
        return Registers[reg.strip('$')].value
    
    rd = reg_map(rd) if rd else 0
    rs = reg_map(rs) if rs else 0
    rt = reg_map(rt) if rt else 0

    if (op_code in [0x00, 0x10]):
        raw = op_code
        raw = (raw << 5) | rs
        raw = (raw << 5) | rt
        raw = (raw << 5) | rd
        raw = (raw << 5) | shamt
        raw = (raw << 6) | funct
        return struct.pack('>I', raw)
    
    elif (op_code in [0x02, 0x03]):
        raw = op_code
        raw = (raw << 26) | address
        return struct.pack('>I', raw)
    
    else:
        raw = op_code
        raw = (raw << 5)  | rs
        raw = (raw << 5)  | rt
        raw = (raw << 16) | immediate & 0xFFFF
        return struct.pack('>I', raw)
    
def get_args(ir_data:dict, expected_args: list[str]) -> list[str]:
    for arg in expected_args:
        if arg not in ir_data:
            raise SyntaxError(f"Line {ir_data['lineno']}: Invalid instruction arguments")
    return [ir_data[arg] for arg in expected_args]


def encode_rd_rs_rt(ir_data: dict) -> bytes:
    rd, rs, rt = get_args(ir_data, ['r0','r1','r2'])
    return build_instruction(ir_data, rd=rd, rs=rs, rt=rt)

def encode_rd_rt_shamt(ir_data: dict) -> bytes:
    rd,rt,shamt = get_args(ir_data, ['r0','r1', 'immediate'])
    shamt = shamt['val']
    return build_instruction(ir_data, rd=rd, rt=rt, shamt=shamt)
    
def encode_rd_rt_rs(ir_data: dict) -> bytes:
    rd,rt,rs = get_args(ir_data, ['r0','r1','r2'])
    return build_instruction(ir_data, rd=rd, rt=rt, rs=rs)

def encode_rs_rt(ir_data: dict) -> bytes:
    rs,rt = get_args(ir_data, ['r0','r1'])
    return build_instruction(ir_data, rs=rs, rt=rt)

def encode_rd(ir_data: dict) -> bytes:
    rd = get_args(ir_data, ['r0'])[0]
    return build_instruction(ir_data, rd=rd)

def encode_rs(ir_data: dict) -> bytes:
    rs = get_args(ir_data, ['r0'])[0]
    return build_instruction(ir_data, rs=rs)

def encode_rd_rs(ir_data: dict) -> bytes:
    rd, rs = get_args(ir_data, ['r0', 'r1'])
    return build_instruction(ir_data, rd=rd, rs=rs)

def encode_rt_rs_imm(ir_data: dict) -> bytes:
    rt, rs, imm = get_args(ir_data, ['r0', 'r1', 'immediate'])
    imm = imm['val']
    return build_instruction(ir_data, rt=rt, rs=rs, immediate=imm)

def encode_rt_offset_rs(ir_data: dict) -> bytes:
    rt, rs, imm = get_args(ir_data, ['r0', 'r1', 'immediate'])
    imm = imm['val']
    return build_instruction(ir_data, rt=rt, immediate=imm, rs=rs)

def encode_rt_imm(ir_data: dict) -> bytes:
    rt, imm = get_args(ir_data, ['r0', 'immediate'])
    imm = imm['val']
    return build_instruction(ir_data, rt=rt, immediate=imm)

def encode_rs_rt_offset(ir_data: dict) -> bytes:
    rs, rt, imm = get_args(ir_data, ['r0', 'r1', 'immediate'])
    addr = imm['val']
    offset = addr - ir_data['addr'] - 4
    offset = offset >> 2
    return build_instruction(ir_data, rs=rs, rt=rt, immediate=offset)

def encode_rs_offset(ir_data: dict) -> bytes:
    rs, imm = get_args(ir_data, ['r0', 'immediate'])
    addr = imm['val']
    offset = addr - ir_data['addr'] - 4
    offset = offset >> 2
    return build_instruction(ir_data, rs=rs, immediate=offset)

def encode_addr(ir_data: dict) -> bytes:
    imm = get_args(ir_data, ['immediate'])[0]
    addr = imm['val'] >> 2
    return build_instruction(ir_data, address=addr)

def encode_jalr(ir_data: dict) -> bytes:
    if 'r0' in ir_data and 'r1' not in ir_data:
        # implicit rd=$ra
        rs = get_args(ir_data, ['r0'])[0]
        rd = '$ra'
    else:
        rd, rs = get_args(ir_data, ['r0','r1'])

    return build_instruction(ir_data, rs=rs, rd=rd)

INSTRUCTION_ENCODERS: Final[dict[str, Callable[[str], bytes]]] = {
    'sll':   encode_rd_rt_shamt,
    'srl':   encode_rd_rt_shamt,
    'sra':   encode_rd_rt_shamt,
    'jr':    encode_rs,
    'jalr':  encode_jalr,
    'mfhi':  encode_rd,
    'mthi':  encode_rs,
    'mflo':  encode_rd,
    'mtlo':  encode_rs,
    'mult':  encode_rs_rt,
    'multu': encode_rs_rt,
    'div':   encode_rs_rt,
    'divu':  encode_rs_rt,
    'add':   encode_rd_rs_rt,
    'addu':  encode_rd_rs_rt,
    'sub':   encode_rd_rs_rt,
    'subu':  encode_rd_rs_rt,
    'and':   encode_rd_rs_rt,
    'or':    encode_rd_rs_rt,
    'xor':   encode_rd_rs_rt,
    'nor':   encode_rd_rs_rt,
    'slt':   encode_rd_rs_rt,
    'sltu':  encode_rd_rs_rt,
    'beq':   encode_rs_rt_offset,
    'bne':   encode_rs_rt_offset,
    'blez':  encode_rs_offset,
    'bgtz':  encode_rs_offset,
    'addi':  encode_rt_rs_imm,
    'addiu': encode_rt_rs_imm,
    'slti':  encode_rt_rs_imm,
    'sltiu': encode_rt_rs_imm,
    'andi':  encode_rt_rs_imm,
    'ori':   encode_rt_rs_imm,
    'lui':   encode_rt_imm,
    'lb':    encode_rt_offset_rs, 
    'lh':    encode_rt_offset_rs, 
    'lw':    encode_rt_offset_rs, 
    'lbu':   encode_rt_offset_rs,
    'lhu':   encode_rt_offset_rs,
    'sb':    encode_rt_offset_rs,
    'sh':    encode_rt_offset_rs,
    'sw':    encode_rt_offset_rs,
    'j':     encode_addr,
    'jal':   encode_addr,
}