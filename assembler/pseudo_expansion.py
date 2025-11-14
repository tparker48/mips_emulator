from assembler.isa import INSTRUCTIONS

def instruction_data(name: str):
    return {
        'name': name,
        'op': INSTRUCTIONS[name].op_code,
        'funct': INSTRUCTIONS[name].funct_code,
        'type': 'instruction'
    }


def expand_move(ir_data: dict):
    return [{
        **ir_data,
        **instruction_data('add'),
        'r2': '$zero'
    }]
def expand_neg(ir_data: dict):
    return [{
        **ir_data,
        **instruction_data('sub'),
        'r1': '$zero',
        'r2': ir_data['r1']
    }]
def expand_not(ir_data: dict):
    return [{
        **ir_data,
        **instruction_data('nor'),
        'r2':'$zero',
    }]
def expand_clear(ir_data: dict):
    return [{
        **ir_data,
        **instruction_data('add'),
        'r1': '$zero',
        'r2': '$zero'
    }]
def expand_la(ir_data: dict) -> list[dict]:
    if 'label' not in ir_data:
        print(f'Line {ir_data["lineno"]}: "la" must be used with a label!')
        return [ir_data]
    label_upper = '%U|'+ir_data['label']
    label_lower = '%L|'+ir_data['label']
    lui = {
        **ir_data,
        **instruction_data('lui'),
        'label': label_upper
    }
    ori = {
        **ir_data,
        **instruction_data('ori'),
        'r1': ir_data['r0'],
        'label': label_lower
    }
    return [lui, ori]
def expand_li(ir_data: dict):
    immediate = ir_data['immediate']['val']
    upper = (immediate >> 16) & 0x0000FFFF
    lower = immediate & 0x0000FFFF
    lui = {
        **ir_data,
        **instruction_data('lui'),
        'immediate': {'type':'int', 'val': upper}
    }
    ori = {
        **ir_data,
        **instruction_data('ori'),
        'r1': ir_data['r0'],
        'immediate': {'type':'int', 'val': lower}
    }
    return [lui, ori]
def expand_b(ir_data: dict):
    return [{
        **ir_data,
        **instruction_data('beq'),
        'r0': '$zero',
        'r1': '$zero'
    }]
def expand_blt(ir_data: dict):
    slt = {
        **ir_data,
        **instruction_data('slt'),
        'r0':'$at',
        'r1':ir_data['r0'],
        'r2':ir_data['r1'],
    }
    bne = {
        **ir_data,
        **instruction_data('bne'),
        'r0': '$at',
        'r1': '$zero',
    }
    return [slt, bne]
def expand_bgt(ir_data: dict):
    slt = {
        **ir_data,
        **instruction_data('slt'),
        'r0':'$at',
        'r1':ir_data['r1'],
        'r2':ir_data['r0'],
    }
    bne = {
        **ir_data,
        **instruction_data('bne'),
        'r0': '$at',
        'r1': '$zero',
    }
    return [slt, bne]
def expand_ble(ir_data: dict):
    slt = {
        **ir_data,
        **instruction_data('slt'),
        'r0':'$at',
        'r1':ir_data['r1'],
        'r2':ir_data['r0'],
    }
    beq = {
        **ir_data,
        **instruction_data('beq'),
        'r0': '$at',
        'r1': '$zero',
    }
    return [slt, beq]
def expand_bge(ir_data: dict):
    slt = {
        **ir_data,
        **instruction_data('slt'),
        'r0':'$at',
        'r1':ir_data['r0'],
        'r2':ir_data['r1'],
    }
    beq = {
        **ir_data,
        **instruction_data('beq'),
        'r0': '$at',
        'r1': '$zero',
    }
    return [slt, beq]
def expand_beqz(ir_data: dict):
    return [{
        **ir_data,
        **instruction_data('beq'),
        'r1': '$zero'
    }]
def expand_bnez(ir_data: dict):
    return [{
        **ir_data,
        **instruction_data('bne'),
        'r1': '$zero'
    }]
def expand_push(ir_data: dict):
    addi = {
        **ir_data,
        **instruction_data('addi'),
        'r0':'$sp',
        'r1':'$sp',
        'immediate': {'type': 'int', 'val': -4}
    }
    sw = {
        **ir_data,
        **instruction_data('sw'),
        'r0': '$sp',
        'r1': ir_data['r0'],
        'immediate': {'type': 'int', 'val': 0}
    }
    return [addi, sw]
def expand_pop(ir_data: dict):
    lw = {
        **ir_data,
        **instruction_data('lw'),
        'r0': '$sp',
        'r1': ir_data['r0'],
        'immediate': {'type': 'int', 'val': 0}
    }
    addi = {
        **ir_data,
        **instruction_data('addi'),
        'r0':'$sp',
        'r1':'$sp',
        'immediate': {'type': 'int', 'val': 4}
    }
    return [lw, addi]
def expand_nop(ir_data: dict) -> list[dict]:
    return [{
        **ir_data,
        **instruction_data('addi'),
        'r0': '$zero',
        'r1': '$zero',
        'immediate': {'type':'int', 'val':0},
    }]

PSEUDO_EXPANDERS = {
    'move': expand_move,
    'neg': expand_neg,
    'not': expand_not,
    'clear': expand_clear,
    'la': expand_la,
    'li': expand_li,
    'b': expand_b,
    'blt': expand_blt, 
    'bgt': expand_bgt, 
    'ble': expand_ble, 
    'bge': expand_bge,
    'beqz': expand_beqz,
    'bnez': expand_bnez,
    'push': expand_push,
    'pop': expand_pop,
    'nop': expand_nop,
}
