
// The Speed of the cpu at proccessing instructions
#define CLOCK_MHZ 4.194304

#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4

// The (Picture. Proccessing. Unit.) of the GameBoy
typedef struct PPU {
    Byte Test : 1;
} Ppu;

typedef Byte Word[2]; // 16 bits / (Byte * 2)

// The Cpu Registers that the gameboy uses to store and load small amounts of data very quickly
typedef union {
    Word reg;
    struct {
        Byte low; // Least Significant
        Byte high; // Most Significant
    };
} Register;

typedef struct Cpu {
    Word pc; // Program Counter, keeps track of where in the program we are
    Register sp; // Stack Pointer, stores the current location in the stack 
    Register AF; // The Grouped together (A + F) Registers
    Register BC; // The Grouped together (B + C) Registers
    Register DE; // The Grouped together (D + E) Registers
    Register HL; // The Grouped together (H + L) Registers
    Byte Z : 1; // Zero Flag
    Byte N : 1; // Subtraction Flag
    Byte HC: 1; // Half Carry Flag
    Byte C : 1; // Carry Flag
} Cpu;

// void Cycle(Cpu*); // Should just increase the tick / (cpu tick), by one 
