#include "assembler.h"

unordered_set<char> binaryVal {'0', '1'};
unordered_set<char> octalVal {'0', '1', '2', '3', '4', '5', '6', '7'};
unordered_set<char> decimalVal {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
unordered_set<char> hexadecimalVal {'0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
unordered_map<string, AddressLabel> labelAddress;

unordered_set<string> allOpCodes {
    "adc", "and", "asl", "bcc", "bcs", "beq", "bit", "bmi", "bne", "bpl", "brk", "bvc", "bvs", "clc",
    "cld", "cli", "clv", "cmp", "cpx", "cpy", "dec", "dex", "dey", "eor", "inc", "inx", "iny", "jmp",
    "jsr", "lda", "ldx", "ldy", "lsr", "nop", "ora", "pha", "php", "pla", "plp", "rol", "ror", "rti",
    "rts", "sbc", "sec", "sed", "sei", "sta", "stx", "sty", "tax", "tay", "tsx", "txa", "txs", "tya"
};

unordered_map<string, vector<pair<AddressingMode, Opcode>>> allAddressingModes {
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

void printInstruction(vector<string>& vec) {
    cout << "[";
    for(int i = 0; i < vec.size(); i++) {
        if(i < vec.size() - 1) {
            cout << vec[i] << "-";
        }
        else {
            cout << vec[i];
        }
    }
    cout << "]" << endl;
}

string tolower(string str) {
    for(char& c : str) {
        c = tolower(static_cast<unsigned char>(c));
    }

    return str;
}

bool checkIsByte(const string& num) {
    if(checkIsNumber(num) == Radix::ERR) {
        return false;
    }
    else if(checkIsNumber(num) == Radix::BIN) {
        int value = strtol(num.substr(1).c_str(), nullptr, 2);
        if(value <= MAX_BYTE) {
            return true;
        }
        return false;
    }
    else if(checkIsNumber(num) == Radix::OCT) {
        int value = strtol(num.substr(1).c_str(), nullptr, 8);
        if(value <= MAX_BYTE) {
            return true;
        }
        return false;
    }
    else if(checkIsNumber(num) == Radix::DEC) {
        int value = strtol(num.c_str(), nullptr, 10);
        if(value <= MAX_BYTE) {
            return true;
        }
        return false;
    }
    else if(checkIsNumber(num) == Radix::HEX) {
        int value = strtol(num.substr(1).c_str(), nullptr, 16);
        if(value <= MAX_BYTE) {
            return true;
        }
        return false;
    }
    else {
        return false;
    }
}

bool checkIsWord(const string& num) {
    if(checkIsNumber(num) == Radix::ERR) {
        return false;
    }
    else if(checkIsNumber(num) == Radix::BIN) {
        int value = strtol(num.substr(1).c_str(), nullptr, 2);
        if(value <= MAX_WORD) {
            return true;
        }
        return false;
    }
    else if(checkIsNumber(num) == Radix::OCT) {
        int value = strtol(num.substr(1).c_str(), nullptr, 8);
        if(value <= MAX_WORD) {
            return true;
        }
        return false;
    }
    else if(checkIsNumber(num) == Radix::DEC) {
        int value = strtol(num.c_str(), nullptr, 10);
        if(value <= MAX_WORD) {
            return true;
        }
        return false;
    }
    else if(checkIsNumber(num) == Radix::HEX) {
        int value = strtol(num.substr(1).c_str(), nullptr, 16);
        if(value <= MAX_WORD) {
            return true;
        }
        return false;
    }
    else {
        return false;
    }
}

Radix checkIsNumber(const string& num) {
    if(num[0] == '%') {
        for(int i = 1; i < num.size(); i++) {
            if(binaryVal.find(num[i]) == binaryVal.end()) {
                return Radix::ERR;
            }
        }
        return Radix::BIN;
    }
    else if(num[0] == '@') {
        for(int i = 1; i < num.size(); i++) {
            if(octalVal.find(num[i]) == octalVal.end()) {
                return Radix::ERR;
            }
        }
        return Radix::OCT;
    }
    else if(num[0] == '$') {
        for(int i = 1; i < num.size(); i++) {
            if(hexadecimalVal.find(num[i]) == hexadecimalVal.end()) {
                return Radix::ERR;
            }
        }
        return Radix::HEX;
    }
    else {
        for(int i = 0; i < num.size(); i++) {
            if(decimalVal.find(num[i]) == decimalVal.end()) {
                return Radix::ERR;
            }
        }
        return Radix::DEC;
    }

    return Radix::ERR;
}

Assembler::Assembler() {}
Assembler::~Assembler() {}

int getModeIndex(string& opcode, AddressingMode& mode) {
    for(int i = 0; i < allAddressingModes[opcode].size(); i++) {
        if(allAddressingModes[opcode][i].first == mode) {
            return i;
        }
    }
    return -1;
}

bool checkModeExists(string& opcode, AddressingMode mode) {
    for(int i = 0; i < allAddressingModes[opcode].size(); i++) {
        if(allAddressingModes[opcode][i].first == mode) {
            return true;
        }
    }
    return false;
}

void setErrorModeIfMissing(string& opcode, AddressingMode& mode) {
    if(!checkModeExists(opcode, mode)) {
        mode = Error;
    }
}

Byte getModeBytes(AddressingMode mode) {
    switch(mode) {
        case Implicit: {return ImplicitBytes;}
        case Accumulator: {return AccumulatorBytes;}
        case Immediate: {return ImmediateBytes;}
        case ZeroPage: {return ZeroPageBytes;}
        case ZeroPageX: {return ZeroPageXBytes;}
        case ZeroPageY: {return ZeroPageYBytes;}
        case Relative: {return RelativeBytes;}
        case Absolute: {return AbsoluteBytes;}
        case AbsoluteX: {return AbsoluteXBytes;}
        case AbsoluteY: {return AbsoluteYBytes;}
        case Indirect: {return IndirectBytes;}
        case IndirectX: {return IndirectXBytes;}
        case IndirectY: {return IndirectYBytes;}
        case Error: {return ErrorBytes;}
        case Label: {return LabelBytes;}
        case Define: {return DefineByte;}
        default: {return ErrorBytes;}
    }
}

void appendOpcode(string& opcode, AddressingMode& mode, vector<Byte>& conversion, Word& PC) {
    int modeIndex = getModeIndex(opcode, mode);
    if(modeIndex != -1) {
        conversion.push_back(allAddressingModes[opcode][modeIndex].second);
    }
}

string removeParenthesisOperands(string& operands, AddressingMode& mode) {
    if(mode == Indirect || mode == IndirectY) {
        return operands.substr(1, operands.size() - 2);
    }
    else if(mode == IndirectX) {
        return operands.substr(1);
    }
    return operands;
}

Word stringToNum(string& num) {
    Word value = 0x0;

    if(checkIsNumber(num) == Radix::BIN) {
        value = strtol(num.substr(1).c_str(), nullptr, 2);
    }
    else if(checkIsNumber(num) == Radix::OCT) {
        value = strtol(num.substr(1).c_str(), nullptr, 8);
    }
    else if(checkIsNumber(num) == Radix::DEC) {
        value = strtol(num.c_str(), nullptr, 10);
    }
    else if(checkIsNumber(num) == Radix::HEX) {
        value = strtol(num.substr(1).c_str(), nullptr, 16);
    }

    return value;
}

void appendOperands(string& opcode, string& operands, AddressingMode& mode, vector<Byte>& conversion, Word& PC) {
    Word value = 0x0;
    string num;

    if(labelAddress.find(operands) != labelAddress.end() || (labelAddress.find(operands.substr(1)) != labelAddress.end() && operands[0] == '#')) {
        if(mode == Absolute || mode == AbsoluteX || mode == AbsoluteY) {
            value = labelAddress[operands].value;
        }
        else {
            if(labelAddress[operands].mode == Define) {
                value = labelAddress[operands].value;
            }
            else if(labelAddress[operands.substr(1)].mode == Define && operands[0] == '#') {
                value = labelAddress[operands.substr(1)].value;
            }
            else {
                value = labelAddress[operands].value - (PC + 1);

                if(((value >> 8) & 0xFF) == 0xFF) {
                    value = value & 0xFF;
                }
            }

        }


    }
    else {
        if(operands[0] == '#') {
            num = operands.substr(1);
        }
        else {
            num = operands;
        }

        value = stringToNum(num);
    }

    Byte lower = value & 0xFF;
    conversion.push_back(lower);

    if(value > MAX_BYTE) {
        Byte upper = (value >> 8) & 0xFF;
        conversion.push_back(upper);
    }
}

void checkImplicit(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS

    if(ins.size() == 1) {
        mode = Implicit;
        setErrorModeIfMissing(opcode, mode);
    }
}

void checkAccumulator(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS A

    if(ins.size() == 2) {
        string operand = ins[1];
        if(operand == "a") {
            mode = Accumulator;
            setErrorModeIfMissing(opcode, mode);
        }
    }
}

void checkImmediate(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS #$FF
    // INS #LABEL

    if(ins.size() == 2) {
        string operand = ins[1];
        if(operand[0] == '#') {
            operand = operand.substr(1);

            if(checkIsByte(operand)) {
                mode = Immediate;
                setErrorModeIfMissing(opcode, mode);
            }
            else if(labelAddress.find(operand) != labelAddress.end() && checkModeExists(opcode, Immediate)) {
                if(labelAddress[operand].value <= MAX_BYTE) {
                    mode = Immediate;
                }
            }
        }
    }
}

void checkZeroPage(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS $FF
    // INS LABEL

    if(ins.size() == 2) {
        string operand = ins[1];

        if(checkIsByte(operand)) {
            mode = ZeroPage;
            setErrorModeIfMissing(opcode, mode);
        }
        else if(labelAddress.find(operand) != labelAddress.end() && checkModeExists(opcode, ZeroPage)) {
            mode = ZeroPage;
        }
    }
}

void checkZeroPageX(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS $FF, X
    // INS LABEL, X

    if(ins.size() == 3) {
        string operand = ins[1];
        string reg = ins[2];

        if(checkIsByte(operand) && reg == "x") {
            mode = ZeroPageX;
            setErrorModeIfMissing(opcode, mode);
        }
        else if(labelAddress.find(operand) != labelAddress.end() && reg == "x" && checkModeExists(opcode, ZeroPageX)) {
            mode = ZeroPageX;
        }
    }
}

void checkZeroPageY(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS $FF, Y
    // INS LABEL, Y

    if(ins.size() == 3) {
        string operand = ins[1];
        string reg = ins[2];

        if(checkIsByte(operand) && reg == "y") {
            mode = ZeroPageY;
            setErrorModeIfMissing(opcode, mode);
        }
        else if(labelAddress.find(operand) != labelAddress.end() && reg == "y" && checkModeExists(opcode, ZeroPageY)) {
            mode = ZeroPageY;
        }
    }
}

void checkRelative(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS LABEL

    if(ins.size() == 2) {
        string operand = ins[1];

        if(labelAddress.find(operand) != labelAddress.end() && checkModeExists(opcode, Relative)) {
            mode = Relative;
        }
    }
}

void checkAbsolute(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS $FFFF
    // INS LABEL

    if(ins.size() == 2 && mode != ZeroPage) {
        string operand = ins[1];
        if(checkIsWord(operand)) {
            mode = Absolute;
            setErrorModeIfMissing(opcode, mode);
        }
        else if(labelAddress.find(operand) != labelAddress.end() && checkModeExists(opcode, Absolute)) {
            mode = Absolute;
        }
    }
}

void checkAbsoluteX(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS $FFFF, X
    // INS LABEL, X

    if(ins.size() == 3 && mode != ZeroPageX) {
        string operand = ins[1];
        string reg = ins[2];

        if(checkIsWord(operand) && reg == "x") {
            mode = AbsoluteX;
            setErrorModeIfMissing(opcode, mode);
        }
        else if(labelAddress.find(operand) != labelAddress.end() && reg == "x" && checkModeExists(opcode, AbsoluteX)) {
            mode = AbsoluteX;
        }
    }
}

void checkAbsoluteY(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS $FFFF, Y
    // INS LABEL, Y

    if(ins.size() == 3 && mode != ZeroPageY) {
        string operand = ins[1];
        string reg = ins[2];

        if(checkIsWord(operand) && reg == "y") {
            mode = AbsoluteY;
            setErrorModeIfMissing(opcode, mode);
        }
        else if(labelAddress.find(operand) != labelAddress.end() && reg == "y" && checkModeExists(opcode, AbsoluteY)) {
            mode = AbsoluteY;
        }
    }
}

void checkIndirect(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS ($FFFF)
    // INS (LABEL)

    if(ins.size() == 2) {
        string operand = ins[1];

        if(operand[0] == '(' && operand[operand.size() - 1] == ')') {
            operand = operand.substr(1, operand.length() - 2);
            if(checkIsWord(operand)) {
                mode = Indirect;
                setErrorModeIfMissing(opcode, mode);
            }
            else if(labelAddress.find(operand) != labelAddress.end() && checkModeExists(opcode, Indirect)) {
                mode = Indirect;
            }
        }
    }
}

void checkIndirectX(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS ($FFFF, X)
    // INS (LABEL, X)

    if(ins.size() == 3) {
        string operand = ins[1];
        string reg = ins[2];

        if(operand[0] == '(' && reg[reg.size() - 1] == ')') {
            operand = operand.substr(1);
            reg = reg.substr(0, 1);
            if(checkIsByte(operand) && reg == "x") {
                mode = IndirectX;
                setErrorModeIfMissing(opcode, mode);
            }
            else if(labelAddress.find(operand) != labelAddress.end() && reg == "x" && checkModeExists(opcode, IndirectX)) {
                mode = IndirectX;
            }
        }
    }
}

void checkIndirectY(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS ($FFFF), Y
    // INS (LABEL), Y

    if(ins.size() == 3) {
        string operand = ins[1];
        string reg = ins[2];

        if(operand[0] == '(' && operand[operand.size() - 1] == ')') {
            operand = operand.substr(1, operand.length() - 2);

            if(checkIsByte(operand) && reg == "y") {
                mode = IndirectY;
                setErrorModeIfMissing(opcode, mode);
            }
            else if(labelAddress.find(operand) != labelAddress.end() && reg == "y" && checkModeExists(opcode, IndirectY)) {
                mode = IndirectY;
            }
        }
    }
}

void checkLabel(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // LABEL:

    if(ins.size() == 1)
        if(opcode[opcode.size() - 1] == ':') {
            mode = Label;
        }
}

void checkDefine(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // define var $FFFF

    if(ins.size() == 3) {
        if(opcode == "define") {
            string operand = ins[2];
            if(checkIsWord(operand)) {
                mode = Define;
            }
        }
    }
}

void checkModes(vector<string>& ins, AddressingMode& mode, string& opcode) {
    mode = Error;
    checkImplicit(ins, mode, opcode);
    checkAccumulator(ins, mode, opcode);
    checkImmediate(ins, mode, opcode);
    checkZeroPage(ins, mode, opcode);
    checkZeroPageX(ins, mode, opcode);
    checkZeroPageY(ins, mode, opcode);
    checkAbsolute(ins, mode, opcode);
    checkAbsoluteX(ins, mode, opcode);
    checkAbsoluteY(ins, mode, opcode);
    checkRelative(ins, mode, opcode);
    checkIndirect(ins, mode, opcode);
    checkIndirectX(ins, mode, opcode);
    checkIndirectY(ins, mode, opcode);
    checkLabel(ins, mode, opcode);
    checkDefine(ins, mode, opcode);

    printf("Mode: ");
    switch(mode) {
        case Implicit: {printf("Implicit\n"); break;}
        case Accumulator: {printf("Accumulator\n"); break;}
        case Immediate: {printf("Immediate\n"); break;}
        case ZeroPage: {printf("ZeroPage\n"); break;}
        case ZeroPageX: {printf("ZeroPageX\n"); break;}
        case ZeroPageY: {printf("ZeroPageY\n"); break;}
        case Relative: {printf("Relative\n"); break;}
        case Absolute: {printf("Absolute\n"); break;}
        case AbsoluteX: {printf("AbsoluteX\n"); break;}
        case AbsoluteY: {printf("AbsoluteY\n"); break;}
        case Indirect: {printf("Indirect\n"); break;}
        case IndirectX: {printf("IndirectX\n"); break;}
        case IndirectY: {printf("IndirectY\n"); break;}
        case Error: {printf("Error\n"); break;}
        case Label: {printf("Label\n"); break;}
        case Define: {printf("Define\n"); break;}
        default: {printf("Error\n"); break;}
    }
}

void insertLabels(vector<string>& ins) {
    string opcode = ins[0];
    AddressingMode mode = Error;

    checkModes(ins, mode, opcode);

    if(mode == Label) {
        opcode = opcode.substr(0, opcode.size() - 1);
        labelAddress.insert({opcode, {0x0, Label}});
    }
    else if(mode == Define) {
        string varName = ins[1];
        string operand = ins[2];
        labelAddress.insert({varName, {stringToNum(operand), Define}});
    }
}

void parseLabels(vector<string>& ins, Word& PC) {
    string opcode = ins[0];
    AddressingMode mode = Error;

    checkModes(ins, mode, opcode);

    PC += getModeBytes(mode);

    if(mode == Label) {
        labelAddress[opcode.substr(0, opcode.size() - 1)].value = PC;
    }
}

void parseOperands(vector<string>& ins, Word& PC, vector<Byte>& conversion) {
    string opcode = ins[0];
    AddressingMode mode;

    checkModes(ins, mode, opcode);
    appendOpcode(opcode, mode, conversion, PC);

    if(ins.size() > 1 && mode != Define && mode != Error) {
        string operands = removeParenthesisOperands(ins[1], mode);
        appendOperands(opcode, operands, mode, conversion, PC);
    }
}

void Assembler::parse(CPU* cpu, const string& filename) {
    Word PC = cpu->PC;

    ifstream file(filename);
    string line;
    unordered_set<char> delim{' ', ','};
    vector<vector<string>> parsed;

    while(getline(file, line)) {
        vector<string> lineParsed = split(line, delim);
        if(!lineParsed.empty()) {
            parsed.push_back(lineParsed);
        }
    }

    for(int i = 0; i < parsed.size(); i++) {
        insertLabels(parsed[i]);
    }

    printf("---------------------------------------------------------------\n");

    for(int i = 0; i < parsed.size(); i++) {
        parseLabels(parsed[i], PC);
    }
    PC = cpu->PC;

    printf("---------------------------------------------------------------\n");

    for(int i = 0; i < parsed.size(); i++) {
        vector<Byte> conversion;

        printInstruction(parsed[i]);
        parseOperands(parsed[i], PC, conversion);
        addToMemory(conversion, cpu, PC);
    }

    file.close();
}

vector<string> Assembler::split(const string& str, const unordered_set<char>& delimiter) {
    vector<string> tokens;
    string buffer;
    bool comment = false;

    for(char c : str) {
        if(comment) {
            continue;
        }
        if(delimiter.find(c) != delimiter.end()) {
            if(!buffer.empty()) {
                tokens.push_back(tolower(buffer));
                buffer.clear();
            }
        }
        else if(c == ';') {
            comment = true;
        }
        else {
            buffer += c;
        }
    }
    if(!buffer.empty()) {
        tokens.push_back(tolower(buffer));
        buffer.clear();
    }
    return tokens;
}

void Assembler::addToMemory(vector<Byte>& instructions, CPU* cpu, Word& PC) {
    for(auto ins : instructions) {
        printf("%x: %x\n", PC, ins);
        cpu->write(PC, ins);
        PC++;
    }
}
