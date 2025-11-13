from assembler.lexer import MIPSLexer
from assembler.parser import MIPSParser
from assembler.assembly_processing import preprocess, postprocess
from assembler.encoder import encode_binary
    

def assemble(input_file: str, output_file: str, verbose: bool = False):
    lexer = MIPSLexer()
    parser = MIPSParser()

    if verbose:
        print(f'Assembling "{input_file}" -> "{output_file}"')

    with open(input_file) as test_input:
        data = preprocess(test_input.read())
        toks = lexer.tokenize(data)
        ir = parser.parse(toks)
        ir = postprocess(ir, lexer.labels)
        bytes = encode_binary(ir, verbose)

        with open(f'{output_file}', 'wb') as out_file:
            out_file.write(bytes)