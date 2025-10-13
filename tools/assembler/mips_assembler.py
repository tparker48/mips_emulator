import argparse
import sys

from tools.assembler.instruction import Instruction
from tools.assembler.isa import OPCODES, FUNCTS, INSTRUCTION_PARSERS

def parse_line(file_name:str, line_number: int, line: str, verbose: bool = False) -> bytearray:
    if verbose: print(f'{file_name}({line_number}):')

    line = line.split('#')[0] # everything before first '#'
    line = line.strip() # remove whitespace

    if line == "":
        if verbose: print('Line is empty, skipping.')
        return None
    
    mnemonic = line.split()[0]
    args = ' '.join(line.split()[1:])

    if (mnemonic not in OPCODES):
        print(f"({file_name}:{line_number}) ERROR: Unrecognized instruction mnemonic: {mnemonic}")
        exit(1)

    parse_result: Instruction = INSTRUCTION_PARSERS[mnemonic](args)
    parse_result.op_code = OPCODES[mnemonic]
    parse_result.funct = FUNCTS[mnemonic] if mnemonic in FUNCTS else None

    if verbose: 
        print(f'{line}')
        print(f'{parse_result}')
        print(f'0x{parse_result.bytes().hex().upper()}')
        print('--------------------------------------------')

    return parse_result.bytes()

def assemble_binary(input_file: str, output_file: str, verbose: bool = False, dump_bytes: bool = False):
    with open(input_file, 'r') as f:
        lines = f.readlines()

    with open(output_file, 'wb') as f:
        for line_no,line in enumerate(lines):
            instr_bytes = parse_line(input_file, line_no, line, verbose)
            if (instr_bytes):
                f.write(instr_bytes)
                if (dump_bytes):
                    print(instr_bytes.hex().upper())


def main():
    parser = argparse.ArgumentParser(
        description="MIPS Assembler: Convert .s assembly files to binary."
    )

    parser.add_argument(
        "input_file",
        type=str,
        help="Path to the input MIPS assembly file (.s)"
    )

    parser.add_argument(
        "-o", "--output",
        type=str,
        default=None,
        help="Path to the output binary file (.bin). Defaults to input filename with .bin extension"
    )

    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Print detailed assembly steps"
    )

    args = parser.parse_args()

    # Determine output file
    output_file = args.output or args.input_file.rsplit('.', 1)[0] + ".bin"

    try:
        assemble_binary(
            input_file=args.input_file,
            output_file=output_file,
            verbose=args.verbose,
        )
    except Exception as e:
        print(f"Assembly failed: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == '__main__':
    main()