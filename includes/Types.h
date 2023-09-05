#include <stdbool.h>
#include <stddef.h>

#include "raylib.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"

#include "rlImGui.h"

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#define igDebugCheckVersionAndDataLayout igDebugCheckVersionAndDataLayout_Str
#endif

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
} name \

typedef enum {
    ROM,
    RAM,
} BankState;

/*typedef enum {
    // Signals
} ControlSiganl;*/

#define KiloBytes(b) b*1000
