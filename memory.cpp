#include "memory.h"

Memory::Memory() {
    RAM = new Byte[RAM_SIZE];
}
Memory::~Memory() {
    delete[] RAM;
}

void Memory::write(Word address, Byte value) {
    if(address >= 0x0000 && address <= 0xFFFF) {
        RAM[address] = value;
    }
}
Byte Memory::read(Word address) {
    if(address >= 0x0000 && address <= 0xFFFF) {
        return RAM[address];
    }
    return 0;
}
void Memory::printMemory() {
    std::ofstream outfile("output.bin", std::ios::binary);
    outfile.write(reinterpret_cast<const char*>(RAM), RAM_SIZE);
    outfile.close();
}
