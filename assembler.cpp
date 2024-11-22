#include "assembler.h"

Assembler::Assembler() {}
Assembler::~Assembler() {}

void Assembler::printInstruction(vector<string>& vec) {
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

void Assembler::printMode(AddressingMode& mode) {
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
                tokens.push_back(toLower(buffer));
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
        tokens.push_back(toLower(buffer));
        buffer.clear();
    }
    return tokens;
}

string Assembler::removeParenthesisOperands(string& operands, AddressingMode& mode) {
    if(mode == Indirect || mode == IndirectY) {
        return operands.substr(1, operands.size() - 2);
    }
    else if(mode == IndirectX) {
        return operands.substr(1);
    }
    return operands;
}

string Assembler::toLower(string str) {
    for(char& c : str) {
        c = tolower(static_cast<unsigned char>(c));
    }

    return str;
}

Word Assembler::stringToNum(string& num) {
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

bool Assembler::checkIsByte(const string& num) {
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

bool Assembler::checkIsWord(const string& num) {
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

Radix Assembler::checkIsNumber(const string& num) {
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

int Assembler::getModeIndex(string& opcode, AddressingMode& mode) {
    for(int i = 0; i < allAddressingModes[opcode].size(); i++) {
        if(allAddressingModes[opcode][i].first == mode) {
            return i;
        }
    }
    return -1;
}

bool Assembler::checkModeExists(string& opcode, AddressingMode mode) {
    for(int i = 0; i < allAddressingModes[opcode].size(); i++) {
        if(allAddressingModes[opcode][i].first == mode) {
            return true;
        }
    }
    return false;
}

void Assembler::setErrorModeIfMissing(string& opcode, AddressingMode& mode) {
    if(!checkModeExists(opcode, mode)) {
        mode = Error;
    }
}

Byte Assembler::getModeBytes(AddressingMode mode) {
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

void Assembler::appendOpcode(string& opcode, AddressingMode& mode, vector<Byte>& conversion, Word& PC) {
    int modeIndex = getModeIndex(opcode, mode);
    if(modeIndex != -1) {
        conversion.push_back(allAddressingModes[opcode][modeIndex].second);
    }
}

void Assembler::appendOperands(string& opcode, string& operands, AddressingMode& mode, vector<Byte>& conversion, Word& PC) {
    Word value = 0x0;
    string num;

    if(labelAddress.find(operands) != labelAddress.end() || (labelAddress.find(operands.substr(1)) != labelAddress.end() && operands[0] == '#')) {
        if(mode == Absolute || mode == AbsoluteX || mode == AbsoluteY) {
            value = labelAddress[operands].value;
        }
        else {
            if(labelAddress[operands.substr(1)].mode == Define && operands[0] == '#') {
                value = labelAddress[operands.substr(1)].value;
            }
            else if(labelAddress[operands].mode == Define) {
                value = labelAddress[operands].value;
            }
            else {
                value = labelAddress[operands].value - (PC + 2);

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

void Assembler::checkImplicit(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS

    if(ins.size() == 1) {
        mode = Implicit;
        setErrorModeIfMissing(opcode, mode);
    }
}

void Assembler::checkAccumulator(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS
    // INS A

    if(ins.size() == 1 && mode != Implicit) {
        mode = Accumulator;
        setErrorModeIfMissing(opcode, mode);
    }

    if(ins.size() == 2) {
        string operand = ins[1];
        if(operand == "a") {
            mode = Accumulator;
            setErrorModeIfMissing(opcode, mode);
        }
    }
}

void Assembler::checkImmediate(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkZeroPage(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkZeroPageX(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkZeroPageY(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkRelative(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // INS LABEL

    if(ins.size() == 2) {
        string operand = ins[1];

        if(labelAddress.find(operand) != labelAddress.end() && checkModeExists(opcode, Relative)) {
            mode = Relative;
        }
    }
}

void Assembler::checkAbsolute(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkAbsoluteX(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkAbsoluteY(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkIndirect(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkIndirectX(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkIndirectY(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkLabel(vector<string>& ins, AddressingMode& mode, string& opcode) {
    // LABEL:

    if(ins.size() == 1)
        if(opcode[opcode.size() - 1] == ':') {
            mode = Label;
        }
}

void Assembler::checkDefine(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

void Assembler::checkModes(vector<string>& ins, AddressingMode& mode, string& opcode) {
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

    // printMode(mode);
}

void Assembler::insertLabels(vector<string>& ins) {
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

void Assembler::parseLabels(vector<string>& ins, Word& PC) {
    string opcode = ins[0];
    AddressingMode mode = Error;

    checkModes(ins, mode, opcode);

    PC += getModeBytes(mode);

    if(mode == Label) {
        labelAddress[opcode.substr(0, opcode.size() - 1)].value = PC;
    }

    if(mode == Error) {
        cout << "Error in line: ";
        for(int i = 0; i < ins.size(); i++) {
            cout << ins[i] << " ";
        }
        cout << endl;
        exit(EXIT_FAILURE);
    }
}

void Assembler::parseOperands(vector<string>& ins, Word& PC, vector<Byte>& conversion) {
    string opcode = ins[0];
    AddressingMode mode;

    checkModes(ins, mode, opcode);
    appendOpcode(opcode, mode, conversion, PC);

    if(ins.size() > 1 && mode != Define) {
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

    for(int i = 0; i < parsed.size(); i++) {
        parseLabels(parsed[i], PC);
    }
    PC = cpu->PC;

    for(int i = 0; i < parsed.size(); i++) {
        vector<Byte> conversion;

        // printInstruction(parsed[i]);
        parseOperands(parsed[i], PC, conversion);
        addToMemory(conversion, cpu, PC);
    }

    file.close();
}

void Assembler::addToMemory(vector<Byte>& instructions, CPU* cpu, Word& PC) {
    for(auto ins : instructions) {
        cpu->write(PC, ins);
        PC++;
    }
}
