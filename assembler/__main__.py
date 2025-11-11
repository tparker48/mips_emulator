import argparse
from assembler.lexer import MIPSLexer
from assembler.parser import MIPSParser
from assembler.assembly_processing import preprocess, postprocess
from assembler.encoder import encode_binary
    

def assemble(input_file: str, output_file: str, verbose: bool = False):
    lexer = MIPSLexer()
    parser = MIPSParser()

    with open(input_file) as test_input:
        data = preprocess(test_input.read())
        toks = lexer.tokenize(data)
        ir = parser.parse(toks)
        ir = postprocess(ir, lexer.labels)

        if verbose:
            for seg in ir.keys():
                for line in ir[seg]:
                    print(line)

        bytes = encode_binary(ir)
        with open(f'{output_file}', 'wb') as out_file:
            out_file.write(bytes)

def main():
    argparser = argparse.ArgumentParser(
        description="MIPS Assembler: Convert .s assembly files to binary."
    )

    argparser.add_argument(
        "input_file",
        type=str,
        help="Path to the input MIPS assembly file (.s)"
    )

    argparser.add_argument(
        "-o", "--output",
        type=str,
        default=None,
        help="Path to the output binary file (.bin). Defaults to input filename with .bin extension"
    )

    argparser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Print detailed assembly steps"
    )

    args = argparser.parse_args()

    # Determine output file
    output_file = args.output or args.input_file.rsplit('.', 1)[0] + ".bin"

   
    assemble(
        input_file=args.input_file,
        output_file=output_file,
        verbose=args.verbose,
    )


main()