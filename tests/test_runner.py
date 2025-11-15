import struct
import os
import subprocess
import argparse

from typing import Callable


def run_test_group(group_dir: str, test_run_function: Callable):
    test_group = []
    for path, _dir, files in os.walk(group_dir):
        for file in files:
            if (file.endswith('.s')):
                test_group.append(os.path.join(path,file[:-2]))

    for test_path in test_group:
        print("-------------------------------------")
        print(f"{test_path}:")
        result = test_run_function(test_path)
        print(f"    Result: {'PASS' if result else 'FAIL'}")


def run_assembler_test(test_path: str) -> bool:
    asm_file = test_path+'.s'
    bin_file = test_path+'.bin'
    bin_exepected_file = test_path+'.bin.expected'

    assemble_binary(asm_file, bin_file)
    test_result = check_assembler_output(bin_file, bin_exepected_file)

    if test_result:
        os.remove(bin_file)

    return test_result

def assemble_binary(asm_file: str, bin_file: str):
    os.system(f'python -m assembler {asm_file} -o {bin_file}')

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


def run_emulator_test(test_path: str) -> bool:
    asm_file = test_path+'.s'
    bin_file = test_path+'.bin'
    out_file = test_path+'.output'
    exepected_output_file = test_path+'.expected'

    assemble_binary(asm_file, bin_file)
    run_emulator(bin_file, out_file)

    test_result = check_emulator_output(out_file, exepected_output_file)
    
    if (test_result):
        os.remove(bin_file)
        os.remove(out_file)
    
    return test_result

def run_emulator(bin_file: str, out_file: str):
    with open(out_file, 'w') as f_out:
        subprocess.run(f'.\emulator\mips_sim.exe {bin_file}', stdout=f_out, stderr=f_out)

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
        run_test_group('tests/assembler_tests/', run_assembler_test)
        run_test_group('tests/emulator_tests/', run_emulator_test)
    elif args.assembler:
        run_test_group('tests/assembler_tests/', run_assembler_test)
    elif args.emulator:
        run_test_group('tests/emulator_tests/', run_emulator_test)
