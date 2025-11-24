from typing import Final
from enum import Enum

OP_SPECIAL = 0b000000
OP_SPECIAL3 = 0b011111
OP_PCREL = 0b111011
FUNCT_BSHFL = 0b100000

class InstructionData:
    op_code:int = None
    funct_code:int = None
    minor_op_code:int = None
    bshfl_code:int = None
    regimm_code:int = None

    def __init__(self, op_code: int, 
                 funct_code: int = None, 
                 minor_op_code: int = None,
                 bshfl_code: int = None,
                 regimm_code: int = None):
        self.op_code = op_code
        self.funct_code = funct_code
        self.minor_op_code = minor_op_code
        self.bshfl_code = bshfl_code
        self.regimm_code = regimm_code

    def __repr__(self) -> str:
        return f'| op:{self.op_code}, funct:{self.funct_code}, type:{self.instruction_type} |'

INSTRUCTIONS : Final[dict[str, int]] = {
    'add': InstructionData(OP_SPECIAL, funct_code=0b100000),
    'addiu': InstructionData(0b001001),
    'addiupc': InstructionData(OP_PCREL, minor_op_code=0b00),
    'addu': InstructionData(OP_SPECIAL, funct_code=0b100001),
    'align': InstructionData(OP_SPECIAL3, funct_code=FUNCT_BSHFL, bshfl_code=0b010),
    'aluipc': InstructionData(OP_PCREL, minor_op_code=0b11111),
    'and': InstructionData(OP_SPECIAL, funct_code=0b100100),
    'andi': None,
    'aui': None,
    'auipc': InstructionData(OP_PCREL, minor_op_code=0b11110),
    'balc': None,
    'bc': None,
    'beq': None,
    'bgez': None,
    'beqc': None,
    'bnec': None,
    'beqzc': None,
    'bnezc': None,
    'bgtz': None,
    'bitswap': InstructionData(OP_SPECIAL3, funct_code=FUNCT_BSHFL, bshfl_code=0b00000),
    'blez': None,
    'bltz': None,
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
    'lwpc': InstructionData(OP_PCREL, minor_op_code=0b01),
    'mul': None,
    'muh': None,
    'mulu': None,
    'muhu': None,
    'nor': InstructionData(OP_SPECIAL, funct_code=0b100111),
    'or': InstructionData(OP_SPECIAL, funct_code=0b100101),
    'ori': None,
    'pause': None,
    'pref': None,
    'rotr': None,
    'rotrv': None,
    'sb': None,
    'sc': None,
    'sdbbp': None,
    'seb': InstructionData(OP_SPECIAL3, funct_code=FUNCT_BSHFL, bshfl_code=0b10000),
    'seh': InstructionData(OP_SPECIAL3, funct_code=FUNCT_BSHFL, bshfl_code=0b11000),
    'seleqz': None,
    'selnez': None,
    'sh': None,
    'sigrie': None,
    'sll': None,
    'sllv': None,
    'slt': InstructionData(OP_SPECIAL, funct_code=0b101010),
    'slti': None,
    'sltiu': None,
    'sltu': InstructionData(OP_SPECIAL, funct_code=0b101011),
    'sra': None,
    'srav': None,
    'srl': None,
    'srlv': None,
    'sub': InstructionData(OP_SPECIAL, funct_code=0b100010),
    'subu': InstructionData(OP_SPECIAL, funct_code=0b100011),
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
    'wsbh': InstructionData(OP_SPECIAL3, funct_code=FUNCT_BSHFL, bshfl_code=0b00010),
    'xor': InstructionData(OP_SPECIAL, funct_code=0b100110),
    'xori': None,
}

PSEUDOINSTRUCTIONS = {
    'move',
    'neg',
    'not',
    'clear',
    'la',
    'li',
    'b',
    'blt', 
    'bgt', 
    'ble', 
    'bge',
    'beqz',
    'bnez',
    'push',
    'pop',
    'nop'
}

ADDRESS_OFFSETS = {
    '.text': 0x00400000,
    '.data': 0x10000000
}