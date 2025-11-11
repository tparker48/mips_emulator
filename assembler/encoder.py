from assembler.instruction_encoders import INSTRUCTION_ENCODERS

def encode_binary(ir_data: dict) -> bytearray:
    # 1st word: TEXT length
    # 2nd word: DATA length
    # TEXT
    # DATA

    segment_bytes = {key: bytearray() for key in ir_data.keys()}

    for segment, segment_data in ir_data.items():
        for ir_entry in segment_data:
            raw_bytes = generate_bytes(ir_entry)
            if raw_bytes:
                segment_bytes[segment] += raw_bytes

    file_header = bytearray()
    file_segments = bytearray()
    for segment in ['.text', '.data']:
        length = len(segment_bytes[segment]) if segment in segment_bytes else 0
        file_header += length.to_bytes(4, 'big')
        if segment in segment_bytes:
            file_segments += segment_bytes[segment]
    
    return file_header + file_segments

def generate_bytes(ir_entry: dict) -> bytes:
    ir_type = ir_entry['type']
    if ir_type == 'raw_byte':
        return generate_raw_byte(ir_entry)
    elif ir_type == 'instruction':
        return generate_instruction_bytes(ir_entry)

def generate_raw_byte(ir_entry: dict) -> bytes:
    val: int = ir_entry['val']
    return val.to_bytes(1, byteorder='big')

def generate_instruction_bytes(ir_entry: dict) -> bytes:
    return INSTRUCTION_ENCODERS[ir_entry['name']](ir_entry)

    