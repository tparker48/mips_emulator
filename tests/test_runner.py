import struct
import os
import sys
import subprocess
import argparse

from typing import Callable

ASSEMBLER_TESTS_DIR = "tests\\assembler_tests\\"
EMULATOR_TESTS_DIR = "tests\\emulator_tests\\"
DIFFERENTIAL_TESTS_DIR = "tests\\differential_tests\\"

MARS_JAR_PATH = "tests\\tools\\Mars4_5.jar"

def run_test_group(group_dir: str, test_run_function: Callable, target_list: list[str] = []):
    test_group = []
    for path, _dir, files in os.walk(group_dir):
        for file in files:
            if (file.endswith('.s')):
                if not target_list or file[:-2] in target_list:
                    with open(os.path.join(path,file), 'r') as testfile:
                        if testfile.readline().strip() == "# TODO":
                            print(f"Skipping {file} (TODO)")
                        else:
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
    
    if test_result:
        os.remove(bin_file)
        os.remove(out_file)
    
    return test_result

def run_emulator(bin_file: str, out_file: str):
    with open(out_file, 'w') as f_out:
        subprocess.run(f'.\\emulator\\mips_sim.exe {bin_file}', stdout=f_out, stderr=f_out)

def check_emulator_output(out_file: str, expected_file: str) -> bool:
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


def run_differential_test(test_path: str) -> bool:
    asm_file = test_path+'.s'
    bin_file = test_path+'.bin'
    out_file = test_path+'.output'
    mars_out_file = test_path+'.mars_output'
    
    if not os.path.exists(MARS_JAR_PATH):
        print(f"ERROR: '{MARS_JAR_PATH}' not found.")
        print("Differential tests require Mars4_5.jar.")
        print("Please see: tests/tools/README.md")
        sys.exit(1)

    assemble_binary(asm_file, bin_file)
    run_emulator(bin_file, out_file)
    with open (mars_out_file, 'w') as out:
        subprocess.run(f'java -jar {MARS_JAR_PATH} {asm_file}', stdout=out, stderr=out)

    with open(mars_out_file, 'r') as mars_out, open(out_file, 'r') as emulator_out:
        mars_output = mars_out.readlines()
        emulator_output = emulator_out.readlines()
    
    test_result = check_differential_output(emulator_output, mars_output)

    if test_result:
        os.remove(bin_file)
        os.remove(out_file)
        os.remove(mars_out_file)

    return test_result


def check_differential_output(emulator_output: list[str], mars_output: list[str]) -> bool:
    success = True

    # remove boilerplate lines etc
    emulator_output = emulator_output[:-2]
    mars_output = mars_output[2:-2] 

    for i in range(min(len(emulator_output), len(mars_output))):
        if emulator_output[i] != mars_output[i]:
            print(f'    Output line {i+1} does not match!')
            print(f'        Expected: "{mars_output[i].strip()}"')
            print(f'        Actual:   "{emulator_output[i].strip()}"')
            success = False
    
    if len(emulator_output) != len(mars_output):
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
        help="Run emulator tests"
    )

    argparser.add_argument(
        "-a", "--assembler",
        action="store_true",
        help="Run assembler tests"
    )

    argparser.add_argument(
        "-d", "--differential",
        action="store_true",
        help="Run differential tests"
    )

    argparser.add_argument(
        "-l", "--list",
        nargs="*",
        help= "List of tests to run"
    )

    args = argparser.parse_args()

    run_em = args.emulator
    run_as = args.assembler
    run_df = args.differential

    if not (run_em or run_as or run_df):
        run_em = True
        run_as = True
        run_df = True

    if run_as:
        run_test_group(ASSEMBLER_TESTS_DIR, run_assembler_test, target_list=args.list)
    if run_em:
        run_test_group(EMULATOR_TESTS_DIR, run_emulator_test, target_list=args.list)
    if run_df:
        run_test_group(DIFFERENTIAL_TESTS_DIR, run_differential_test, target_list=args.list)
    
