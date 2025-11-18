# Differential Testing Tools

Differential tests run our emulator against the MARS emulator and compare their outputs to ensure correctness.

## Requirements

To run the differential tests, `Mars4_5.jar` must be placed **in this folder**.

You can download `Mars4_5.jar` from the official MARS repository:

https://github.com/dpetersanderson/MARS/releases/tag/v.4.5.1

## Running the Tests
- Run **all** tests:
  - `python tests/test_runner.py`
- Run **only differential** tests:
  - `python tests/test_runner.py -d`
