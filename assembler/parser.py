from sly import Parser
from assembler.lexer import MIPSLexer
from assembler.isa import INSTRUCTIONS, InstructionData

class MIPSParser(Parser):
    tokens = MIPSLexer.tokens
    
    # Program
    @_('segment')
    def program(self, p):
        return p.segment
    
    @_('program segment')
    def program(self, p):
        segment_name = list(p.segment.keys())[0]
        if segment_name in p.program:
            p.program[segment_name].extend(p.segment[segment_name])
        else:
            p.program[segment_name] = p.segment[segment_name]
        return p.program
    
    # Segment
    @_('segment NEWLINE')
    def segment(self,p):
        return p.segment
    
    @_('TEXT')
    def segment(self,p):
        return {'.text': []}
    
    @_('DATA')
    def segment(self,p):
        return {'.data':[]}
    
    @_('segment expression')
    def segment(self,p):
        if type(p.expression) is list:
            p.segment[list(p.segment.keys())[0]].extend(p.expression)
        else:
            p.segment[list(p.segment.keys())[0]].append(p.expression)
        return p.segment
    
    # Expression
    @_('instruction_call NEWLINE')
    def expression(self, p):
        return p.instruction_call | {'lineno': p.lineno-1}
    
    @_('pseudo_call NEWLINE')
    def expression(self, p):
        return p.pseudo_call | {'lineno': p.lineno-1}
    
    @_('data_decl NEWLINE')
    def expression(self, p):
        return [decl | {'lineno': p.lineno-1} for decl in p.data_decl]

    @_('LABEL_DEFINE')
    def expression(self, p):
        return {'type': 'label_decl', 'label_name': p.LABEL_DEFINE.strip(':')} | {'lineno': p.lineno-1}

    # Data Declarative
    @_('WORD literal_list')
    def data_decl(self, p):
        return [{'type': 'data_decl','data_type':'word', **literal} for literal in p.literal_list]
    
    @_('HALF literal_list')
    def data_decl(self, p):
        return [{'type': 'data_decl','data_type':'half', **literal} for literal in p.literal_list]
    
    @_('BYTE literal_list')
    def data_decl(self, p):
        return [{'type': 'data_decl','data_type':'byte', **literal} for literal in p.literal_list]
    
    @_('SPACE literal')
    def data_decl(self, p):
        return [{
                'type': 'data_decl',
                'data_type': 'space',
                'literal': p.literal
            }]

    @_('ALIGN literal')
    def data_decl(self, p):
        return [{
            'type': 'data_decl',
            'data_type': 'align',
            'literal': p.literal
            }]

    @_('ASCII STRING')
    def data_decl(self, p):
        return [{
            'type': 'data_decl',
            'data_type': 'ascii',
            'literal': {
                'type':'string', 
                'val': [ord(c) for c in p.STRING.strip('"')]
            }}]
    
    @_('ASCIIZ STRING')
    def data_decl(self, p):
        return [{
            'type': 'data_decl',
            'data_type': 'asciiz',
            'literal': {
                'type':'string', 
                'val': [ord(c) for c in p.STRING.strip('"')]+[0]
            }}]

    # Instruction Call
    @_('instruction')
    def instruction_call(self, p):
        return p.instruction
    
    @_('instruction REGISTER')
    def instruction_call(self, p):
        return p.instruction | {
            'r0': p.REGISTER,
        }
    
    @_('instruction label')
    def instruction_call(self, p):
        return p.instruction | {
            'label': p.label,
        }
    
    @_('instruction literal')
    def instruction_call(self, p):
        return p.instruction | {
            'immediate': p.literal,
        }
    
    @_('instruction REGISTER COMMA REGISTER')
    def instruction_call(self, p):
        return p.instruction | {
            'r0': p.REGISTER0,
            'r1': p.REGISTER1
        }

    @_('instruction REGISTER COMMA label')
    def instruction_call(self, p):
        return p.instruction | {
            'r0': p.REGISTER,
            'label': p.label
        }

    @_('instruction REGISTER COMMA literal')
    def instruction_call(self, p):
        return p.instruction | {
            'r0': p.REGISTER,
            'immediate': p.literal
        }

    @_('instruction REGISTER COMMA REGISTER COMMA REGISTER')
    def instruction_call(self, p):
        return p.instruction | {
            'r0': p.REGISTER0,
            'r1': p.REGISTER1,
            'r2': p.REGISTER2
        }

    @_('instruction REGISTER COMMA REGISTER COMMA label')
    def instruction_call(self, p):
        return p.instruction | {
            'r0': p.REGISTER0,
            'r1': p.REGISTER1,
            'label': p.label
        }

    @_('instruction REGISTER COMMA REGISTER COMMA literal')
    def instruction_call(self, p):
        return p.instruction | {
            'r0': p.REGISTER0,
            'r1': p.REGISTER1,
            'immediate': p.literal
        }

    @_('instruction REGISTER COMMA literal LPAREN REGISTER RPAREN')
    def instruction_call(self, p):
        return p.instruction | {
            'r0': p.REGISTER0,
            'r1': p.REGISTER1,
            'immediate': p.literal
        }

    @_('instruction REGISTER COMMA label LPAREN REGISTER RPAREN')
    def instruction_call(self, p):
        return p.instruction | {
            'r0': p.REGISTER0,
            'r1': p.REGISTER1,
            'label': p.label
        }
        
    # Pseudo-Instruction Call
    @_('pseudo REGISTER COMMA REGISTER')
    def pseudo_call(self, p):
        return p.pseudo | {
            'r0': p.REGISTER0,
            'r1': p.REGISTER1
        }
    @_('pseudo REGISTER COMMA literal')
    def pseudo_call(self, p):
        return p.pseudo | {
            'r0': p.REGISTER,
            'imm': p.literal
        }
    @_('pseudo REGISTER COMMA label')
    def pseudo_call(self, p):
        return p.pseudo | {
            'r0': p.REGISTER,
            'label': p.label
        }
    @_('pseudo REGISTER COMMA REGISTER COMMA label')
    def pseudo_call(self, p):
        return p.pseudo | {
            'r0': p.REGISTER0,
            'r1': p.REGISTER1,
            'label': p.label
        }

    # Instruction
    @_('MNEMONIC')
    def instruction(self, p):
        instruction_data : InstructionData = INSTRUCTIONS[p.MNEMONIC]
        inst = {
            'type': 'instruction',
            'name': p.MNEMONIC,
            'op':instruction_data.op_code,
            'funct': instruction_data.funct_code,
            'instruction_type': instruction_data.instruction_type
            }
        return inst

    # Pseudo-Instruction
    @_('PSEUDO_MNEMONIC')
    def pseudo(self, p):
        return { 
            'type': 'pseudo',
            'name': p.PSEUDO_MNEMONIC,
        }
    
    # Literal list
    @_('literal_list COMMA literal_or_label')
    def literal_list(self, p):
        return p.literal_list + [p.literal_or_label]

    @_('literal_or_label')
    def literal_list(self, p):
        return [p.literal_or_label]

    @_('literal')
    def literal_or_label(self, p):
        return {'literal': p.literal}

    @_('label')
    def literal_or_label(self, p):
        return {'label': p.label}
    
    # Literal
    @_('INTEGER')
    def literal(self, p):
        return {'type':'int', 'val': int(p.INTEGER, 10) }
    
    @_('HEX')
    def literal(self, p):
        return {'type':'hex', 'val': int(p.HEX, 16) }
    
    @_('BINARY')
    def literal(self, p):
        return {'type':'binary', 'val': int(p.BINARY, 2) }
    
    @_('CHAR')
    def literal(self, p):
        return {'type':'char', 'val': ord(p.CHAR) }
    
    # Label
    @_('ID')
    def label(self, p):
        return p.ID
