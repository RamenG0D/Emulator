#pragma once

#include <stdbool.h>
#include <stddef.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Raylib/raylib.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_NO_EXPORT
#include "cimgui.h"

#define KiloBytes(b) b*1000

typedef unsigned char Byte;

typedef int16_t Word; // 16 bits / (Byte * 2)

typedef union {
    Byte wram[KiloBytes(8)];
    struct {
        Byte W1[KiloBytes(4)]; // first 4 kb
        Byte W2[KiloBytes(4)]; // second 4 kb
    };
} WorkRam;

// The Cpu Registers that the gameboy uses to store and load small amounts of data very quickly
typedef union {
    Word reg;
    struct {
        Byte low; // Least Significant
        Byte high; // Most Significant
    };
} Register;
