#include "assembler.h"

unordered_set<char> binaryVal {'0', '1'};
unordered_set<char> octalVal {'0', '1', '2', '3', '4', '5', '6', '7'};
unordered_set<char> decimalVal {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
unordered_set<char> hexadecimalVal {'0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
unordered_map<string, Word> labelAddress;

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

Byte stringToHex(string str) {
    Byte hex = 0;
    for(char c : str) {
        if(isdigit(c)) {
            hex <<= 4;
            hex |= (c - '0');
        }
        else if(isalpha(c)) {
            hex <<= 4;
            if(c >= 'a' && c <= 'f') {
                hex |= (c - 'a' + 10);
            }
        }
    }
    return hex;
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

bool checkModeExists(string& opcode, AddressingMode& mode) {
    for(int i = 0; i < allAddressingModes[opcode].size(); i++) {
        if(allAddressingModes[opcode][i].first == mode) {
            return true;
        }
    }
    return false;
}

void changeModeToError(string& opcode, AddressingMode& mode) {
    if(!checkModeExists(opcode, mode)) {
        mode = Error;
    }
}

void appendOpcode(string& opcode, AddressingMode& mode, vector<Byte>& conversion) {
    int modeIndex = getModeIndex(opcode, mode);
    if(modeIndex != -1) {
        conversion.push_back(allAddressingModes[opcode][modeIndex].second);
    }
}



void appendOperands(string& opcode, string& operands, AddressingMode& mode, vector<Byte>& conversion) {
    string num;
    if(operands[0] == '#') {
        num = operands.substr(1);
    }
    else {
        num = operands;
    }

    Byte value;
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
    conversion.push_back(value);
}


vector<Byte> parseOperands(vector<string>& ins) {
    string opcode = ins[0];
    AddressingMode mode;
    vector<Byte> conversion;

    if(allOpCodes.find(opcode) == allOpCodes.end()) {
        if(opcode.empty()) {
            mode = Error;
        }
        else if(opcode[opcode.size() - 1] == ':') {
            if(labelAddress.find(opcode.substr(0, opcode.size() - 1)) == labelAddress.end()) {
                labelAddress.insert({opcode.substr(0, opcode.size() - 1), 0x0});
                mode = Label;
            }
            else {
                mode = Error;
            }
        }
        else {
            mode = Error;
        }
    }
    else {
        if(ins.size() == 1) {
            mode = Implicit;
            changeModeToError(opcode, mode);
            appendOpcode(opcode, mode, conversion);

        }
        else if(ins.size() == 2) {
            string operand = ins[1];

            if(operand.empty()) {
                mode = Error;
            }
            else if(operand == "a" || operand == "x" || operand == "y") {
                mode = Accumulator;
                changeModeToError(opcode, mode);
                appendOpcode(opcode, mode, conversion);
            }
            else if(operand[0] == '#') {
                if(checkIsByte(operand.substr(1))) {
                    mode = Immediate;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else {
                    mode = Error;
                }
            }
            else if(operand[0] == '(' && operand[operand.size() - 1] == ')') {
                if(checkIsWord(operand.substr(1, operand.length() - 2))) {
                    mode = Indirect;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else if(labelAddress.find(operand.substr(1, operand.length() - 2)) != labelAddress.end()) {
                    mode = Indirect;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else {
                    mode = Error;
                }
            }
            else {
                if(checkIsByte(operand)) {
                    mode = ZeroPage;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else if(checkIsWord(operand)) {
                    mode = Absolute;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else if(labelAddress.find(operand) != labelAddress.end()) {
                    if(labelAddress[operand] <= MAX_BYTE) {
                        mode = ZeroPage;
                        if(!checkModeExists(opcode, mode)) {
                            mode = Absolute;
                            if(!checkModeExists(opcode, mode)) {
                                mode = Relative;
                            }
                        }
                        changeModeToError(opcode, mode);
                        appendOpcode(opcode, mode, conversion);
                        appendOperands(opcode, operand, mode, conversion);
                    }
                    else if(labelAddress[operand] <= MAX_WORD) {
                        mode = Absolute;
                        changeModeToError(opcode, mode);
                        appendOpcode(opcode, mode, conversion);
                        appendOperands(opcode, operand, mode, conversion);
                    }
                    else {
                        mode = Error;
                    }
                }
                else {
                    mode = Error;
                }
            }
        }
        else if(ins.size() == 3) {
            string operand = ins[1];
            string reg = ins[2];

            if(operand.empty()) {
                mode = Error;
            }
            else if(reg.empty()) {
                mode = Error;
            }
            else if(operand[0] == '(' && operand[operand.size() - 1] == ')') {
                if(checkIsByte(operand.substr(1, operand.length() - 2)) && reg == "y") {
                    mode = IndirectY;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else if(labelAddress.find(operand.substr(1, operand.length() - 2)) != labelAddress.end() && reg == "y") {
                    mode = IndirectY;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else {
                    mode = Error;
                }
            }
            else if(operand[0] == '(' && reg[reg.size() - 1] == ')') {
                if(checkIsByte(operand.substr(1)) && reg.substr(0, 1) == "x") {
                    mode = IndirectX;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else if(labelAddress.find(operand.substr(1)) != labelAddress.end() && reg.substr(0, 1) == "x") {
                    mode = IndirectX;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else {
                    mode = Error;
                }
            }
            else {
                if(checkIsByte(operand) && reg == "x") {
                    mode = ZeroPageX;
                    if(!checkModeExists(opcode, mode)) {
                        mode = AbsoluteX;
                    }
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else if(checkIsByte(operand) && reg == "y") {
                    mode = ZeroPageY;
                    if(!checkModeExists(opcode, mode)) {
                        mode = AbsoluteY;
                    }
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else if(checkIsWord(operand) && reg == "x") {
                    mode = AbsoluteX;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else if(checkIsWord(operand) && reg == "y") {
                    mode = AbsoluteY;
                    changeModeToError(opcode, mode);
                    appendOpcode(opcode, mode, conversion);
                    appendOperands(opcode, operand, mode, conversion);
                }
                else if(labelAddress.find(operand) != labelAddress.end()) {
                    if(labelAddress[operand] <= MAX_BYTE && reg == "x") {
                        mode = ZeroPageX;
                        if(!checkModeExists(opcode, mode)) {
                            mode = AbsoluteX;
                        }
                        changeModeToError(opcode, mode);
                        appendOpcode(opcode, mode, conversion);
                        appendOperands(opcode, operand, mode, conversion);
                    }
                    else if(labelAddress[operand] <= MAX_BYTE && reg == "y") {
                        mode = ZeroPageY;
                        if(!checkModeExists(opcode, mode)) {
                            mode = AbsoluteY;
                        }
                        changeModeToError(opcode, mode);
                        appendOpcode(opcode, mode, conversion);
                        appendOperands(opcode, operand, mode, conversion);
                    }
                    else if(labelAddress[operand] <= MAX_WORD && reg == "x") {
                        mode = AbsoluteX;
                        changeModeToError(opcode, mode);
                        appendOpcode(opcode, mode, conversion);
                        appendOperands(opcode, operand, mode, conversion);
                    }
                    else if(labelAddress[operand] <= MAX_WORD && reg == "y") {
                        mode = AbsoluteY;
                        changeModeToError(opcode, mode);
                        appendOpcode(opcode, mode, conversion);
                        appendOperands(opcode, operand, mode, conversion);
                    }
                    else {
                        mode = Error;
                    }
                }
                else {
                    mode = Error;
                }
            }
        }
        else {
            mode = Error;
        }
    }



    

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
        default: {printf("Error\n"); break;}
    }


    return conversion;
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
        vector<Byte> conversion;
        string opcode = parsed[i][0];
        printInstruction(parsed[i]);
        conversion = parseOperands(parsed[i]);
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
