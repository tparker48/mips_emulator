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


def expand_space(ir: dict, _:int) -> list[dict]:
    lineno = ir['lineno']
    space = ir['literal']['val']
    return [{'type': 'raw_byte', 'val': 0x00, 'lineno': lineno} for _ in range(space)]

def expand_ascii(ir: dict, _: int) -> list[dict]:
    lineno = ir['lineno']
    return [{'type': 'raw_byte', 'val': b, 'lineno': lineno} for b in ir['literal']['val']]

def get_align_bytes(lineno:int, addr:int, alignment: int) -> list[dict]:
    aligned_addr = (addr + alignment - 1) & ~(alignment - 1)
    padding_size = aligned_addr - addr
    return [{'type': 'raw_byte', 'val': 0, 'lineno': lineno} for _ in range(padding_size)]

def expand_align(ir: dict, addr: int) -> list[dict]:
    lineno = ir.get('lineno', 0)
    val = ir['literal']['val']
    alignment = 2 ** val
    return get_align_bytes(lineno, addr, alignment)

def expand_label_big_endian(lineno:int, label: str) -> list[dict]:
    # label is 1 word, list as bytes, BIG ENDIAN
    return [
        {'type': 'raw_byte', 'label': f'%BE0|'+label, 'lineno': lineno},
        {'type': 'raw_byte', 'label': f'%BE1|'+label, 'lineno': lineno},
        {'type': 'raw_byte', 'label': f'%BE2|'+label, 'lineno': lineno},
        {'type': 'raw_byte', 'label': f'%BE3|'+label, 'lineno': lineno},
    ]

def expand_bytes_big_endian(lineno: int, unit_size: int, val: int) -> list[dict]:
    output = []
    for i in reversed(range(unit_size)):
        byte = (val >> (i * 8)) & 0xFF
        output.append({'type': 'raw_byte', 'val': byte, 'lineno': lineno})
    return output

def expand_word(ir: dict, addr: int) -> list[dict]:
    lineno = ir.get('lineno', 0)
    output = []
    unit_size = 4
    output.extend(get_align_bytes(lineno, addr, unit_size))
    if 'label' in ir:
        output.extend(expand_label_big_endian(lineno, ir['label']))
    else:
        output.extend(expand_bytes_big_endian(lineno, unit_size, ir['literal']['val']))
    return output

def expand_half(ir: dict, addr: int) -> list[dict]:
    lineno = ir.get('lineno', 0)
    output = []
    unit_size = 2
    output.extend(get_align_bytes(lineno, addr, unit_size))
    output.extend(expand_bytes_big_endian(lineno, unit_size, ir['literal']['val']))
    return output

def expand_byte(ir: dict, addr: int) -> list[dict]:
    lineno = ir.get('lineno', 0)
    byte = ir['literal']['val']
    return [{'type': 'raw_byte', 'val': byte, 'lineno': lineno}]

PSEUDO_EXPANDERS = {
    'la': expand_la
}

DATA_EXPANDERS = {
    'word': expand_word,
    'half': expand_half,
    'byte': expand_byte,
    'space': expand_space,
    'ascii': expand_ascii,
    'asciiz': expand_ascii,
    'align': expand_align
}