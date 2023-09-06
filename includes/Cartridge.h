
typedef struct Cartrige {
    Byte EPROM[KiloBytes(32)]; // The (Main) Space Where the Games are stored
    Byte RAM[KiloBytes(8)]; // Extra addressable Ram
} Cart;
