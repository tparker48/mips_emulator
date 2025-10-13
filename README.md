# MIPS Emulator

 - A MIPS CPU emulator written in C
    - Emulates full MIPS 5-stage pipeline (IF, ID, EXE, MEM, WB)
    - Forwarding and Bubble Insertion for RAW hazards
    - Interrupt handling (WIP)

 - MIPS assembler written in Python
    - Writes assembly files (text) to binary (big endian)

## Building
```
make
```
This produces the executable mips_sim (or mips_sim.exe on Windows).



## Running the Simulator

1. Assemble a MIPS program using the Python assembler:

```
python -m tools.assembler.mips_assembler [assembly_file].s -o [output_file].bin --verbose
```

2. Run the simulator
```
./mips_sim bin/[filename].bin
```

## Running Tests
```
python -m tests.test_runner
```

## Cleaning Build Files
```
make clean
```
