#include "globals.h"
#include "cpu.h"
#include "memory.h"
#include "assembler.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    CPU* cpu = new CPU();
    const string filename = "code.yk";
    
    Assembler* assembler = new Assembler();
    assembler->parse(cpu, filename);

    printf("---------------------------------------------------------------\n");
    cpu->printMemory();
    cpu->run();
    cpu->printRegisters();
    printf("---------------------------------------------------------------\n");
    return 0;
}
