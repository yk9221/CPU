#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <unordered_set>

#include "cpu.h"

#define MAX_BYTE 255
#define MAX_WORD 65535

using namespace std;

enum class Radix {
    BIN,
    OCT,
    DEC,
    HEX,
    ERR
};

bool checkIsByte(const string& num);
bool checkIsWord(const string& num);
Radix checkIsNumber(const string& num);

class Assembler {
public:
    Assembler();
    ~Assembler();

    void parse(CPU* cpu, const string& filename);
    vector<string> split(const string& str, const unordered_set<char>& delimiter);
    void addToMemory(vector<Byte>& instructions, CPU* cpu, Word& PCCopy);
};

#endif