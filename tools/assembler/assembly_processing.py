from tools.assembler.pseudo_expansion import PSEUDO_EXPANDERS
from tools.assembler.data_expansion import DATA_EXPANDERS
from tools.assembler.isa import ADDRESS_OFFSETS
from tools.assembler.symbol_table import create_symbol_table, resolve_symbol


def preprocess(lines: str) -> str:
    lines = '.text\n'+lines + '\n'
    return lines

def postprocess(ir: dict, labels: set):
    ir = expand_pseudoinstructions(ir)
    ir = expand_data(ir)
    ir = resolve_labels(ir, labels)
    return ir


# ------------ Post Processing Steps ------------
def expand_pseudoinstructions(ir: dict) -> dict:
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

def expand_data(ir: dict) -> dict:
    for segment_name in ir.keys():
        expanded_items = []
        addr = ADDRESS_OFFSETS[segment_name]
        for item in ir[segment_name]:
            if item['type'] == 'label_decl':
                # label declaration
                expanded_items.append(item)
                continue
            elif item['type'] == 'data_decl':
                # data declaration
                expander = DATA_EXPANDERS[item['data_type']]
                result = expander(item, addr)
                expanded_items.extend(result)
                addr += len(result)
            else:
                # instruction
                addr += 4
                expanded_items.append(item)
        ir[segment_name] = expanded_items
    return ir

def resolve_labels(ir: dict, labels: set) -> dict:
    symbol_table = create_symbol_table(ir, labels)

    for segment in ir.values():
        for item in segment:
            if 'label' in item:
                label_stub = item['label']
                value = resolve_symbol(label_stub, symbol_table)
                del item['label']
                if item['type'] == 'instruction':
                    item['immediate'] = value
                else:
                    item['val'] = value
    return ir
