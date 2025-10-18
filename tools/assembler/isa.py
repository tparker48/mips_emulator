from typing import Final, Callable
from enum import Enum

class InstructionType(Enum):
    R_TYPE = 0
    I_TYPE = 1
    J_TYPE = 2
    PSEUDO = 3

class InstructionData:
    op_code: int = None
    funct_code:int = None
    instruction_type: InstructionType = None
    def __init__(self, instruction_type: InstructionType, op_code: int, funct_code: int = None):
        self.op_code = op_code
        self.instruction_type = instruction_type
        self.funct_code = funct_code

INSTRUCTIONS : Final[dict[str, int]] = {
    'sll':   InstructionData(InstructionType.R_TYPE, 0x00, 0x00),
    'srl':   InstructionData(InstructionType.R_TYPE, 0x00, 0x02),
    'sra':   InstructionData(InstructionType.R_TYPE, 0x00, 0x03),
    'jr':    InstructionData(InstructionType.R_TYPE, 0x00, 0x08),
    'jalr':  InstructionData(InstructionType.R_TYPE, 0x00, 0x09),
    'mfhi':  InstructionData(InstructionType.R_TYPE, 0x00, 0x10),
    'mthi':  InstructionData(InstructionType.R_TYPE, 0x00, 0x11),
    'mflo':  InstructionData(InstructionType.R_TYPE, 0x00, 0x12),
    'mtlo':  InstructionData(InstructionType.R_TYPE, 0x00, 0x13),
    'mult':  InstructionData(InstructionType.R_TYPE, 0x00, 0x18),
    'multu': InstructionData(InstructionType.R_TYPE, 0x00, 0x19),
    'div':   InstructionData(InstructionType.R_TYPE, 0x00, 0x1A),
    'divu':  InstructionData(InstructionType.R_TYPE, 0x00, 0x1B),
    'add':   InstructionData(InstructionType.R_TYPE, 0x00, 0x20),
    'addu':  InstructionData(InstructionType.R_TYPE, 0x00, 0x21),
    'sub':   InstructionData(InstructionType.R_TYPE, 0x00, 0x22),
    'subu':  InstructionData(InstructionType.R_TYPE, 0x00, 0x23),
    'and':   InstructionData(InstructionType.R_TYPE, 0x00, 0x24),
    'or':    InstructionData(InstructionType.R_TYPE, 0x00, 0x25),
    'xor':   InstructionData(InstructionType.R_TYPE, 0x00, 0x26),
    'nor':   InstructionData(InstructionType.R_TYPE, 0x00, 0x27),
    'slt':   InstructionData(InstructionType.R_TYPE, 0x00, 0x2A),
    'sltu':  InstructionData(InstructionType.R_TYPE, 0x00, 0x2B),

    'beq':   InstructionData(InstructionType.I_TYPE, 0x04),
    'bne':   InstructionData(InstructionType.I_TYPE, 0x05),
    'blez':  InstructionData(InstructionType.I_TYPE, 0x06),
    'bgtz':  InstructionData(InstructionType.I_TYPE, 0x07),
    'addi':  InstructionData(InstructionType.I_TYPE, 0x08),
    'addiu': InstructionData(InstructionType.I_TYPE, 0x09),
    'slti':  InstructionData(InstructionType.I_TYPE, 0x0A),
    'sltiu': InstructionData(InstructionType.I_TYPE, 0x0B),
    'andi':  InstructionData(InstructionType.I_TYPE, 0x0C),
    'ori':   InstructionData(InstructionType.I_TYPE, 0x0D),
    'lui':   InstructionData(InstructionType.I_TYPE, 0x0F),
    'lb':    InstructionData(InstructionType.I_TYPE, 0x20), 
    'lh':    InstructionData(InstructionType.I_TYPE, 0x21), 
    'lw':    InstructionData(InstructionType.I_TYPE, 0x23), 
    'lbu':   InstructionData(InstructionType.I_TYPE, 0x24),
    'lhu':   InstructionData(InstructionType.I_TYPE, 0x25),
    'sb':    InstructionData(InstructionType.I_TYPE, 0x28),
    'sh':    InstructionData(InstructionType.I_TYPE, 0x29),
    'sw':    InstructionData(InstructionType.I_TYPE, 0x2B),

    'j':     InstructionData(InstructionType.J_TYPE, 0x02),
    'jal':   InstructionData(InstructionType.J_TYPE, 0x03),
}

PSEUDOINSTRUCTIONS = {
    'move',
    'neg',
    'not',
    'clear',

    'la',
    'li',

    'b',
    'beqz',
    'bnez',

    'push',
    'pop',

    'nop'
}

FUNCTS : Final[dict[str, int]] = {
    'sll':   0x00,
    'srl':   0x02,
    'sra':   0x03,
    'jr':    0x08,
    'jalr':  0x09,
    'mfhi':  0x10,
    'mthi':  0x11,
    'mflo':  0x12,
    'mtlo':  0x13,
    'mult':  0x18,
    'multu': 0x19,
    'div':   0x1A,
    'divu':  0x1B,
    'add':   0x20,
    'addu':  0x21,
    'sub':   0x22,
    'subu':  0x23,
    'and':   0x24,
    'or':    0x25,
    'xor':   0x26,
    'nor':   0x27,
    'slt':   0x2A,
    'sltu':  0x2B,
}
