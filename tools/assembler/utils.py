
def comma_separated_args(args:str) -> list[str]:
    return [w.strip() for w in args.strip().split(',')]

def fits_uint16(val):
    if 0 <= val <= 2**16 - 1:
        print(f'label value {val} fits in 16 bits')
        return True
