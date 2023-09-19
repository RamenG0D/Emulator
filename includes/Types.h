#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "Raylib/raylib.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_NO_EXPORT
#include "cimgui.h"

#define KiloBytes(b) b*1000

typedef uint8_t Byte; // 8 bits
typedef uint16_t Word; // 16 bits

// The Cpu Registers that the gameboy uses to store and load small amounts of data very quickly
typedef union {
    Word reg;
    struct {
        Byte low; // Least Significant
        Byte high; // Most Significant
    };
} Register;
