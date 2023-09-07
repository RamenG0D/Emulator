#include "Types.h"
#include "GameBoy.h"

// Fetches an instruction of 1 byte in length 
Byte Fetch(Cpu* cpu, Bus* bus) {
    Byte data = bus->cartridge.EPROM[cpu->pc];
    cpu->pc += 0x1;
    return data;
}

void WriteMem(GameBoy* gb, Byte addr, Byte val) {
    //
}

Byte ReadMem(GameBoy* gb, Byte addr) {
    //
}
