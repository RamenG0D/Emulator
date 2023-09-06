#include <stdbool.h>
#include <stddef.h>

#include "raylib.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"

#include "rlImGui.h"

#include "rlImGuiColors.h"

typedef unsigned char Byte;

/*
 *
 * Used to create a struct of only a single bit with the specified name,
 * it helps with ensureing that where ever its used is only a single bit value and it can be named
 *
 */
#define Bit(name) \
struct { \
    Byte bit : 1; \
} name

typedef enum {
    MBC1,
    MBC2
} MbcState;

/*typedef enum {
    // Signals
} ControlSiganl;*/

#define KiloBytes(b) b*1000
