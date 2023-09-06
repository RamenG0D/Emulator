#include "Types.h"
#include "Cpu.h"

#include <assert.h>

inline void print_binary(const char* format, Byte num) {
    while (num) {
        if (num & 1) printf("1");
        else printf("0");
        num >>= 1;
    }
}

void execute_opcode(Byte opcode) {
  switch (opcode) {
    case 0x00:{
        // NOP
        break;
    }
    case 0x01:{
        // LD BC,nn
        break;
    }
    case 0xFF:{
        // RST n
        break;
    }
    default:{
        assert("Invalid opcode");
        break;
    }
  }
}
