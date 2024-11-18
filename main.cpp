#include "globals.h"
#include "cpu.h"
#include "memory.h"
#include "assembler.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    const string filename = "code.yk";
    CPU* cpu = new CPU();
    Assembler* assembler = new Assembler();
    assembler->parse(cpu, filename);

    printf("---------------------------------------------------------------\n");
    cpu->run();
    cpu->printRegisters();
    cpu->printMemory();
    printf("---------------------------------------------------------------\n");
    return 0;
}
