#ifndef MEMORY_H
#define MEMORY_H

#include "globals.h"

class Memory {
public:
    Byte* RAM;

    Memory();
    ~Memory();

    void write(Word address, Byte value);
    Byte read(Word address);
    void printMemory();
};

#endif
