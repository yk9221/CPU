#ifndef CPU_H
#define CPU_H

#include "globals.h"
#include "memory.h"

class CPU {
private:
    Memory* memory;
    Byte currentValue;
    Word currentAddress;
    Byte cycles;
    Byte pageBoundaryCrossed;
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
    void pushByte(Byte value);
    void pushWord(Word value);
    Byte popByte();
    Word popWord();

    Byte getFlag(int flag);
    void setFlag(int flag);
    void clearFlag(int flag);
    void assignFlag(int flag, bool set);

    void printRegisters();
    void printFlags();
    void printMemory();
    void printColorBlock(int r, int g, int b);
    void clearScreen();
    void printScreen();
};

#endif
