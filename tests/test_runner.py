import struct
import os
import subprocess
import argparse

def assemble_binary(asm_file: str, bin_file: str):
    os.system(f'python -m assembler {asm_file} -o {bin_file}')

def run_emulator(bin_file: str, out_file: str):
    with open(out_file, 'w') as f_out:
        subprocess.run(f'.\emulator\mips_sim.exe {bin_file}', stdout=f_out, stderr=f_out)

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

def check_emulator_output(out_file: str, expected_file: str):
    success = True

    with open(out_file, 'r') as out, open(expected_file, 'r') as expected:
        out_lines = out.readlines()
        expected_lines = expected.readlines()
    
    for i in range(min(len(out_lines), len(expected_lines))):
        if out_lines[i] != expected_lines[i]:
            print(f'    Output line {i+1} does not match!')
            print(f'        Expected: "{expected_lines[i].strip()}"')
            print(f'        Actual:   "{out_lines[i].strip()}"')
            success = False
    
    if len(out_lines) != len(expected_lines):
        print(f'Length mismatch')
        success = False

    return success

def run_assembler_tests():
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

        os.remove(bin_file)
        print(f"    Result: {'PASS' if test_result else 'FAIL'}")

def run_emulator_tests():
    root_dir = 'tests/emulator_tests/'
    assembler_tests = []
    for file in os.listdir(root_dir):
        if (file.endswith('.s')):
            assembler_tests.append(file[:-2])

    for test_name in assembler_tests:
        print("-------------------------------------")
        print(f"{test_name}:")
        asm_file = os.path.join(root_dir, test_name+'.s')
        bin_file = os.path.join(root_dir, test_name+'.bin')
        out_file = os.path.join(root_dir, test_name+'.output')
        exepected_output_file = os.path.join(root_dir, test_name+'.expected')

        print(f"    Assembling {asm_file}")
        assemble_binary(asm_file, bin_file)

        print(f"    Running Emulator {bin_file}")
        run_emulator(bin_file, out_file)

        print(f"    Checking output")
        test_result = check_emulator_output(out_file, exepected_output_file)
        
        os.remove(bin_file)
        os.remove(out_file)

        print(f"    Result: {'PASS' if test_result else 'FAIL'}")


if __name__ == '__main__':
    argparser = argparse.ArgumentParser(
        description="MIPS Assembler: Convert .s assembly files to binary."
    )

    argparser.add_argument(
        "-e", "--emulator",
        action="store_true",
        help="Run only emulator tests"
    )

    argparser.add_argument(
        "-a", "--assembler",
        action="store_true",
        help="Run only assembler tests"
    )

    args = argparser.parse_args()

    if (not args.assembler) and (not args.emulator):
        run_assembler_tests()
        run_emulator_tests()
    elif args.assembler:
        run_assembler_tests()
    elif args.emulator:
        run_emulator_tests()
