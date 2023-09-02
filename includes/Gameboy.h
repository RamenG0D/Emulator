#include "Cpu.h"
#include "Cartridge.h"

typedef struct Gameboy {
    Cpu cpu; Cart cartridge;
} Gameboy;
