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

def reg_map(ir_data: dict, reg: str) -> int:
    reg = reg.strip('$')
    if reg not in [reg.name for reg in Registers]:
        raise SyntaxError(f'Unrecognized register "{reg}", line {ir_data["lineno"]}')
    else:
        return Registers[reg].value

def insert_bits(string, value, start_bit, length):
    bits = f'{value:0{length}b}'
    start = 31-start_bit
    end = start+length-1
    return string[:start] + bits + string[end:]

def op_code(string, val):
    return insert_bits(string, val, 31, 6)

def rs(string, val):
    return insert_bits(string, val, 25, 5)

def rt(string, val):
    return insert_bits(string, val, 20, 5)

def rd(string, val):
    return insert_bits(string, val, 15, 5)

def sa(string, val):
    return insert_bits(string, val, 10, 5)

def funct(string, val):
    return insert_bits(string, val, 5, 6)

def immediate16(string, val):
    return insert_bits(string, 0xFFFF & (val>>2), 15, 16)

def immediate19(string, val):
    return insert_bits(string, 0x7FFFF & (val>>2), 18, 19)

def immediate26(string, val):
    return insert_bits(string, 0x3FFFFFF & (val>>2), 25, 26)

def code(string, val):
    return insert_bits(string, 0xFFFF & val, 15, 16)

def sa3(string, val):
    return insert_bits(string, val, 10, 3)

def bp(string, val):
    return insert_bits(string, 0b11 & val, 7, 2)

def pcrel_op2(string, val):
    return insert_bits(string, val, 20, 2)

def pcrel_op5(string, val):
    return insert_bits(string, val, 20, 5)


def encode_rd_rs_rt(ir):
    bits = "0"*32
    bits = op_code(ir["op"], bits)
    bits = rd(reg_map(ir,ir["r0"]), bits)
    bits = rs(reg_map(ir,ir["r1"]), bits)
    bits = rt(reg_map(ir,ir["r2"]), bits)
    bits = sa(0, bits)
    bits = funct(ir["funct"], bits)
    return bits

def encode_rt_rs_imm(ir):
    bits = "0"*32
    bits = op_code(ir["op"], bits)
    bits = rs(reg_map(ir,ir["r0"]), bits)
    bits = rt(reg_map(ir,ir["r1"]), bits)
    bits = immediate16(ir["imm"]["val"], bits)
    return bits

def encode_pcrel2(ir):
    bits = "0"*32
    bits = op_code(ir["op"], bits)
    bits = rs(reg_map(ir,ir["r0"]), bits)
    bits = pcrel_op2(ir["minor_op"], bits)
    bits = immediate19(ir["imm"]["val"], bits)
    return bits

def encode_pcrel5(ir):
    bits = "0"*32
    bits = op_code(ir["op"], bits)
    bits = rs(reg_map(ir,ir["r0"]), bits)
    bits = pcrel_op5(ir["minor_op"], bits)
    bits = immediate16(ir["imm"]["val"], bits)
    return bits

def encode_align(ir):
    bits = "0"*32
    bits = op_code(ir["op"], bits)
    bits = rd(reg_map(ir,ir["r0"]), bits)
    bits = rs(reg_map(ir,ir["r1"]), bits)
    bits = rt(reg_map(ir,ir["r2"]), bits)
    bits = sa3(ir["bshfl"], bits)
    bits = bp(ir["imm"]["val"], bits)
    bits = funct(ir["funct"], bits)
    return bits

def encode_special3_rd_rt(ir):
    bits = "0"*32
    bits = op_code(ir["op"], bits)
    bits = rs(0, bits)
    bits = rd(reg_map(ir,ir["r0"]), bits)
    bits = rt(reg_map(ir,ir["r1"]), bits)
    bits = sa(ir["bshfl"], bits)
    bits = funct(ir["funct"], bits)
    return bits

def encode_unconditional_branch(ir):
    bits = "0"*32
    bits = op_code(ir["op"], bits)
    bits = immediate26(ir["imm"]["val"], bits)
    return bits

def encode_rs_rt_offset16(ir):
    bits = "0"*32
    bits = op_code(ir["op"], bits)
    bits = rs(reg_map(ir,ir["r0"]), bits)
    bits = rt(reg_map(ir,ir["r1"]), bits)
    bits = immediate16(ir["imm"]["val"], bits)
    return bits

def encode_regimm_rs_offset(ir):
    bits = "0"*32
    bits = op_code(ir["op"], bits)
    bits = rs(reg_map(ir,ir["r0"]), bits)
    bits = rt(ir["regimm"], bits)
    bits = immediate16(ir["imm"]["val"], bits)
    return bits

def encode_sigrie(ir):
    bits = "0"*32
    bits = op_code(ir["op"], bits)
    bits = rs(0, bits)
    bits = rt(ir["regimm"], bits)
    bits = code(ir["imm"]["val"], bits)
    return bits

INSTRUCTION_ENCODERS: Final[dict[str, Callable[[str], bytes]]] = {
    'add': encode_rd_rs_rt,
    'addiu': encode_rt_rs_imm,
    'addiupc': pcrel_op2,
    'addu': encode_rd_rs_rt,
    'align': encode_align,
    'aluipc': encode_pcrel5,
    'and': encode_rd_rs_rt,
    'andi': encode_rt_rs_imm,
    'aui': encode_rt_rs_imm,
    'auipc': encode_pcrel5,
    'balc': encode_unconditional_branch,
    'bc': encode_unconditional_branch,
    'beq': encode_rs_rt_offset16,
    'bgez': encode_regimm_rs_offset,
    'beqc': None,
    'bnec': None,
    'beqzc': None,
    'bnezc': None,
    'bgtz': None,
    'bitswap': encode_special3_rd_rt,
    'blez': None,
    'bltz': encode_regimm_rs_offset,
    'bne': None,
    'bovc': None,
    'bnvc': None,
    'break': None,
    'clo': None,
    'clz': None,
    'div': None,
    'mod': None,
    'divu': None,
    'modu': None,
    'ehb': None,
    'ext': None,
    'ins': None,
    'j': None,
    'jal': None,
    'jalr': None,
    'jialc': None,
    'jic': None,
    'lb': None,
    'lbu': None,
    'lh': None,
    'lhu': None,
    'll': None,
    'lsa': None,
    'lw': None,
    'lwpc': encode_pcrel2,
    'mul': None,
    'muh': None,
    'mulu': None,
    'muhu': None,
    'nor': encode_rd_rs_rt,
    'or': encode_rd_rs_rt,
    'ori': encode_rt_rs_imm,
    'pause': None,
    'pref': None,
    'rotr': None,
    'rotrv': None,
    'sb': None,
    'sc': None,
    'sdbbp': None,
    'seb': encode_special3_rd_rt,
    'seh': encode_special3_rd_rt,
    'seleqz': None,
    'selnez': None,
    'sh': None,
    'sigrie': encode_sigrie,
    'sll': None,
    'sllv': None,
    'slt': encode_rd_rs_rt,
    'slti': encode_rt_rs_imm,
    'sltiu': encode_rt_rs_imm,
    'sltu': encode_rd_rs_rt,
    'sra': None,
    'srav': None,
    'srl': None,
    'srlv': None,
    'sub': encode_rd_rs_rt,
    'subu': encode_rd_rs_rt,
    'sw': None,
    'sync': None,
    'synci': None,
    'syscall': None,
    'teq': None,
    'tge': None,
    'tgeu': None,
    'tlt': None,
    'tltu': None,
    'tne': None,
    'wsbh': encode_special3_rd_rt,
    'xor': encode_rd_rs_rt,
    'xori': encode_rt_rs_imm,
}