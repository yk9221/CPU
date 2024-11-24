#include "globals.h"
#include "cpu.h"
#include "memory.h"
#include "assembler.h"

int main() {
    CPU* cpu = new CPU();
    const string filename = "code.yk";
    
    Assembler* assembler = new Assembler();
    assembler->parse(cpu, filename);

    cpu->run();
    
    return 0;
}
