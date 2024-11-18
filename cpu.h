#ifndef CPU_H
#define CPU_H

#include "globals.h"
#include "memory.h"

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
    Label
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

class CPU {
private:
    Memory* memory;
    Byte currentValue;
    Word currentAddress;
    Byte cycles;
    bool pageBoundaryCrossed;
    AddressingMode addressingMode;

public:
    // Registers
    Word PC;
    Byte SP;
    Byte A;
    Byte X;
    Byte Y;
    Byte PS;

    CPU();
    ~CPU();

    // Adressing Modes
    void AM_IMP(); void AM_ACC(); void AM_IMM(); void AM_ZP(); void AM_ZPX(); void AM_ZPY(); void AM_REL();
    void AM_AB(); void AM_ABX(); void AM_ABY(); void AM_ID(); void AM_IDX(); void AM_IDY();

    // Instructions
    void I_ADC(); void I_AND(); void I_ASL(); void I_BCC(); void I_BCS(); void I_BEQ(); void I_BIT();
    void I_BMI(); void I_BNE(); void I_BPL(); void I_BRK(); void I_BVC(); void I_BVS(); void I_CLC();
    void I_CLD(); void I_CLI(); void I_CLV(); void I_CMP(); void I_CPX(); void I_CPY(); void I_DEC();
    void I_DEX(); void I_DEY(); void I_EOR(); void I_INC(); void I_INX(); void I_INY(); void I_JMP();
    void I_JSR(); void I_LDA(); void I_LDX(); void I_LDY(); void I_LSR(); void I_NOP(); void I_ORA();
    void I_PHA(); void I_PHP(); void I_PLA(); void I_PLP(); void I_ROL(); void I_ROR(); void I_RTI();
    void I_RTS(); void I_SBC(); void I_SEC(); void I_SED(); void I_SEI(); void I_STA(); void I_STX();
    void I_STY(); void I_TAX(); void I_TAY(); void I_TSX(); void I_TXA(); void I_TXS(); void I_TYA();

    void run();
    bool instructions();
    Byte read(Word address);
    void write(Word address, Byte value);
    void push(Byte value);
    Byte pop();

    Byte getFlag(int flag);
    void setFlag(int flag);
    void clearFlag(int flag);
    void assignFlag(int flag, bool set);

    void printRegisters();
    void printFlags();
    void printMemory();
};

#endif
