import struct
import os

def assemble_binary(asm_file: str, bin_file: str):
    os.system(f'python -m tools.assembler {asm_file} -o {bin_file}')

def check_assembler_output(binary_file: str, reference_file: str):
    success = True

    with open(reference_file, 'r') as f:
        expected_words = f.readlines()
    expected_words = [word.strip() for word in expected_words]
    expected_words = [word for word in expected_words if word]

    words = []
    with open(binary_file, 'rb') as f:
        data = f.read()
    for i in range(8, len(data), 4):
        word = data[i:i+4]
        raw = struct.unpack('>I', word)[0]
        words.append(f'{raw:08X}')

    for i in range(min(len(words), len(expected_words))):
        if words[i] != expected_words[i]:
            print(f'    Instruction Word {i+1} does not match!')
            print(f'        Expected: 0x{expected_words[i]}')
            print(f'        Actual:   0x{words[i]}')
            success = False

    if (len(words) != len(expected_words)):
        print(f'Length mismatch')
        success = False
    
    return success


def run_assebler_tests():
    root_dir = 'tests/assembler_tests/'
    assembler_tests = []
    for file in os.listdir(root_dir):
        if (file.endswith('.s')):
            assembler_tests.append(file[:-2])

    for test_name in assembler_tests:
        print("-------------------------------------")
        print(f"{test_name}:")
        asm_file = os.path.join(root_dir, test_name+'.s')
        bin_file = os.path.join(root_dir, test_name+'.bin')
        bin_exepected_file = os.path.join(root_dir, test_name+'.bin.expected')

        print(f"    Assembling {asm_file}")
        assemble_binary(asm_file, bin_file)

        print(f"    Checking {bin_file}")
        test_result = check_assembler_output(bin_file, bin_exepected_file)
        
        print(f"    Cleaning up {bin_file}")
        os.remove(bin_file)

        print(f"    Result: {'PASS' if test_result else 'FAIL'}")

if __name__ == '__main__':
    run_assebler_tests()