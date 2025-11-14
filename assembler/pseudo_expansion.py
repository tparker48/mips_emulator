from assembler.isa import INSTRUCTIONS

def expand_la(ir: dict) -> list[dict]:
    if 'label' not in ir:
        print(f'Line {ir["lineno"]}: "la" must be used with a label!')
        return [ir]
    label_upper = '%U|'+ir['label']
    label_lower = '%L|'+ir['label']
    lui = {
        **ir,
        'name': 'lui',
        'type': 'instruction',
        'op': INSTRUCTIONS['lui'].op_code,
        'funct': INSTRUCTIONS['lui'].funct_code,
        'label': label_upper
    }
    ori = {
        **ir,
        'r1': ir['r0'],
        'name': 'ori',
        'type': 'instruction',
        'op': INSTRUCTIONS['ori'].op_code,
        'funct': INSTRUCTIONS['ori'].funct_code,
        'label': label_lower
    }
    return [lui, ori]

def expand_nop(ir: dict) -> list[dict]:
    return [{
        'name': 'addi',
        'type': 'instruction',
        'op': INSTRUCTIONS['addi'].op_code,
        'funct': INSTRUCTIONS['addi'].funct_code,
        'r0': '$zero',
        'r1': '$zero',
        'immediate': {'type':'int', 'val':0},
    }]

PSEUDO_EXPANDERS = {
    'la': expand_la,
    'nop': expand_nop
}

