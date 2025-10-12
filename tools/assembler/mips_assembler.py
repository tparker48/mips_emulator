from tools.assembler.instruction import Instruction
from tools.assembler.isa import OPCODES, FUNCTS, INSTRUCTION_PARSERS

def parse_line(file_name:str, line_number: int, line: str) -> bytearray:
    line = line.split('#')[0] # everything before first '#'
    line = line.strip() # remove whitespace
    if line == "":
        return None
    
    op_code = line.split()[0]
    args = ' '.join(line.split()[1:])

    if (op_code not in OPCODES):
        print(f"({file_name}:{line_number}) Unrecognized op_code: {op_code}")
        exit(1)

    parse_result: Instruction = INSTRUCTION_PARSERS[op_code](args)
    parse_result.op_code = OPCODES[op_code]
    parse_result.funct = FUNCTS[op_code] if op_code in FUNCTS else None

    return parse_result.bytes()

def assemble_binary(asm_file: str, out_file: str, print_raw_words: bool = False):
    with open(asm_file, 'r') as f:
        lines = f.readlines()

    with open(out_file, 'wb') as f:
        for line_no,line in enumerate(lines):
            instr_bytes = parse_line('test.asm', line_no, line)
            if (instr_bytes):
                f.write(instr_bytes)
                if (print_raw_words):
                    print(instr_bytes.hex().upper())
