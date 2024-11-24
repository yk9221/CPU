# 6502 Processor

## Introduction
A highly accurate emulator for the MOS 6502 processor, written from scratch. This project simulates the 6502 CPU, its instruction set, and its behavior, providing a platform to learn, test, and experiment with assembly programming. Instructions were refereneced from [6502](http://6502.org/users/obelisk/).

## Features
- Full Instruction Set Support: Implements all 6502 instructions, including addressing modes.
- Accurate Cycle Emulation: Ensures proper timing for each instruction, including page-crossing behavior.
- Stack Support: Implements stack operations (PHA, PLA, JSR, RTS, etc.).

## Installation
Clone the repository:
```bash
git clone https://github.com/yk9221/CPU.git
cd CPU
```

Building the project:
```bash
make all
```

Running the project:
```bash
make run FILE=<filename>.yk
```

Cleaning the project:
```bash
make clean
```


## Examples
Sample Assembly Code
```
    JSR init
    JSR loop
    JSR end

init:
    LDX #$00
    RTS

loop:
    INX
    CPX #$05
    BNE loop
    RTS

end:
    BRK
```

Sample Output:

```
Registers:
 PC  SP A  X  Y  NV-BDIZC
0613 FD 00 05 00 00110011
```

An output.bin file is generated to verify the binary values produced by the simulator. The Program Counter (PC) is initialized to 0x0600, and the Stack Pointer (SP) starts at 0x01FF by default.

```
0x0600: 20 09 06 20 0C 06 20 12 06 A2 00 60 E8 E0 05 D0
0x0610: Fb 60 00 00 00 00 00 00 00 00 00 00 00 00 00 00
```
