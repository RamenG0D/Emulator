#include "utils.h"

#include "Cpu.h"
#include "Cartridge.h"

#define MBC_STATE 0x147

typedef struct BUS {
    Ppu ppu; Cart cartridge;
    MbcState state;
} Bus;

inline void DetermineBankState(Bus* bus) {
    switch (bus->cartridge.EPROM[MBC_STATE])
    {
        case 1: bus->state = MBC1; break;
        case 2: bus->state = MBC1; break;
        case 3: bus->state = MBC1; break;
        case 5: bus->state = MBC2; break;
        case 6: bus->state = MBC2; break;
        default: break;
    }
}

typedef struct Gameboy {
    Cpu cpu; Bus bus;
    struct {
        Byte ROM[KiloBytes(16)];
        Byte E_SWITCHABLE_ROM_BANK[KiloBytes(16)];
        Byte VRAM[KiloBytes(8)];
        Byte E_SWITCHABLE_RAM_BANK[KiloBytes(8)];
        Byte RAM[KiloBytes(8)];
        Byte IO[KiloBytes(8)];
    } Memory;
} Gameboy;

Byte Fetch(Cpu* cpu, Bus* bus); // will fetch the next instruction from memory and increase the cpu tick by one
