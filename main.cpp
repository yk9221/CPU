#include "globals.h"
#include "cpu.h"
#include "memory.h"
#include "assembler.h"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cerr << "Usage: ./cpu <filename>.yk" << endl;
        return EXIT_FAILURE;
    }
    const string filename = argv[1];
    ifstream f(filename.c_str());

    if(!f.good()) {
        cerr << "File " << filename << " does not exit." << endl;
        return EXIT_FAILURE;
    }
    if(filename.substr(filename.size() - 2, 2) != "yk") {
        cerr << "File " << filename << " should be in .yk format" << endl;
        return EXIT_FAILURE;
    }


    CPU* cpu = new CPU();
    
    Assembler* assembler = new Assembler();
    assembler->parse(cpu, filename);

    cpu->run();

    return 0;
}
