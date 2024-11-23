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
    currentValue = A;
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
    currentAddress = PC;
    currentValue = read(currentAddress);
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
        pageBoundaryCrossed = 1;
    }
    currentValue = read(currentAddress);
    addressingMode = AbsoluteX;
}

void CPU::AM_ABY() {
    Byte lower = read(PC++);
    Byte upper = read(PC++);
    currentAddress = ((upper << 8) | lower) + Y;
    if((currentAddress & 0xFF00) != ((currentAddress - Y) & 0xFF00)) {
        pageBoundaryCrossed = 1;
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
    if((currentAddress & 0xFF00) != ((currentAddress - Y) & 0xFF00)) {
        pageBoundaryCrossed = 1;
    }
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

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageX: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        case AbsoluteX: {cycles = 4 + pageBoundaryCrossed; break;}
        case AbsoluteY: {cycles = 4 + pageBoundaryCrossed; break;}
        case IndirectX: {cycles = 6; break;}
        case IndirectY: {cycles = 5 + pageBoundaryCrossed; break;}
        default: {break;}
    }
}

void CPU::I_AND() {
    A &= currentValue;
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageX: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        case AbsoluteX: {cycles = 4 + pageBoundaryCrossed; break;}
        case AbsoluteY: {cycles = 4 + pageBoundaryCrossed; break;}
        case IndirectX: {cycles = 6; break;}
        case IndirectY: {cycles = 5; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_ASL() {

    switch(addressingMode) {
        case Accumulator: {cycles = 2; break;}
        case ZeroPage: {cycles = 5; break;}
        case ZeroPageX: {cycles = 6; break;}
        case Absolute: {cycles = 6; break;}
        case AbsoluteX: {cycles = 7; break;}
        default: {break;}
    }

}

// TODO
void CPU::I_BCC() {
    if(getFlag(CARRY_BIT) == 0) {
        PC += currentValue;
    }

    switch(addressingMode) {
        case Relative: {
            if(getFlag(CARRY_BIT) == 1) {
                cycles = 3;
            }
            else {
                cycles = 2;
            }

            break;
        }
        default: {
            break;
        }
    }
}

// TODO
void CPU::I_BCS() {

}

// TODO
void CPU::I_BEQ() {

}

// TODO
void CPU::I_BIT() {

}

// TODO
void CPU::I_BMI() {

}

// TODO
void CPU::I_BNE() {

}

// TODO
void CPU::I_BPL() {

}

void CPU::I_BRK() {
    setFlag(BREAK_BIT);

    switch(addressingMode) {
        case Implicit: {cycles = 7; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_BVC() {

}

// TODO
void CPU::I_BVS() {

}

void CPU::I_CLC() {
    clearFlag(CARRY_BIT);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_CLD() {
    clearFlag(DECIMAL_BIT);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_CLI() {
    clearFlag(INTERRUPT_BIT);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_CLV() {
    clearFlag(OVERFLOW_BIT);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_CMP() {

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageX: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        case AbsoluteX: {cycles = 4 + pageBoundaryCrossed; break;}
        case AbsoluteY: {cycles = 4 + pageBoundaryCrossed; break;}
        case IndirectX: {cycles = 6; break;}
        case IndirectY: {cycles = 5 + pageBoundaryCrossed; break;}
        default: {break;}
    }
}

void CPU::I_CPX() {
    assignFlag(CARRY_BIT, X >= currentValue);
    assignFlag(ZERO_BIT, X == currentValue);
    assignFlag(NEGATIVE_BIT, ((X - currentValue) & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case Absolute: {cycles = 4; break;}
        default: {break;}
    }
}

void CPU::I_CPY() {
    assignFlag(CARRY_BIT, Y >= currentValue);
    assignFlag(ZERO_BIT, Y == currentValue);
    assignFlag(NEGATIVE_BIT, ((Y - currentValue) & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case Absolute: {cycles = 4; break;}
        default: {break;}
    }
}

void CPU::I_DEC() {
    Byte dec = currentValue - 1;
    assignFlag(ZERO_BIT, dec == 0);
    assignFlag(NEGATIVE_BIT, (dec & 0x80) >> NEGATIVE_BIT == 1);

    write(currentAddress, dec);

    switch(addressingMode) {
        case ZeroPage: {cycles = 5; break;}
        case ZeroPageX: {cycles = 6; break;}
        case Absolute: {cycles = 6; break;}
        case AbsoluteX: {cycles = 7; break;}
        default: {break;}
    }
}

void CPU::I_DEX() {
    X--;
    assignFlag(ZERO_BIT, (X - 1) == 0);
    assignFlag(NEGATIVE_BIT, ((X - 1) & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_DEY() {
    Y--;
    assignFlag(ZERO_BIT, (Y - 1) == 0);
    assignFlag(NEGATIVE_BIT, ((Y - 1) & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_EOR() {
    A ^= currentValue;
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageX: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        case AbsoluteX: {cycles = 4 + pageBoundaryCrossed; break;}
        case AbsoluteY: {cycles = 4 + pageBoundaryCrossed; break;}
        case IndirectX: {cycles = 6; break;}
        case IndirectY: {cycles = 5 + pageBoundaryCrossed; break;}
        default: {break;}
    }
}

void CPU::I_INC() {
    Byte inc = currentValue + 1;
    assignFlag(ZERO_BIT, inc == 0);
    assignFlag(NEGATIVE_BIT, (inc & 0x80) >> NEGATIVE_BIT == 1);

    write(currentAddress, inc);

    switch(addressingMode) {
        case ZeroPage: {cycles = 5; break;}
        case ZeroPageX: {cycles = 6; break;}
        case Absolute: {cycles = 6; break;}
        case AbsoluteX: {cycles = 7; break;}
        default: {break;}
    }
}

void CPU::I_INX() {
    X++;
    assignFlag(ZERO_BIT, X == 0);
    assignFlag(NEGATIVE_BIT, (X & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_INY() {
    Y++;
    assignFlag(ZERO_BIT, Y == 0);
    assignFlag(NEGATIVE_BIT, (Y & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_JMP() {
    PC = currentAddress;

    switch(addressingMode) {
        case Absolute: {cycles = 3; break;}
        case Indirect: {cycles = 5; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_JSR() {
    push(PC - 1);
    PC = currentAddress;

    switch(addressingMode) {
        case Absolute: {cycles = 6; break;}
        default: {break;}
    }
}

void CPU::I_LDA() {
    A = currentValue;
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageX: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        case AbsoluteX: {cycles = 4 + pageBoundaryCrossed; break;}
        case AbsoluteY: {cycles = 4 + pageBoundaryCrossed; break;}
        case IndirectX: {cycles = 6; break;}
        case IndirectY: {cycles = 5; break;}
        default: {break;}
    }
}

void CPU::I_LDX() {
    X = currentValue;
    assignFlag(ZERO_BIT, X == 0);
    assignFlag(NEGATIVE_BIT, (X & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageY: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        case AbsoluteY: {cycles = 4 + pageBoundaryCrossed; break;}
        default: {break;}
    }
}

void CPU::I_LDY() {
    Y = currentValue;
    assignFlag(ZERO_BIT, Y == 0);
    assignFlag(NEGATIVE_BIT, (Y & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageX: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        case AbsoluteX: {cycles = 4 + pageBoundaryCrossed; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_LSR() {

    switch(addressingMode) {
        case Accumulator: {cycles = 2; break;}
        case ZeroPage: {cycles = 5; break;}
        case ZeroPageX: {cycles = 6; break;}
        case Absolute: {cycles = 7; break;}
        case AbsoluteX: {cycles = 7; break;}
        default: {break;}
    }
}

void CPU::I_NOP() {
    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_ORA() {
    A |= currentValue;
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageX: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        case AbsoluteX: {cycles = 4 + pageBoundaryCrossed; break;}
        case AbsoluteY: {cycles = 4 + pageBoundaryCrossed; break;}
        case IndirectX: {cycles = 6; break;}
        case IndirectY: {cycles = 5 + pageBoundaryCrossed; break;}
        default: {break;}
    }
}

void CPU::I_PHA() {
    push(A);

    switch(addressingMode) {
        case Implicit: {cycles = 3; break;}
        default: {break;}
    }
}

void CPU::I_PHP() {
    push(PS);

    switch(addressingMode) {
        case Implicit: {cycles = 3; break;}
        default: {break;}
    }
}

void CPU::I_PLA() {
    A = pop();
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A &= 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Implicit: {cycles = 4; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_PLP() {

    switch(addressingMode) {
        case Implicit: {cycles = 4; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_ROL() {

    switch(addressingMode) {
        case Accumulator: {cycles = 2; break;}
        case ZeroPage: {cycles = 5; break;}
        case ZeroPageX: {cycles = 6; break;}
        case Absolute: {cycles = 7; break;}
        case AbsoluteX: {cycles = 7; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_ROR() {

    switch(addressingMode) {
        case Accumulator: {cycles = 2; break;}
        case ZeroPage: {cycles = 5; break;}
        case ZeroPageX: {cycles = 6; break;}
        case Absolute: {cycles = 7; break;}
        case AbsoluteX: {cycles = 7; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_RTI() {

    switch(addressingMode) {
        case Implicit: {cycles = 6; break;}
        default: {break;}
    }
}

// TODO
void CPU::I_RTS() {
    PC = pop();

    switch(addressingMode) {
        case Implicit: {cycles = 6; break;}
        default: {break;}
    }
}

void CPU::I_SBC() {
    Byte sum = A - currentValue - (1 - getFlag(CARRY_BIT));
    assignFlag(CARRY_BIT, sum < A);
    assignFlag(ZERO_BIT, sum == 0);
    assignFlag(OVERFLOW_BIT, ((A ^ sum) & ~(A ^ currentValue) & 0x80) >> NEGATIVE_BIT == 1);
    assignFlag(NEGATIVE_BIT, (sum & 0x80) >> NEGATIVE_BIT == 1);
    A = sum;

    switch(addressingMode) {
        case Immediate: {cycles = 2; break;}
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageX: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        case AbsoluteX: {cycles = 4 + pageBoundaryCrossed; break;}
        case AbsoluteY: {cycles = 4 + pageBoundaryCrossed; break;}
        case IndirectX: {cycles = 6; break;}
        case IndirectY: {cycles = 5 + pageBoundaryCrossed; break;}
        default: {break;}
    }
}

void CPU::I_SEC() {
    setFlag(CARRY_BIT);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_SED() {
    setFlag(DECIMAL_BIT);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_SEI() {
    setFlag(INTERRUPT_BIT);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_STA() {
    write(currentAddress, A);

    switch(addressingMode) {
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageX: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        case AbsoluteX: {cycles = 5; break;}
        case AbsoluteY: {cycles = 5; break;}
        case IndirectX: {cycles = 6; break;}
        case IndirectY: {cycles = 6; break;}
        default: {break;}
    }
}

void CPU::I_STX() {
    write(currentAddress, X);

    switch(addressingMode) {
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageY: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        default: {break;}
    }
}

void CPU::I_STY() {
    write(currentAddress, Y);

    switch(addressingMode) {
        case ZeroPage: {cycles = 3; break;}
        case ZeroPageY: {cycles = 4; break;}
        case Absolute: {cycles = 4; break;}
        default: {break;}
    }
}

void CPU::I_TAX() {
    X = A;
    assignFlag(ZERO_BIT, X == 0);
    assignFlag(NEGATIVE_BIT, (X & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_TAY() {
    Y = A;
    assignFlag(ZERO_BIT, Y == 0);
    assignFlag(NEGATIVE_BIT, (Y & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_TSX() {
    X = SP;
    assignFlag(ZERO_BIT, X == 0);
    assignFlag(NEGATIVE_BIT, (X & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_TXA() {
    A = X;
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_TXS() {
    SP = X;

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::I_TYA() {
    A = Y;
    assignFlag(ZERO_BIT, A == 0);
    assignFlag(NEGATIVE_BIT, (A & 0x80) >> NEGATIVE_BIT == 1);

    switch(addressingMode) {
        case Implicit: {cycles = 2; break;}
        default: {break;}
    }
}

void CPU::run() {
    while(instructions()) {
        printMemory();
        printScreen();
    }
}

bool CPU::instructions() {
    Byte opcode = read(PC++);
    pageBoundaryCrossed = false;
    cycles = 0;

    switch(opcode) {
        // ADC
        case ADC_IMM: {AM_IMM(); I_ADC(); return true;}
        case ADC_ZP: {AM_ZP(); I_ADC(); return true;}
        case ADC_ZPX: {AM_ZPX(); I_ADC(); return true;}
        case ADC_AB: {AM_AB(); I_ADC(); return true;}
        case ADC_ABX: {AM_ABX(); I_ADC(); return true;}
        case ADC_ABY: {AM_ABY(); I_ADC(); return true;}
        case ADC_IDX: {AM_IDX(); I_ADC(); return true;}
        case ADC_IDY: {AM_IDY(); I_ADC(); return true;}
        // AND
        case AND_IMM: {AM_IMM(); I_AND(); return true;}
        case AND_ZP: {AM_ZP(); I_AND(); return true;}
        case AND_ZPX: {AM_ZPX(); I_AND(); return true;}
        case AND_AB: {AM_AB(); I_AND(); return true;}
        case AND_ABX: {AM_ABX(); I_AND(); return true;}
        case AND_ABY: {AM_ABY(); I_AND(); return true;}
        case AND_IDX: {AM_IDX(); I_AND(); return true;}
        case AND_IDY: {AM_IDY(); I_AND(); return true;}
        // ASL
        case ASL_ACC: {AM_ACC(); I_ASL(); return true;}
        case ASL_ZP: {AM_ZP(); I_ASL(); return true;}
        case ASL_ZPX: {AM_ZPX(); I_ASL(); return true;}
        case ASL_AB: {AM_AB(); I_ASL(); return true;}
        case ASL_ABX: {AM_ABX(); I_ASL(); return true;}
        // BCC
        case BCC_REL: {AM_REL(); I_BCC(); return true;}
        // BCS
        case BCS_REL: {AM_REL(); I_BCS(); return true;}
        // BEQ
        case BEQ_REL: {AM_REL(); I_BEQ(); return true;}
        // BIT
        case BIT_ZP: {AM_ZP(); I_BIT(); return true;}
        case BIT_AB: {AM_AB(); I_BIT(); return true;}
        // BMI
        case BMI_REL: {AM_REL(); I_BMI(); return true;}
        // BNE
        case BNE_REL: {AM_REL(); I_BNE(); return true;}
        // BPL
        case BPL_REL: {AM_REL(); I_BPL(); return true;}
        // BRK
        case BRK_IMP: {AM_IMP(); I_BRK(); return false;}
        // BVC
        case BVC_REL: {AM_REL(); I_BVC(); return true;}
        // BVS
        case BVS_REL: {AM_REL(); I_BVS(); return true;}
        // CLC
        case CLC_IMP: {AM_IMP(); I_CLC(); return true;}
        // CLD
        case CLD_IMP: {AM_IMP(); I_CLD(); return true;}
        // CLI
        case CLI_IMP: {AM_IMP(); I_CLI(); return true;}
        // CLV
        case CLV_IMP: {AM_IMP(); I_CLV(); return true;}
        // CMP
        case CMP_IMM: {AM_IMM(); I_CMP(); return true;}
        case CMP_ZP: {AM_ZP(); I_CMP(); return true;}
        case CMP_ZPX: {AM_ZPX(); I_CMP(); return true;}
        case CMP_AB: {AM_AB(); I_CMP(); return true;}
        case CMP_ABX: {AM_ABX(); I_CMP(); return true;}
        case CMP_ABY: {AM_ABY(); I_CMP(); return true;}
        case CMP_IDX: {AM_IDX(); I_CMP(); return true;}
        case CMP_IDY: {AM_IDY(); I_CMP(); return true;}
        // CPX
        case CPX_IMM: {AM_IMM(); I_CPX(); return true;}
        case CPX_ZP: {AM_ZP(); I_CPX(); return true;}
        case CPX_AB: {AM_AB(); I_CPX(); return true;}
        // CPY
        case CPY_IMM: {AM_IMM(); I_CPY(); return true;}
        case CPY_ZP: {AM_ZP(); I_CPY(); return true;}
        case CPY_AB: {AM_AB(); I_CPY(); return true;}
        // DEC
        case DEC_ZP: {AM_ZP(); I_DEC(); return true;}
        case DEC_ZPX: {AM_ZPX(); I_DEC(); return true;}
        case DEC_AB: {AM_AB(); I_DEC(); return true;}
        case DEC_ABX: {AM_ABX(); I_DEC(); return true;}
        // DEX
        case DEX_IMP: {AM_IMP(); I_DEX(); return true;}
        // DEY
        case DEY_IMP: {AM_IMP(); I_DEY(); return true;}
        // EOR
        case EOR_IMM: {AM_IMM(); I_EOR(); return true;}
        case EOR_ZP: {AM_ZP(); I_EOR(); return true;}
        case EOR_ZPX: {AM_ZPX(); I_EOR(); return true;}
        case EOR_AB: {AM_AB(); I_EOR(); return true;}
        case EOR_ABX: {AM_ABX(); I_EOR(); return true;}
        case EOR_ABY: {AM_ABY(); I_EOR(); return true;}
        case EOR_IDX: {AM_IDX(); I_EOR(); return true;}
        case EOR_IDY: {AM_IDY(); I_EOR(); return true;}
        // INC
        case INC_ZP: {AM_ZP(); I_INC(); return true;}
        case INC_ZPX: {AM_ZPX(); I_INC(); return true;}
        case INC_AB: {AM_AB(); I_INC(); return true;}
        case INC_ABX: {AM_ABX(); I_INC(); return true;}
        // INX
        case INX_IMP: {AM_IMP(); I_INX(); return true;}
        // INY
        case INY_IMP: {AM_IMP(); I_INY(); return true;}
        // JMP
        case JMP_AB: {AM_AB(); I_JMP(); return true;}
        case JMP_ID: {AM_ID(); I_JMP(); return true;}
        //JSR
        case JSR_AB: {AM_AB(); I_JSR(); return true;}
        // LDA
        case LDA_IMM: {AM_IMM(); I_LDA(); return true;}
        case LDA_ZP: {AM_ZP(); I_LDA(); return true;}
        case LDA_ZPX: {AM_ZPX(); I_LDA(); return true;}
        case LDA_AB: {AM_AB(); I_LDA(); return true;}
        case LDA_ABX: {AM_ABX(); I_LDA(); return true;}
        case LDA_ABY: {AM_ABY(); I_LDA(); return true;}
        case LDA_IDX: {AM_IDX(); I_LDA(); return true;}
        case LDA_IDY: {AM_IDY(); I_LDA(); return true;}
        // LDX
        case LDX_IMM: {AM_IMM(); I_LDX(); return true;}
        case LDX_ZP: {AM_ZP(); I_LDX(); return true;}
        case LDX_ZPY: {AM_ZPY(); I_LDX(); return true;}
        case LDX_AB: {AM_AB(); I_LDX(); return true;}
        case LDX_ABY: {AM_ABY(); I_LDX(); return true;}
        // LDY
        case LDY_IMM: {AM_IMM(); I_LDY(); return true;}
        case LDY_ZP: {AM_ZP(); I_LDY(); return true;}
        case LDY_ZPX: {AM_ZPX(); I_LDY(); return true;}
        case LDY_AB: {AM_AB(); I_LDY(); return true;}
        case LDY_ABX: {AM_ABX(); I_LDY(); return true;}
        // LSR
        case LSR_ACC: {AM_ACC(); I_LSR(); return true;}
        case LSR_ZP: {AM_ZP(); I_LSR(); return true;}
        case LSR_ZPX: {AM_ZPX(); I_LSR(); return true;}
        case LSR_AB: {AM_AB(); I_LSR(); return true;}
        case LSR_ABX: {AM_ABX(); I_LSR(); return true;}
        // NOP
        case NOP_IMP: {AM_IMP(); I_NOP(); return true;}
        // ORA
        case ORA_IMM: {AM_IMM(); I_ORA(); return true;}
        case ORA_ZP: {AM_ZP(); I_ORA(); return true;}
        case ORA_ZPX: {AM_ZPX(); I_ORA(); return true;}
        case ORA_AB: {AM_AB(); I_ORA(); return true;}
        case ORA_ABX: {AM_ABX(); I_ORA(); return true;}
        case ORA_ABY: {AM_ABY(); I_ORA(); return true;}
        case ORA_IDX: {AM_IDX(); I_ORA(); return true;}
        case ORA_IDY: {AM_IDY(); I_ORA(); return true;}
        // PHA
        case PHA_IMP: {AM_IMP(); I_PHA(); return true;}
        // PHP
        case PHP_IMP: {AM_IMP(); I_PHP(); return true;}
        // PLA
        case PLA_IMP: {AM_IMP(); I_PLA(); return true;}
        // PLP
        case PLP_IMP: {AM_IMP(); I_PLP(); return true;}
        // ROL
        case ROL_ACC: {AM_ACC(); I_ROL(); return true;}
        case ROL_ZP: {AM_ZP(); I_ROL(); return true;}
        case ROL_ZPX: {AM_ZPX(); I_ROL(); return true;}
        case ROL_AB: {AM_AB(); I_ROL(); return true;}
        case ROL_ABX: {AM_ABX(); I_ROL(); return true;}
        // ROR
        case ROR_ACC: {AM_ACC(); I_ROR(); return true;}
        case ROR_ZP: {AM_ZP(); I_ROR(); return true;}
        case ROR_ZPX: {AM_ZPX(); I_ROR(); return true;}
        case ROR_AB: {AM_AB(); I_ROR(); return true;}
        case ROR_ABX: {AM_ABX(); I_ROR(); return true;}
        // RTI
        case RTI_IMP: {AM_IMP(); I_RTI(); return true;}
        // RTS
        case RTS_IMP: {AM_IMP(); I_RTS(); return true;}
        // SBC
        case SBC_IMM: {AM_IMM(); I_SBC(); return true;}
        case SBC_ZP: {AM_ZP(); I_SBC(); return true;}
        case SBC_ZPX: {AM_ZPX(); I_SBC(); return true;}
        case SBC_AB: {AM_AB(); I_SBC(); return true;}
        case SBC_ABX: {AM_ABX(); I_SBC(); return true;}
        case SBC_ABY: {AM_ABY(); I_SBC(); return true;}
        case SBC_IDX: {AM_IDX(); I_SBC(); return true;}
        case SBC_IDY: {AM_IDY(); I_SBC(); return true;}
        // SEC
        case SEC_IMP: {AM_IMP(); I_SEC(); return true;}
        // SED
        case SED_IMP: {AM_IMP(); I_SED(); return true;}
        // SEI
        case SEI_IMP: {AM_IMP(); I_SEI(); return true;}
        // STA
        case STA_ZP: {AM_ZP(); I_STA(); return true;}
        case STA_ZPX: {AM_ZPX(); I_STA(); return true;}
        case STA_AB: {AM_AB(); I_STA(); return true;}
        case STA_ABX: {AM_ABX(); I_STA(); return true;}
        case STA_ABY: {AM_ABY(); I_STA(); return true;}
        case STA_IDX: {AM_IDX(); I_STA(); return true;}
        case STA_IDY: {AM_IDY(); I_STA(); return true;}
        // STX
        case STX_ZP: {AM_ZP(); I_STX(); return true;}
        case STX_ZPY: {AM_ZPY(); I_STX(); return true;}
        case STX_AB: {AM_AB(); I_STX(); return true;}
        // STX
        case STY_ZP: {AM_ZP(); I_STY(); return true;}
        case STY_ZPX: {AM_ZPX(); I_STY(); return true;}
        case STY_AB: {AM_AB(); I_STY(); return true;}
        // TAX
        case TAX_IMP: {AM_IMP(); I_TAX(); return true;}
        // TAY
        case TAY_IMP: {AM_IMP(); I_TAY(); return true;}
        // TSX
        case TSX_IMP: {AM_IMP(); I_TSX(); return true;}
        // TXA
        case TXA_IMP: {AM_IMP(); I_TXA(); return true;}
        // TXS
        case TXS_IMP: {AM_IMP(); I_TXS(); return true;}
        // TYA
        case TYA_IMP: {AM_IMP(); I_TYA(); return true;}
        default: {return false;}
    }
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
    printf("Registers:\n");
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

void CPU::printColorBlock(int r, int g, int b) {
    cout << "\033[48;2;" << r << ";" << g << ";" << b << "m  \033[0m";
}

void CPU::clearScreen() {
    cout << "\033[2J\033[H";
}

void CPU::printScreen() {
    int pixel[GRIDSIZE][GRIDSIZE][3];
    for (int y = 0; y < GRIDSIZE; y++) {
        for (int x = 0; x < GRIDSIZE; x++) {
            Word address = y * GRIDSIZE + x + 0x200;
            Byte value = read(address);
            string rgb = colors[value % 0x10];

            pixel[y][x][0] = stoi(rgb.substr(0, 2), nullptr, 16);
            pixel[y][x][1] = stoi(rgb.substr(2, 2), nullptr, 16);
            pixel[y][x][2] = stoi(rgb.substr(4, 2), nullptr, 16);
        }
    }

    for (int y = 0; y < GRIDSIZE; y++) {
        for (int x = 0; x < GRIDSIZE; x++) {
            printColorBlock(pixel[y][x][0], pixel[y][x][1], pixel[y][x][2]);
        }
        std::cout << "\n";
    }
}
