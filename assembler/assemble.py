from assembler.lexer import MIPSLexer
from assembler.parser import MIPSParser
from assembler.assembly_processing import preprocess, postprocess
from assembler.encoder import encode_binary
    
import traceback

def assemble(input_file: str, output_file: str, verbose: bool = False):
    lexer = MIPSLexer()
    parser = MIPSParser()

    if verbose:
        print(f'Assembling "{input_file}" -> "{output_file}"')

    with open(input_file) as test_input:
        data = preprocess(test_input.read())
        print(1)

        toks = lexer.tokenize(data)
        print(2)

        try:
            ir = parser.parse(toks)
        except Exception as e:
            traceback.print_exc()
            print("Hello")

        print(3)

        ir = postprocess(ir, lexer.labels)
        print(4)

        bytes = encode_binary(ir, verbose)
        print(5)

        with open(f'{output_file}', 'wb') as out_file:
            out_file.write(bytes)