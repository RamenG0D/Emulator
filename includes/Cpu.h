
typedef struct Cpu {
    Byte pc[2];
    Byte sp[2];
} Cpu;

Byte Fetch(Cpu*);
