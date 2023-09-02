
typedef struct Cartrige {
    Byte EPROM[KiloBytes(32)];
    Byte RAM[KiloBytes(8)];
    BankState state; // Game Can switch between using !either! EPROM, or RAM
} Cart;
