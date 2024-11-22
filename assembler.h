#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "cpu.h"

class Assembler {
public:
    Assembler();
    ~Assembler();

    void printInstruction(vector<string>& vec);
    void printMode(AddressingMode& mode);
    vector<string> split(const string& str, const unordered_set<char>& delimiter);
    string removeParenthesisOperands(string& operands, AddressingMode& mode);

    string toLower(string str);
    Word stringToNum(string& num);
    
    bool checkIsByte(const string& num);
    bool checkIsWord(const string& num);
    Radix checkIsNumber(const string& num);

    int getModeIndex(string& opcode, AddressingMode& mode);
    bool checkModeExists(string& opcode, AddressingMode mode);
    void setErrorModeIfMissing(string& opcode, AddressingMode& mode);
    Byte getModeBytes(AddressingMode mode);

    void appendOpcode(string& opcode, AddressingMode& mode, vector<Byte>& conversion, Word& PC);
    void appendOperands(string& opcode, string& operands, AddressingMode& mode, vector<Byte>& conversion, Word& PC);

    void checkImplicit(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkAccumulator(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkImmediate(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkZeroPage(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkZeroPageX(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkZeroPageY(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkRelative(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkAbsolute(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkAbsoluteX(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkAbsoluteY(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkIndirect(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkIndirectX(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkIndirectY(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkLabel(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkDefine(vector<string>& ins, AddressingMode& mode, string& opcode);
    void checkModes(vector<string>& ins, AddressingMode& mode, string& opcode);

    void insertLabels(vector<string>& ins);
    void parseLabels(vector<string>& ins, Word& PC);
    void parseOperands(vector<string>& ins, Word& PC, vector<Byte>& conversion);
    void parse(CPU* cpu, const string& filename);

    void addToMemory(vector<Byte>& instructions, CPU* cpu, Word& PCCopy);
};

#endif