#include <stdio.h>

#include "Types.h"
#include "Gameboy.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void ReadFileBytes(const char*, Byte*, size_t);

/* A neat idea but doesnt work
#ifndef CUSTOM_IMPLEMENTATION
    Gameboy gb = { 0 };

    void GameBoyInit();
#endif*/
