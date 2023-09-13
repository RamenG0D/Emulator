
// where the MBC state is stored in the rom
#define MBC_STATE 0x147

#define Ram 0
#define Ram_Bank 1

typedef struct MEMMAP {
    Byte VRAM[KiloBytes(8)]; // Video Memory
    Byte RAM[2][KiloBytes(8)]; // Use Defined vals to access ram or bank
    Byte IO[KiloBytes(8)];
} MemMap;

// Byte Fetch(Bus* bus); // will fetch the next instruction from memory and increase the cpu tick by one

// void WriteMem(Bus* bus, Word addr, Byte val);

// Byte ReadMem(Bus* bus, Word addr);

// typedef enum {
    // Nop = 0x00, // Do Nothing
    // Load = 0x41, // Load val into Specified Register kinda like (mov Reg A, Reg B)
// } OpCode;

// To be defined later / TODO
// typedef enum {
    // x
// } LoadType;

//--------------------------------------------------------------------------------------------------------
// Begin OPERATION FUNCTIONS
//--------------------------------------------------------------------------------------------------------

//#define NUM_CODES 2

// void load();
// void noop();

//--------------------------------------------------------------------------------------------------------
// End OPERATION FUNCTIONS
//--------------------------------------------------------------------------------------------------------

/*
typedef struct InStruct {
    Function func; int ClockCycles;
} Instruction;
*/

// #define NOP 0
// #define LOAD 1

// GB opcode instruction map
// Instruction instructions[NUM_CODES] = {
    // { noop, 4 },
    // { load, 4 }
// };
