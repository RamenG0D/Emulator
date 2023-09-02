#include "Types.h"

/*typedef enum {
    // Signals
} ControlSiganl;*/

typedef enum {
    ROM,
    RAM,
} BankState;

typedef struct Cartrige {
    Byte EPROM[KiloBytes(32)];
    Byte RAM[KiloBytes(8)];
    BankState state;
} Cart;
