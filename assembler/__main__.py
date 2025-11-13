import argparse
from assembler.assemble import assemble
    
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