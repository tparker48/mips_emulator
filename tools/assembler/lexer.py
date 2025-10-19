from sly import Lexer

from tools.assembler.isa import INSTRUCTIONS, PSEUDOINSTRUCTIONS

class MIPSLexer(Lexer):
    labels = set()
    ignore = ' \t'
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

    # Token REGEX
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

    # Populate set of labels for symbol table
    @_(r'[a-zA-Z_][a-zA-Z0-9_]*\:')
    def LABEL_DEFINE(self, t):
        label = t.value.strip(':')
        assert label not in self.labels
        self.labels.add(label)
        return t
    
    # Detect instructions vs pseudos
    @_(r'[a-zA-Z_][a-zA-Z0-9_]*')
    def ID(self, t):
        if t.value in INSTRUCTIONS:
            t.type = 'MNEMONIC'
        elif t.value in PSEUDOINSTRUCTIONS:
            t.type = 'PSEUDO_MNEMONIC'
        return t
    
    # Ignore Comments
    @_(r'\#[^\n]*')
    def COMMENT(self, t):
        return None

    # Track line numbers
    @_(r'\n+')
    def NEWLINE(self, t):
        self.lineno += len(t.value)
        return t
    