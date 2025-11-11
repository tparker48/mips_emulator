from assembler.isa import ADDRESS_OFFSETS

def create_symbol_table(ir: dict, labels:set) -> dict:
    table = {
        label: None for label in labels
    }
    for segment_name in ir.keys():
        addr = ADDRESS_OFFSETS[segment_name]
        for item in ir[segment_name]:
            item['addr'] = addr
            item_type = item['type']
            if item_type == 'label_decl':
                table[item['label_name']] = addr
            elif item_type == 'instruction':
                addr+=4
            elif item_type == 'raw_byte':
                addr+=1

    return table

def resolve_symbol(label: str, symbol_table: dict) -> int:
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