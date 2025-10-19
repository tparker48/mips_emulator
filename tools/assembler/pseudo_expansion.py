from tools.assembler.isa import INSTRUCTIONS

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
        'name': 'ori',
        'type': 'instruction',
        'op': INSTRUCTIONS['ori'].op_code,
        'funct': INSTRUCTIONS['ori'].funct_code,
        'label': label_lower
    }
    return [lui, ori]

PSEUDO_EXPANDERS = {
    'la': expand_la
}

