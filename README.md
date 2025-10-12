# MIPS Simulator

A MIPS CPU simulator written in C, with a supporting Python assembler for converting MIPS assembly to binary.

## Project Structure
mips_sim/
├─ include/ # Public headers
├─ src/ # C source files
├─ tools/ # Python scripts (assembler)
├─ tests/ # Sample assembly programs
├─ bin/ # Output binaries
├─ Makefile # Build script
└─ README.md

## Building
```
make
```
This produces the executable mips_sim (or mips_sim.exe on Windows).



## Running the Simulator

1. Assemble a MIPS program using the Python assembler:

```
python3 tools/mips_assembler.py tests/hello.asm -o bin/hello.bin
```

2. Run the simulator
```
./mips_sim bin/hello.bin
```

## Cleaning Build Files
```
make clean
```
