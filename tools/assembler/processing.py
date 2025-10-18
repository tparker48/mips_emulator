from tools.assembler.pseudo_expansion import PSEUDO_EXPANDERS, DATA_EXPANDERS

class ELFFile:
    None

def preprocess(lines: list[str]) -> list[str]:
    # implicit at '.text' at beginning 
    lines = '.text\n'+lines + '\n'
    return lines


def postprocess(ir, symbols: set):
    ir = expand_pseudoinstructions(ir)
    ir = expand_data(ir)
    ir = resolve_labels(ir, symbols)
    return ir


def expand_pseudoinstructions(ir):
    for segment_name in ir.keys():
        expanded_items = []
        for item in ir[segment_name]:
            if item['type'] == 'pseudo':
                expander = PSEUDO_EXPANDERS[item['name']]
                expanded_items.extend(expander(item))
            else:
                expanded_items.append(item)
        ir[segment_name] = expanded_items
    return ir

def expand_data(ir):
    addr_offsets = {
        '.text': 0x00400000,
        '.data': 0x10000000
    }
    for segment_name in ir.keys():
        expanded_items = []
        addr = addr_offsets[segment_name]
        for item in ir[segment_name]:
            if item['type'] == 'label_decl':
                # label declaration
                expanded_items.append(item)
                continue
            elif item['type'] == 'data_decl':
                # data declaration
                expander = DATA_EXPANDERS[item['data_type']]
                expanded_items.extend(expander(item, addr))
                addr += len(expanded_items) # 1 byte each
            else:
                # instruction
                addr += 4
                expanded_items.append(item)
        ir[segment_name] = expanded_items
    return ir


def resolve_label_value(label: str, symbol_table: dict) -> int:
    if not label.startswith('%'):
        return symbol_table[label]
    
    tag, label_name = label[1:].split('|')
    value = symbol_table[label_name]

    if tag == 'U':
        return (value >> 16) & 0xFFFF
    elif tag == 'L':
        return (value) & 0xFFFF
    elif tag.startswith('BE'):
        idx = int(tag.strip('BE'))
        return (value >> ((3-idx)*8)) & 0xFF

def construct_symbol_table(ir, labels: set) -> dict:
    addr_offsets = {
        '.text':0x00400000,
        '.data': 0x10000000
    }
    symbol_table = {
        label: 0 for label in labels
    }
    for segment_name in ir.keys():
        addr = addr_offsets[segment_name]
        for item in ir[segment_name]:
            item_type = item['type']
            if item_type == 'label_decl':
                symbol_table[item['label_name']] = addr
            elif item_type == 'instruction':
                addr+=4
            elif item_type == 'raw_byte':
                addr+=1
    print(symbol_table)
    return symbol_table

def resolve_labels(ir, labels: set):
    symbol_table = construct_symbol_table(ir, labels)
    for segment in ir.values():
        for item in segment:
            if 'label' not in item:
                continue
            item_type = item['type']
            label_stub = item['label']
            value = resolve_label_value(label_stub, symbol_table)

            del item['label']
            if item['type'] == 'instruction':
                item['immediate'] = value
            else:
                item['val'] = value
    return ir

def encode(ir):
    None