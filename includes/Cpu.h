
typedef struct Cpu {
    Byte pc[2];
    Byte sp[2];
} Cpu;

Byte Fetch(Cpu*); // will fetch the next instruction from memory and increase the cpu tick by one

// void Cycle(Cpu*); // Should just increase the tick / (cpu tick), by one 
