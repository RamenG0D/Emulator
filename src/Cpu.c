#include "Types.h"
#include "Cpu.h"

#include <assert.h>

void print_binary(const char* format, Byte num) {
    while (num) {
        if (num & 1) printf("1");
        else printf("0");
        num >>= 1;
    }
}

void execute_opcode(Byte opcode) {
  switch (opcode) {
    case Load:{
        //
        break;
    }
    default:{
        assert("Invalid opcode");
        break;
    }
  }
}
