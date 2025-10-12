# MIPS Simulator

A MIPS CPU simulator written in C, with a supporting Python assembler for converting MIPS assembly to binary.

## Building
```
make
```
This produces the executable mips_sim (or mips_sim.exe on Windows).



## Running the Simulator

1. Assemble a MIPS program using the Python assembler:

```
todo
```

2. Run the simulator
```
./mips_sim bin/hello.bin
```

## Running Tests
```
python -m tests.test_runner
```

## Cleaning Build Files
```
make clean
```
