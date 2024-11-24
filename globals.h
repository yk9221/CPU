#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cstdlib>

#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <unordered_set>


#define RAM_SIZE (64 * 1024)
#define GRIDSIZE 32
#define MAX_BYTE 255
#define MAX_WORD 65535

using Byte = uint8_t;
using signedByte = int8_t;
using Word = uint16_t;
using namespace std;

enum AddressingMode {
    Implicit,
    Accumulator,
    Immediate,
    ZeroPage,
    ZeroPageX,
    ZeroPageY,
    Relative,
    Absolute,
    AbsoluteX,
    AbsoluteY,
    Indirect,
    IndirectX,
    IndirectY,
    Error,
    Label,
    Define
};

enum Bits {
    CARRY_BIT,
    ZERO_BIT,
    INTERRUPT_BIT,
    DECIMAL_BIT,
    BREAK_BIT,
    UNUSED_BIT,
    OVERFLOW_BIT,
    NEGATIVE_BIT
};

enum Opcode {
    // ADC
    ADC_IMM = 0x69,
    ADC_ZP = 0x65,
    ADC_ZPX = 0x75,
    ADC_AB = 0x6D,
    ADC_ABX = 0x7D,
    ADC_ABY = 0x79,
    ADC_IDX = 0x61,
    ADC_IDY = 0x71,
    // AND
    AND_IMM = 0x29,
    AND_ZP = 0x25,
    AND_ZPX = 0x35,
    AND_AB = 0x2D,
    AND_ABX = 0x3D,
    AND_ABY = 0x39,
    AND_IDX = 0x21,
    AND_IDY = 0x31,
    // ASL
    ASL_ACC = 0x0A,
    ASL_ZP = 0x06,
    ASL_ZPX = 0x16,
    ASL_AB = 0x0E,
    ASL_ABX = 0x1E,
    // BCC
    BCC_REL = 0x90,
    // BCS
    BCS_REL = 0xB0,
    // BEQ
    BEQ_REL = 0xF0,
    // BIT
    BIT_ZP = 0x24,
    BIT_AB = 0x2C,
    // BMI
    BMI_REL = 0x30,
    // BNE
    BNE_REL = 0xD0,
    // BPL
    BPL_REL = 0x10,
    // BRK
    BRK_IMP = 0x00,
    // BVC
    BVC_REL = 0x50,
    // BVS
    BVS_REL = 0x70,
    // CLC
    CLC_IMP = 0x18,
    // CLD
    CLD_IMP = 0xD8,
    // CLI
    CLI_IMP = 0x58,
    // CLV
    CLV_IMP = 0xB8,
    // CMP
    CMP_IMM = 0xC9,
    CMP_ZP = 0xC5,
    CMP_ZPX = 0xD5,
    CMP_AB = 0xCD,
    CMP_ABX = 0xDD,
    CMP_ABY = 0xD9,
    CMP_IDX = 0xC1,
    CMP_IDY = 0xD1,
    // CPX
    CPX_IMM = 0xE0,
    CPX_ZP = 0xE4,
    CPX_AB = 0xEC,
    // CPY
    CPY_IMM = 0xC0,
    CPY_ZP = 0xC4,
    CPY_AB = 0xCC,
    // DEC
    DEC_ZP = 0xC6,
    DEC_ZPX = 0xD6,
    DEC_AB = 0xCE,
    DEC_ABX = 0xDE,
    // DEX
    DEX_IMP = 0xCA,
    // DEY
    DEY_IMP = 0x88,
    // EOR
    EOR_IMM = 0x49,
    EOR_ZP = 0x45,
    EOR_ZPX = 0x55,
    EOR_AB = 0x4D,
    EOR_ABX = 0x5D,
    EOR_ABY = 0x59,
    EOR_IDX = 0x41,
    EOR_IDY = 0x51,
    // INC
    INC_ZP = 0xE6,
    INC_ZPX = 0xF6,
    INC_AB = 0xEE,
    INC_ABX = 0xFE,
    // INX
    INX_IMP = 0xE8,
    // INY
    INY_IMP = 0xC8,
    // JMP
    JMP_AB = 0x4C,
    JMP_ID = 0x6C,
    //JSR
    JSR_AB = 0x20,
    // LDA
    LDA_IMM = 0xA9,
    LDA_ZP = 0xA5,
    LDA_ZPX = 0xB5,
    LDA_AB = 0xAD,
    LDA_ABX = 0xBD,
    LDA_ABY = 0xB9,
    LDA_IDX = 0xA1,
    LDA_IDY = 0xB1,
    // LDX
    LDX_IMM = 0xA2,
    LDX_ZP = 0xA6,
    LDX_ZPY = 0xB6,
    LDX_AB = 0xAE,
    LDX_ABY = 0xBE,
    // LDY
    LDY_IMM = 0xA0,
    LDY_ZP = 0xA4,
    LDY_ZPX = 0xB4,
    LDY_AB = 0xAC,
    LDY_ABX = 0xBC,
    // LSR
    LSR_ACC = 0x4A,
    LSR_ZP = 0x46,
    LSR_ZPX = 0x56,
    LSR_AB = 0x4E,
    LSR_ABX = 0x5E,
    // NOP
    NOP_IMP = 0xEA,
    // ORA
    ORA_IMM = 0x09,
    ORA_ZP = 0x05,
    ORA_ZPX = 0x15,
    ORA_AB = 0x0D,
    ORA_ABX = 0x1D,
    ORA_ABY = 0x19,
    ORA_IDX = 0x01,
    ORA_IDY = 0x11,
    // PHA
    PHA_IMP = 0x48,
    // PHP
    PHP_IMP = 0x08,
    // PLA
    PLA_IMP = 0x68,
    // PLP
    PLP_IMP = 0x28,
    // ROL
    ROL_ACC = 0x2A,
    ROL_ZP = 0x26,
    ROL_ZPX = 0x36,
    ROL_AB = 0x2E,
    ROL_ABX = 0x3E,
    // ROR
    ROR_ACC = 0x6A,
    ROR_ZP = 0x66,
    ROR_ZPX = 0x76,
    ROR_AB = 0x6E,
    ROR_ABX = 0x7E,
    // RTI
    RTI_IMP = 0x40,
    // RTS
    RTS_IMP = 0x60,
    // SBC
    SBC_IMM = 0xE9,
    SBC_ZP = 0xE5,
    SBC_ZPX = 0xF5,
    SBC_AB = 0xED,
    SBC_ABX = 0xFD,
    SBC_ABY = 0xF9,
    SBC_IDX = 0xE1,
    SBC_IDY = 0xF1,
    // SEC
    SEC_IMP = 0x38,
    // SED
    SED_IMP = 0xF8,
    // SEI
    SEI_IMP = 0x78,
    // STA
    STA_ZP = 0x85,
    STA_ZPX = 0x95,
    STA_AB = 0x8D,
    STA_ABX = 0x9D,
    STA_ABY = 0x99,
    STA_IDX = 0x81,
    STA_IDY = 0x91,
    // STX
    STX_ZP = 0x86,
    STX_ZPY = 0x96,
    STX_AB = 0x8E,
    // STX
    STY_ZP = 0x84,
    STY_ZPX = 0x94,
    STY_AB = 0x8C,
    // TAX
    TAX_IMP = 0xAA,
    // TAY
    TAY_IMP = 0xA8,
    // TSX
    TSX_IMP = 0xBA,
    // TXA
    TXA_IMP = 0x8A,
    // TXS
    TXS_IMP = 0x9A,
    // TYA
    TYA_IMP = 0x98
};

enum Flags {
    C = (1 << CARRY_BIT),
    Z = (1 << ZERO_BIT),
    I = (1 << INTERRUPT_BIT),
    D = (1 << DECIMAL_BIT),
    B = (1 << BREAK_BIT),
    U = (1 << UNUSED_BIT),
    V = (1 << OVERFLOW_BIT),
    N = (1 << NEGATIVE_BIT),
};

enum ModeByte {
    ImplicitBytes = 1,
    AccumulatorBytes = 1,
    ImmediateBytes = 2,
    ZeroPageBytes = 2,
    ZeroPageXBytes = 2,
    ZeroPageYBytes = 2,
    RelativeBytes = 2,
    AbsoluteBytes = 3,
    AbsoluteXBytes = 3,
    AbsoluteYBytes = 3,
    IndirectBytes = 3,
    IndirectXBytes = 2,
    IndirectYBytes = 2,
    ErrorBytes = 0,
    LabelBytes = 0,
    DefineByte = 0

};

typedef struct AddressLabel {
    Word value;
    AddressingMode mode;
} AddressLabel;

enum Radix {
    BIN,
    OCT,
    DEC,
    HEX,
    ERR
};

inline unordered_map<string, string> instruction_map {
    // ADC
    {"69", "ADC_IMM"},
    {"65", "ADC_ZP"},
    {"75", "ADC_ZPX"},
    {"6d", "ADC_AB"},
    {"7d", "ADC_ABX"},
    {"79", "ADC_ABY"},
    {"61", "ADC_IDX"},
    {"71", "ADC_IDY"},
    // AND
    {"29", "AND_IMM"},
    {"25", "AND_ZP"},
    {"35", "AND_ZPX"},
    {"2d", "AND_AB"},
    {"3d", "AND_ABX"},
    {"39", "AND_ABY"},
    {"21", "AND_IDX"},
    {"31", "AND_IDY"},
    // ASL
    {"0a", "ASL_ACC"},
    {"06", "ASL_ZP"},
    {"16", "ASL_ZPX"},
    {"0e", "ASL_AB"},
    {"1e", "ASL_ABX"},
    // BCC
    {"90", "BCC_REL"},
    // BCS
    {"b0", "BCS_REL"},
    // BEQ
    {"f0", "BEQ_REL"},
    // BIT
    {"24", "BIT_ZP"},
    {"2c", "BIT_AB"},
    // BMI
    {"30", "BMI_REL"},
    // BNE
    {"d0", "BNE_REL"},
    // BPL
    {"10", "BPL_REL"},
    // BRK
    {"00", "BRK_IMP"},
    // BVC
    {"50", "BVC_REL"},
    // BVS
    {"70", "BVS_REL"},
    // CLC
    {"18", "CLC_IMP"},
    // CLD
    {"d8", "CLD_IMP"},
    // CLI
    {"58", "CLI_IMP"},
    // CLV
    {"b8", "CLV_IMP"},
    // CMP
    {"c9", "CMP_IMM"},
    {"c5", "CMP_ZP"},
    {"d5", "CMP_ZPX"},
    {"cd", "CMP_AB"},
    {"dd", "CMP_ABX"},
    {"d9", "CMP_ABY"},
    {"c1", "CMP_IDX"},
    {"d1", "CMP_IDY"},
    // CPX
    {"e0", "CPX_IMM"},
    {"e4", "CPX_ZP"},
    {"ec", "CPX_AB"},
    // CPY
    {"c0", "CPY_IMM"},
    {"c4", "CPY_ZP"},
    {"cc", "CPY_AB"},
    // DEC
    {"c6", "DEC_ZP"},
    {"d6", "DEC_ZPX"},
    {"ce", "DEC_AB"},
    {"de", "DEC_ABX"},
    // DEX
    {"ca", "DEX_IMP"},
    // DEY
    {"88", "DEY_IMP"},
    // EOR
    {"49", "EOR_IMM"},
    {"45", "EOR_ZP"},
    {"55", "EOR_ZPX"},
    {"4d", "EOR_AB"},
    {"5d", "EOR_ABX"},
    {"59", "EOR_ABY"},
    {"41", "EOR_IDX"},
    {"51", "EOR_IDY"},
    // INC
    {"e6", "INC_ZP"},
    {"f6", "INC_ZPX"},
    {"ee", "INC_AB"},
    {"fe", "INC_ABX"},
    // INX
    {"e8", "INX_IMP"},
    // INY
    {"c8", "INY_IMP"},
    // JMP
    {"4c", "JMP_AB"},
    {"6c", "JMP_ID"},
    //JSR
    {"20", "JSR_AB"},
    // LDA
    {"a9", "LDA_IMM"},
    {"a5", "LDA_ZP"},
    {"b5", "LDA_ZPX"},
    {"ad", "LDA_AB"},
    {"bd", "LDA_ABX"},
    {"b9", "LDA_ABY"},
    {"a1", "LDA_IDX"},
    {"b1", "LDA_IDY"},
    // LDX
    {"a2", "LDX_IMM"},
    {"a6", "LDX_ZP"},
    {"b6", "LDX_ZPY"},
    {"ae", "LDX_AB"},
    {"be", "LDX_ABY"},
    // LDY
    {"a0", "LDY_IMM"},
    {"a4", "LDY_ZP"},
    {"b4", "LDY_ZPX"},
    {"ac", "LDY_AB"},
    {"bc", "LDY_ABX"},
    // LSR
    {"4a", "LSR_ACC"},
    {"46", "LSR_ZP"},
    {"56", "LSR_ZPX"},
    {"4e", "LSR_AB"},
    {"5e", "LSR_ABX"},
    // NOP
    {"ea", "NOP_IMP"},
    // ORA
    {"09", "ORA_IMM"},
    {"05", "ORA_ZP"},
    {"15", "ORA_ZPX"},
    {"0d", "ORA_AB"},
    {"1d", "ORA_ABX"},
    {"19", "ORA_ABY"},
    {"01", "ORA_IDX"},
    {"11", "ORA_IDY"},
    // PHA
    {"48", "PHA_IMP"},
    // PHP
    {"08", "PHP_IMP"},
    // PLA
    {"68", "PLA_IMP"},
    // PLP
    {"28", "PLP_IMP"},
    // ROL
    {"2a", "ROL_ACC"},
    {"26", "ROL_ZP"},
    {"36", "ROL_ZPX"},
    {"2e", "ROL_AB"},
    {"3e", "ROL_ABX"},
    // ROR
    {"6a", "ROR_ACC"},
    {"66", "ROR_ZP"},
    {"76", "ROR_ZPX"},
    {"6e", "ROR_AB"},
    {"7e", "ROR_ABX"},
    // RTI
    {"40", "RTI_IMP"},
    // RTS
    {"60", "RTS_IMP"},
    // SBC
    {"e9", "SBC_IMM"},
    {"e5", "SBC_ZP"},
    {"f5", "SBC_ZPX"},
    {"ed", "SBC_AB"},
    {"fd", "SBC_ABX"},
    {"f9", "SBC_ABY"},
    {"e1", "SBC_IDX"},
    {"f1", "SBC_IDY"},
    // SEC
    {"38", "SEC_IMP"},
    // SED
    {"f8", "SED_IMP"},
    // SEI
    {"78", "SEI_IMP"},
    // STA
    {"85", "STA_ZP"},
    {"95", "STA_ZPX"},
    {"8d", "STA_AB"},
    {"9d", "STA_ABX"},
    {"99", "STA_ABY"},
    {"81", "STA_IDX"},
    {"91", "STA_IDY"},
    // STX
    {"86", "STX_ZP"},
    {"96", "STX_ZPY"},
    {"8e", "STX_AB"},
    // STX
    {"84", "STY_ZP"},
    {"94", "STY_ZPX"},
    {"8c", "STY_AB"},
    // TAX
    {"aa", "TAX_IMP"},
    // TAY
    {"a8", "TAY_IMP"},
    // TSX
    {"ba", "TSX_IMP"},
    // TXA
    {"8a", "TXA_IMP"},
    // TXS
    {"9a", "TXS_IMP"},
    // TYA
    {"98", "TYA_IMP"}
};

inline unordered_set<char> binaryVal {'0', '1'};
inline unordered_set<char> octalVal {'0', '1', '2', '3', '4', '5', '6', '7'};
inline unordered_set<char> decimalVal {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
inline unordered_set<char> hexadecimalVal {'0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
inline unordered_map<string, AddressLabel> labelAddress;
inline unordered_map<string, vector<pair<AddressingMode, Opcode>>> allAddressingModes {
    {"adc", {{Immediate, ADC_IMM}, {ZeroPage, ADC_ZP}, {ZeroPageX, ADC_ZPX}, {Absolute, ADC_AB}, {AbsoluteX, ADC_ABX}, {AbsoluteY, ADC_ABY}, {IndirectX, ADC_IDX}, {IndirectY, ADC_IDY}}},
    {"and", {{Immediate, AND_IMM}, {ZeroPage, AND_ZP}, {ZeroPageX, AND_ZPX}, {Absolute, AND_AB}, {AbsoluteX, AND_ABX}, {AbsoluteY, AND_ABY}, {IndirectX, AND_IDX}, {IndirectY, AND_IDY}}},
    {"asl", {{Accumulator, ASL_ACC}, {ZeroPage, ASL_ZP}, {ZeroPageX, ASL_ZPX}, {Absolute, ASL_AB}, {AbsoluteX, ASL_ABX}}},
    {"bcc", {{Relative, BCC_REL}}},
    {"bcs", {{Relative, BCS_REL}}},
    {"beq", {{Relative, BEQ_REL}}},
    {"bit", {{ZeroPage, BIT_ZP}, {Absolute, BIT_AB}}},
    {"bmi", {{Relative, BMI_REL}}},
    {"bne", {{Relative, BNE_REL}}},
    {"bpl", {{Relative, BPL_REL}}},
    {"brk", {{Implicit, BRK_IMP}}},
    {"bvc", {{Relative, BVC_REL}}},
    {"bvs", {{Relative, BVS_REL}}},
    {"clc", {{Implicit, CLC_IMP}}},
    {"cld", {{Implicit, CLD_IMP}}},
    {"cli", {{Implicit, CLI_IMP}}},
    {"clv", {{Implicit, CLV_IMP}}},
    {"cmp", {{Immediate, CMP_IMM}, {ZeroPage, CMP_ZP}, {ZeroPageX, CMP_ZPX}, {Absolute, CMP_AB}, {AbsoluteX, CMP_ABX}, {AbsoluteY, CMP_ABY}, {IndirectX, CMP_IDX}, {IndirectY, CMP_IDY}}},
    {"cpx", {{Immediate, CPX_IMM}, {ZeroPage, CPX_ZP}, {Absolute, CPX_AB}}},
    {"cpy", {{Immediate, CPY_IMM}, {ZeroPage, CPY_ZP}, {Absolute, CPY_AB}}},
    {"dec", {{ZeroPage, DEC_ZP}, {ZeroPageX, DEC_ZPX}, {Absolute, DEC_AB}, {AbsoluteX, DEC_ABX}}},
    {"dex", {{Implicit, DEX_IMP}}},
    {"dey", {{Implicit, DEY_IMP}}},
    {"eor", {{Immediate, EOR_IMM}, {ZeroPage, EOR_ZP}, {ZeroPageX, EOR_ZPX}, {Absolute, EOR_AB}, {AbsoluteX, EOR_ABX}, {AbsoluteY, EOR_ABY}, {IndirectX, EOR_IDX}, {IndirectY, EOR_IDY}}},
    {"inc", {{ZeroPage, INC_ZP}, {ZeroPageX, INC_ZPX}, {Absolute, INC_AB}, {AbsoluteX, INC_ABX}}},
    {"inx", {{Implicit, INX_IMP}}},
    {"iny", {{Implicit, INY_IMP}}},
    {"jmp", {{Absolute, JMP_AB}, {Indirect, JMP_ID}}},
    {"jsr", {{Absolute, JSR_AB}}},
    {"lda", {{Immediate, LDA_IMM}, {ZeroPage, LDA_ZP}, {ZeroPageX, LDA_ZPX}, {Absolute, LDA_AB}, {AbsoluteX, LDA_ABX}, {AbsoluteY, LDA_ABY}, {IndirectX, LDA_IDX}, {IndirectY, LDA_IDY}}},
    {"ldx", {{Immediate, LDX_IMM}, {ZeroPage, LDX_ZP}, {ZeroPageY, LDX_ZPY}, {Absolute, LDX_AB}, {AbsoluteY, LDX_ABY}}},
    {"ldy", {{Immediate, LDY_IMM}, {ZeroPage, LDY_ZP}, {ZeroPageX, LDY_ZPX}, {Absolute, LDY_AB}, {AbsoluteX, LDY_ABX}}},
    {"lsr", {{Accumulator, LSR_ACC}, {ZeroPage, LSR_ZP}, {ZeroPageX, LSR_ZPX}, {Absolute, LSR_AB}, {AbsoluteX, LSR_ABX}}},
    {"nop", {{Implicit, NOP_IMP}}},
    {"ora", {{Immediate, ORA_IMM}, {ZeroPage, ORA_ZP}, {ZeroPageX, ORA_ZPX}, {Absolute, ORA_AB}, {AbsoluteX, ORA_ABX}, {AbsoluteY, ORA_ABY}, {IndirectX, ORA_IDX}, {IndirectY, ORA_IDY}}},
    {"pha", {{Implicit, PHA_IMP}}},
    {"php", {{Implicit, PHP_IMP}}},
    {"pla", {{Implicit, PLA_IMP}}},
    {"plp", {{Implicit, PLP_IMP}}},
    {"rol", {{Accumulator, ROL_ACC}, {ZeroPage, ROL_ZP}, {ZeroPageX, ROL_ZPX}, {Absolute, ROL_AB}, {AbsoluteX, ROL_ABX}}},
    {"ror", {{Accumulator, ROR_ACC}, {ZeroPage, ROR_ZP}, {ZeroPageX, ROR_ZPX}, {Absolute, ROR_AB}, {AbsoluteX, ROR_ABX}}},
    {"rti", {{Implicit, RTI_IMP}}},
    {"rts", {{Implicit, RTS_IMP}}},
    {"sbc", {{Immediate, SBC_IMM}, {ZeroPage, SBC_ZP}, {ZeroPageX, SBC_ZPX}, {Absolute, SBC_AB}, {AbsoluteX, SBC_ABX}, {AbsoluteY, SBC_ABY}, {IndirectX, SBC_IDX}, {IndirectY, SBC_IDY}}},
    {"sec", {{Implicit, SEC_IMP}}},
    {"sed", {{Implicit, SED_IMP}}},
    {"sei", {{Implicit, SEI_IMP}}},
    {"sta", {{ZeroPage, STA_ZP}, {ZeroPageX, STA_ZPX}, {Absolute, STA_AB}, {AbsoluteX, STA_ABX}, {AbsoluteY, STA_ABY}, {IndirectX, STA_IDX}, {IndirectY, STA_IDY}}},
    {"stx", {{ZeroPage, STX_ZP}, {ZeroPageY, STX_ZPY}, {Absolute, STX_AB}}},
    {"sty", {{ZeroPage, STY_ZP}, {ZeroPageX, STY_ZPX}, {Absolute, STY_AB}}},
    {"tax", {{Implicit, TAX_IMP}}},
    {"tay", {{Implicit, TAY_IMP}}},
    {"tsx", {{Implicit, TSX_IMP}}},
    {"txa", {{Implicit, TXA_IMP}}},
    {"txs", {{Implicit, TXS_IMP}}},
    {"tya", {{Implicit, TYA_IMP}}}
};
inline unordered_map<Byte, string> colors {
    {0x0, "000000"}, // black
    {0x1, "FFFFFF"}, // white
    {0x2, "880000"}, // dark red
    {0x3, "AAFFEE"}, // light cyan
    {0x4, "CC44CC"}, // purple
    {0x5, "00CC55"}, // green
    {0x6, "0000AA"}, // blue
    {0x7, "EEEE77"}, // yellow
    {0x8, "DD8855"}, // orange
    {0x9, "664400"}, // brown
    {0xA, "FF7777"}, // pink
    {0xB, "333333"}, // dark gray
    {0xC, "777777"}, // gray
    {0xD, "AAFF66"}, // light gre
    {0xE, "0088FF"}, // sky blue
    {0xF, "BBBBBB"} // light gray
};

#endif
