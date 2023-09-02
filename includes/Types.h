#include <stdbool.h>
#include <stddef.h>

#include "raylib.h"

typedef unsigned char Byte;

typedef enum {
    ROM,
    RAM,
} BankState;

typedef struct InnerWindow {
    Rectangle WindowPos;
    Vector2 Offset;
    bool WindowExit;
    bool DragWindow;
} Window;

/*typedef enum {
    // Signals
} ControlSiganl;*/

#define KiloBytes(b) b*1000
