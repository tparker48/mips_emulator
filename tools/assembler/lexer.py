from sly import Lexer

from tools.assembler.isa import INSTRUCTIONS, PSEUDOINSTRUCTIONS

class MIPSLexer(Lexer):
    # String containing ignored characters between tokens
    ignore = ' \t'

    labels = set()

    # Set of token names.   This is always required
    tokens = {
        REGISTER,
        ID,
        MNEMONIC,
        PSEUDO_MNEMONIC,
        TEXT,
        DATA,
        WORD,
        HALF,
        BYTE,
        ASCII,
        ASCIIZ,
        SPACE,
        ALIGN,
        INTEGER ,
        #DECIMAL,
        LPAREN, 
        RPAREN,
        COMMA,
        LABEL_DEFINE,
        STRING,
        CHAR,
        NEWLINE,
        HEX,
        BINARY
    }

    # Regular expression rules for tokens
    REGISTER = r'\$[a-zA-Z_]*[a-zA-Z0-9_]*'
    LABEL_DEFINE = r'[a-zA-Z_][a-zA-Z0-9_]*\:'
    ID      = r'[a-zA-Z_][a-zA-Z0-9_]*'

    TEXT = '.text'
    DATA = '.data'
    WORD = '.word'
    HALF = '.half'
    BYTE = '.byte'
    ASCIIZ = '.asciiz'
    ASCII = '.ascii'
    SPACE = '.space'
    ALIGN = '.align'

    # DECIMAL = r'\d+\.\d+'
    HEX = r'(0x)(a-fA-F|\d)+'
    BINARY = r'(0b)(1|0)+'
    INTEGER  = r'\d+'
    LPAREN = r'\('
    RPAREN = r'\)'
    COMMA = r'\,'
    STRING = r'\"(\\.|[^"\\\n])*\"'
    CHAR = r"'(\\.|[^'\\\n])'"
    NEWLINE = r'\n+'
    COMMENT = r'\#[^\n]*'

    @_(r'[a-zA-Z_][a-zA-Z0-9_]*\:')
    def LABEL_DEFINE(self, t):
        label = t.value.strip(':')
        assert label not in self.labels
        self.labels.add(label)
        return t
    
    @_(r'[a-zA-Z_][a-zA-Z0-9_]*')
    def ID(self, t):
        if t.value in INSTRUCTIONS:
            t.type = 'MNEMONIC'
        elif t.value in PSEUDOINSTRUCTIONS:
            t.type = 'PSEUDO_MNEMONIC'
        return t
    
    @_(r'\#[^\n]*')
    def COMMENT(self, t):
        return None

    # Define a rule so we can track line numbers
    # start at -1 to account for our preprocessing adding '.text\n'
    @_(r'\n+')
    def NEWLINE(self, t):
        self.lineno += len(t.value)
        return t