#include "cpu.h"

CPU::CPU() {
    memory = new Memory();
    PC = 0x0600;
    SP = 0xFF;
    A = 0x00;
    X = 0x00;
    Y = 0x00;

    clearFlag(CARRY_BIT);
    clearFlag(ZERO_BIT);
    clearFlag(INTERRUPT_BIT);
    clearFlag(DECIMAL_BIT);
    clearFlag(BREAK_BIT);
    setFlag(UNUSED_BIT);
    clearFlag(OVERFLOW_BIT);
    clearFlag(NEGATIVE_BIT);
}
CPU::~CPU() {
    delete memory;
}

void CPU::AM_IMP() {
    addressingMode = Implicit;
}

void CPU::AM_ACC() {
    addressingMode = Accumulator;
}

void CPU::AM_IMM() {
    currentAddress = PC++;
    currentValue = read(currentAddress);
    addressingMode = Immediate;
}

void CPU::AM_ZP() {
    currentAddress = read(PC++);
    currentValue = read(currentAddress);
    addressingMode = ZeroPage;
}

void CPU::AM_ZPX() {
    currentAddress = read(PC++) + X;
    currentValue = read(currentAddress);
    addressingMode = ZeroPageX;
}

void CPU::AM_ZPY() {
    currentAddress = read(PC++) + Y;
    currentValue = read(currentAddress);
    addressingMode = ZeroPageY;
}

void CPU::AM_REL() {
    addressingMode = Relative;
}

void CPU::AM_AB() {
    Byte lower = read(PC++);
    Byte upper = read(PC++);
    currentAddress = (upper << 8) | lower;
    currentValue = read(currentAddress);
    addressingMode = Absolute;
}

void CPU::AM_ABX() {
    Byte lower = read(PC++);
    Byte upper = read(PC++);
    currentAddress = ((upper << 8) | lower) + X;
    if((currentAddress & 0xFF00) != ((currentAddress - X) & 0xFF00)) {
        pageBoundaryCrossed = true;
    }
    currentValue = read(currentAddress);
    addressingMode = AbsoluteX;
}

void CPU::AM_ABY() {
    Byte lower = read(PC++);
    Byte upper = read(PC++);
    currentAddress = ((upper << 8) | lower) + Y;
    if((currentAddress & 0xFF00) != ((currentAddress - Y) & 0xFF00)) {
        pageBoundaryCrossed = true;
    }
    currentValue = read(currentAddress);
    addressingMode = AbsoluteY;
}

void CPU::AM_ID() {
    Byte lower = read(PC++);
    Byte upper = read(PC++);
    currentAddress = ((upper << 8) | lower);
    currentValue = read(currentAddress);
    addressingMode = Indirect;
}

void CPU::AM_IDX() {
    Byte lower = read(read(PC) + X);
    Byte upper = read(read(PC++) + X + 1);
    currentAddress = ((upper << 8) | lower);
    currentValue = read(currentAddress);
    addressingMode = IndirectX;
}

void CPU::AM_IDY() {
    Byte lower = read(read(PC));
    Byte upper = read(read(PC++) + 1);
    currentAddress = ((upper << 8) | lower) + Y;
    currentValue = read(currentAddress);
    addressingMode = IndirectY;
}


void CPU::I_ADC() {
    Byte sum = A + currentValue + getFlag(CARRY_BIT);
    assignFlag(CARRY_BIT, sum < A);
    assignFlag(ZERO_BIT, sum == 0);
    assignFlag(OVERFLOW_BIT, ((A ^ sum) & ~(A ^ currentValue) & 0x80) >> NEGATIVE_BIT == 1);
    assignFlag(NEGATIVE_BIT, (sum & 0x80) >> NEGATIVE_BIT == 1);
    A = sum;
}

void CPU::I_AND() {
    A &= currentValue;
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::I_ASL() {

}

void CPU::I_BCC() {

}

void CPU::I_BCS() {

}

void CPU::I_BEQ() {

}

void CPU::I_BIT() {

}

void CPU::I_BMI() {

}

void CPU::I_BNE() {

}

void CPU::I_BPL() {

}

void CPU::I_BRK() {

}

void CPU::I_BVC() {

}

void CPU::I_BVS() {

}

void CPU::I_CLC() {
    clearFlag(CARRY_BIT);
}

void CPU::I_CLD() {
    clearFlag(DECIMAL_BIT);
}

void CPU::I_CLI() {
    clearFlag(INTERRUPT_BIT);
}

void CPU::I_CLV() {
    clearFlag(OVERFLOW_BIT);
}

void CPU::I_CMP() {

}

void CPU::I_CPX() {

}

void CPU::I_CPY() {

}

void CPU::I_DEC() {

}

void CPU::I_DEX() {

}

void CPU::I_DEY() {

}

void CPU::I_EOR() {

}

void CPU::I_INC() {

}

void CPU::I_INX() {
    X++;
    assignFlag(ZERO_BIT, X == 0);
    assignFlag(NEGATIVE_BIT, (X & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::I_INY() {
    Y++;
    assignFlag(ZERO_BIT, Y == 0);
    assignFlag(NEGATIVE_BIT, (Y & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::I_JMP() {
    PC = currentAddress;
}

void CPU::I_JSR() {
    push(PC - 1);
    PC = currentAddress;
}

void CPU::I_LDA() {
    A = currentValue;
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::I_LDX() {
    X = currentValue;
    assignFlag(ZERO_BIT, X == 0);
    assignFlag(NEGATIVE_BIT, (X & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::I_LDY() {
    Y = currentValue;
    assignFlag(ZERO_BIT, Y == 0);
    assignFlag(NEGATIVE_BIT, (Y & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::I_LSR() {

}

void CPU::I_NOP() {

}

void CPU::I_ORA() {

}

void CPU::I_PHA() {

}

void CPU::I_PHP() {

}

void CPU::I_PLA() {

}

void CPU::I_PLP() {

}

void CPU::I_ROL() {

}

void CPU::I_ROR() {

}

void CPU::I_RTI() {

}

void CPU::I_RTS() {

}

void CPU::I_SBC() {

}

void CPU::I_SEC() {
    setFlag(CARRY_BIT);
}

void CPU::I_SED() {
    setFlag(DECIMAL_BIT);
}

void CPU::I_SEI() {
    setFlag(INTERRUPT_BIT);
}

void CPU::I_STA() {
    write(currentAddress, A);
}

void CPU::I_STX() {
    write(currentAddress, X);
}

void CPU::I_STY() {
    write(currentAddress, Y);
}

void CPU::I_TAX() {
    X = A;
    assignFlag(ZERO_BIT, X == 0);
    assignFlag(NEGATIVE_BIT, (X & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::I_TAY() {
    Y = A;
    assignFlag(ZERO_BIT, Y == 0);
    assignFlag(NEGATIVE_BIT, (Y & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::I_TSX() {
    X = SP;
    assignFlag(ZERO_BIT, X == 0);
    assignFlag(NEGATIVE_BIT, (X & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::I_TXA() {
    A = X;
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::I_TXS() {
    SP = X;
}

void CPU::I_TYA() {
    A = Y;
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A & 0x80) >> NEGATIVE_BIT == 1);
}

void CPU::run() {
    while(instructions());
}

bool CPU::instructions() {
    Byte opcode = read(PC++);
    pageBoundaryCrossed = false;
    cycles = 0;

    switch(opcode) {
        // ADC
        case ADC_IMM: {AM_IMM(); I_ADC(); break;}
        case ADC_ZP: {AM_ZP(); I_ADC(); break;}
        case ADC_ZPX: {AM_ZPX(); I_ADC(); break;}
        case ADC_AB: {AM_AB(); I_ADC(); break;}
        case ADC_ABX: {AM_ABX(); I_ADC(); break;}
        case ADC_ABY: {AM_ABY(); I_ADC(); break;}
        case ADC_IDX: {AM_IDX(); I_ADC(); break;}
        case ADC_IDY: {AM_IDY(); I_ADC(); break;}
        // AND
        // ASL
        // BCC
        // BCS
        // BEQ
        // BIT
        // BMI
        // BNE
        // BPL
        // BRK
        case BRK_IMP: {AM_IMP(); I_BRK(); return false;}
        // BVC
        // BVS

        // CLC
        case CLC_IMP: {AM_IMP(); I_CLC(); break;}
        // CLD
        case CLD_IMP: {AM_IMP(); I_CLD(); break;}
        // CLI
        case CLI_IMP: {AM_IMP(); I_CLI(); break;}
        // CLV
        case CLV_IMP: {AM_IMP(); I_CLV(); break;}

        // CMP
        // CPX
        // CPY
        // DEC
        // DEX
        // DEY
        // EOR
        // INC

        // INX
        case INX_IMP: {AM_IMP(); I_INX(); break;}
        // INY
        case INY_IMP: {AM_IMP(); I_INY(); break;}
        // JMP
        case JMP_AB: {AM_AB(); I_JMP(); break;}
        case JMP_ID: {AM_ID(); I_JMP(); break;}
        // JSR
        case JSR_AB: {AM_AB(); I_JSR(); break;}
        // LDA
        case LDA_IMM: {AM_IMM(); I_LDA(); break;}
        case LDA_ZP: {AM_ZP(); I_LDA(); break;}
        case LDA_ZPX: {AM_ZPX(); I_LDA(); break;}
        case LDA_AB: {AM_AB(); I_LDA(); break;}
        case LDA_ABX: {AM_ABX(); I_LDA(); break;}
        case LDA_ABY: {AM_ABY(); I_LDA(); break;}
        case LDA_IDX: {AM_IDX(); I_LDA(); break;}
        case LDA_IDY: {AM_IDY(); I_LDA(); break;}
        // LDX
        case LDX_IMM: {AM_IMM(); I_LDX(); break;}
        case LDX_ZP: {AM_ZP(); I_LDX(); break;}
        case LDX_ZPY: {AM_ZPY(); I_LDX(); break;}
        case LDX_AB: {AM_AB(); I_LDX(); break;}
        case LDX_ABY: {AM_ABY(); I_LDX(); break;}
        // LDY
        case LDY_IMM: {AM_IMM(); I_LDY(); break;}
        case LDY_ZP: {AM_ZP(); I_LDY(); break;}
        case LDY_ZPX: {AM_ZPX(); I_LDY(); break;}
        case LDY_AB: {AM_AB(); I_LDY(); break;}
        case LDY_ABX: {AM_ABX(); I_LDY(); break;}

        // LSR
        // NOP
        // ORA
        // PHA
        // PHP
        // PLA
        // PLP
        // ROL
        // ROR
        // RTI
        // RTS
        // SBC


        // SEC
        case SEC_IMP: {AM_IMP(); I_SEC(); break;}
        // SED
        case SED_IMP: {AM_IMP(); I_SED(); break;}
        // SEI
        case SEI_IMP: {AM_IMP(); I_SEI(); break;}
        // STA
        case STA_ZP: {AM_ZP(); I_STA(); break;}
        case STA_ZPX: {AM_ZPX(); I_STA(); break;}
        case STA_AB: {AM_AB(); I_STA(); break;}
        case STA_ABX: {AM_ABX(); I_STA(); break;}
        case STA_ABY: {AM_ABY(); I_STA(); break;}
        case STA_IDX: {AM_IDX(); I_STA(); break;}
        case STA_IDY: {AM_IDY(); I_STA(); break;}
        // STX
        case STX_ZP: {AM_ZP(); I_STX(); break;}
        case STX_ZPY: {AM_ZPX(); I_STX(); break;}
        case STX_AB: {AM_AB(); I_STX(); break;}
        // STY
        case STY_ZP: {AM_ZP(); I_STY(); break;}
        case STY_ZPX: {AM_ZPX(); I_STY(); break;}
        case STY_AB: {AM_AB(); I_STY(); break;}
        // TAX
        case TAX_IMP: {AM_IMP(); I_TAX(); break;}
        // TAY
        case TAY_IMP: {AM_IMP(); I_TAY(); break;}
        // TSX
        case TSX_IMP: {AM_IMP(); I_TSX(); break;}
        // TXA
        case TXA_IMP: {AM_IMP(); I_TXA(); break;}
        // TXS
        case TXS_IMP: {AM_IMP(); I_TXS(); break;}
        // TYA
        case TYA_IMP: {AM_IMP(); I_TYA(); break;}
    }
    return true;
}

Byte CPU::read(Word address) {
    return memory->read(address);
}

void CPU::write(Word address, Byte value) {
    memory->write(address, value);
}

void CPU::push(Byte value) {
    memory->write((1 << 8) | SP--, value);
}

Byte CPU::pop() {
    return memory->read(((1 << 8) | SP++) + 1);
}

Byte CPU::getFlag(int flag) {
    return (PS & (1 << flag)) >> flag;
}

void CPU::setFlag(int flag) {
    PS |= (1 << flag);
}

void CPU::clearFlag(int flag) {
    PS &= ~(1 << flag);
}

void CPU::assignFlag(int flag, bool set) {
    if(set) {
        setFlag(flag);
    }
    else {
        clearFlag(flag);
    }
}

void CPU::printRegisters() {
    printf("Registers:\n\n");
    printf(" PC  SP A  X  Y  NV-BDIZC\n");
    printf("%04X %02X %02X %02X %02X ", PC, SP, A, X, Y);
    printFlags();
}

void CPU::printFlags() {
    printf("%x", getFlag(NEGATIVE_BIT));
    printf("%x", getFlag(OVERFLOW_BIT));
    printf("%x", getFlag(UNUSED_BIT));
    printf("%x", getFlag(BREAK_BIT));
    printf("%x", getFlag(DECIMAL_BIT));
    printf("%x", getFlag(INTERRUPT_BIT));
    printf("%x", getFlag(ZERO_BIT));
    printf("%x\n", getFlag(CARRY_BIT));
}

void CPU::printMemory() {
    memory->printMemory();
}
