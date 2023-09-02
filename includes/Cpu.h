#include "Types.h"

typedef struct Cpu {
    Byte pc[2];
    Byte sp[2];
    struct Memory {
        Byte ROM[KiloBytes(16)];
        Byte E_SWITCHABLE_ROM_BANK[KiloBytes(16)];
        Byte VRAM[KiloBytes(8)];
        Byte E_SWITCHABLE_RAM_BANK[KiloBytes(8)];
        Byte RAM[KiloBytes(8)];
        Byte IO[KiloBytes(8)];
    } Memory;
} Cpu;

Byte Fetch(Cpu*);
