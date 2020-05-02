# Dumbfuck

A brainfuck interpreter written in C.

## Specs

* Tape size: 30000 cells
* Cell size: 8 bits (1 byte)
* Handles out of bounds on the tape by wrapping around
* Additions and substrations can overflow and underflow
* Invalid brainfuck characters are NOP

Note that if the brainfuck program is malformed (e.g. not an equal number of `[` and `]`), the interpreter may not behave as expected.
