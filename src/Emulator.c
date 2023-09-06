#include "Emulator.h"
#include <assert.h>

#define GAME_ROM "Roms/Pokemon.gb"

#define DEBUG

// Purpose is to allow for checking to see if the game code (asm) has been loaded into the vRam correctly
#define Log(msg, ...)             \
    igDebugLog("[LOG]");          \
    igDebugLog(msg, __VA_ARGS__); \
    igDebugLog("\n");

inline void DrawScreen(Image *image)
{
    //
}

CreateWindow(GameBoy, 200, 50, (Texture* img), { igImage((ImTextureID)img, (ImVec2){ GAMEBOY_WIDTH, GAMEBOY_HEIGHT }, (ImVec2){ 0 }, (ImVec2){ 0 }, (ImVec4){ 255 }, (ImVec4){ 0 }); });
CreateWindow(Log, 400, 400, (void), {});

void BootGameBoy(Gameboy* gb, const char* GameFile) {
    *gb->cpu.pc = 0x100;
    *gb->cpu.AF.reg = 0x01B0;
    *gb->cpu.BC.reg = 0x0013;
    *gb->cpu.DE.reg = 0x00D8;
    *gb->cpu.HL.reg = 0x014D;
    *gb->cpu.sp.reg = 0xFFFE;
    gb->Memory.ROM[0xFF10] = (Byte) 0x80;
    gb->Memory.ROM[0xFF11] = (Byte) 0xBF;
    gb->Memory.ROM[0xFF12] = (Byte) 0xF3;
    gb->Memory.ROM[0xFF14] = (Byte) 0xBF;
    gb->Memory.ROM[0xFF16] = (Byte) 0x3F;
    gb->Memory.ROM[0xFF19] = (Byte) 0xBF;
    gb->Memory.ROM[0xFF1A] = (Byte) 0x7F;
    gb->Memory.ROM[0xFF1B] = (Byte) 0xFF;
    gb->Memory.ROM[0xFF1C] = (Byte) 0x9F;
    gb->Memory.ROM[0xFF1E] = (Byte) 0xBF;
    gb->Memory.ROM[0xFF20] = (Byte) 0xFF;
    gb->Memory.ROM[0xFF23] = (Byte) 0xBF;
    gb->Memory.ROM[0xFF24] = (Byte) 0x77;
    gb->Memory.ROM[0xFF25] = (Byte) 0xF3;
    gb->Memory.ROM[0xFF26] = (Byte) 0xF1;
    gb->Memory.ROM[0xFF40] = (Byte) 0x91;
    gb->Memory.ROM[0xFF47] = (Byte) 0xFC;
    gb->Memory.ROM[0xFF48] = (Byte) 0xFF;
    gb->Memory.ROM[0xFF49] = (Byte) 0xFF;

    // TODO use program args or smth more dynamic to add the ability to load different roms
    ReadFileBytes(GameFile, gb->bus.cartridge.EPROM, sizeof(gb->bus.cartridge.EPROM));
}

Byte Fetch(Cpu* cpu, Bus* bus) {
    Byte data = bus->cartridge.EPROM[*cpu->pc];
    *cpu->pc += 0x1;
    return data;
}

GenUnloader(Texture);
GenUnloader(Image);

int main(void) {
    init(700, 700);

    Texture background = LoadTextureFromImage(LoadImage("background.png"));
    int Width; // The Raylib Window Width
    int Height; // The Raylib Window Height

    BootGameBoy(&gb, GAME_ROM); // Setting the default Boot values

    while (!WindowShouldClose())
    {
        Width = GetRenderWidth(); Height = GetRenderHeight();

        BeginDrawing();
        {
            ClearBackground(BLACK); DrawTextureScaled(background, 0, 0, Width, Height);

            BeginDraw();
            {
                ImageDrawLine(&ScreenImage, 0, 0, 100, 100, RAYWHITE);
            }
            EndDraw();

            rlImGuiBegin();
            {
                GameBoyWindowEvents(&ScreenTexture);
                LogWindowEvents();
                //igShowDebugLogWindow(NULL);
            }
            rlImGuiEnd();
        }
        EndDrawing();
    }

    // DE - INITIALIZATION

    AutoUnloader(Image, ScreenImage);

    AutoUnloader(Texture, background, ScreenTexture);

    rlImGuiShutdown();

    CloseWindow();

    return 0;
}

// Reads the Game Into the Gameboys Cartridge Memory
inline void ReadFileBytes(const char *filepath, Byte *data, size_t size) {
    if (FileExists(filepath) == true) {
        FILE *file = fopen(filepath, "rb");
        fread(data, sizeof(Byte), size, file);
        fclose(file);
    } else {
        printf("Error: Failed to read file => { %s }\n", filepath);
    }
}
