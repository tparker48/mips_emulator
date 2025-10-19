# MIPS Emulator

 - A MIPS CPU emulator written in C
    - Emulates full MIPS 5-stage pipeline (IF, ID, EXE, MEM, WB)
    - Forwarding and bubble insertion for RAW hazards
    - Interrupt handling (WIP)
    - Full instruction set support (WIP)
    - Debugging (WIP)

 - MIPS assembler written in Python
    - Writes assembly files (.s/.asm) to binary (.bin) (big endian)



# Assembler: Generating Machine Code from MIPS Assembly
---

## Requirements

- **Python 3.10+**
- **sly** (Sly Lex-Yacc) - used for lexing and parsing

```bash
pip install -r requirements.txt
```
## Running the Assembler
```
TODO
```

# CPU Emulator: Execute MIPS Machine Code
---
## Building
```
make
```
This produces the executable mips_sim (or mips_sim.exe on Windows).

Clean with:
```
make clean
```

## Running

Run an assembled binary with:
```
./mips_sim [my_mips_binary]
```

# Running Tests
---
Run tests with:
```
python -m tests.test_runner
```
