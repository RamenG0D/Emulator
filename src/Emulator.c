#include "Emulator.h"

#define DEBUG

int main(void) {
    Vector2 MousePos = { 0 };
    Gameboy gb = { 0 };

    init(700, 700); rlImGuiSetup(true);
    igCreateContext(NULL); ImGuiIO* io = igGetIO();

    Texture background = LoadTextureFromImage(LoadImage("background.png"));

    int Width, Height;

    ReadFileBytes("Roms/Pokemon.gb", gb.cartridge.EPROM, sizeof(gb.cartridge.EPROM));

    while (!WindowShouldClose()) {
        Width = GetRenderWidth(); Height = GetRenderHeight();
        MousePos = GetMousePosition();

        BeginDrawing();
            ClearBackground(BLACK); DrawTextureScaled(background, 0, 0, Width, Height);
            rlImGuiBegin();
                //Purpose is to allow for checking to see if the game code (asm) has been loaded into the vRam correctly
                #ifdef DEBUG
                    bool open = true;
                    igBegin("Log", &open, ImGuiWindowFlags_None);
                        printf("FPS: %d\n", io->Framerate);
                    igEnd();
                #endif
            rlImGuiEnd();
        EndDrawing();
    }

    // DE - INITIALIZATION

    rlImGuiShutdown();

    UnLoadTextures(background);

    CloseWindow();

    return 0;
}

void ReadFileBytes(const char* filepath, Byte* data, size_t size) {
    if(FileExists(filepath) == true) {
        FILE* file = fopen(filepath, "rb");
        fread(data, sizeof(Byte), size, file);
        fclose(file);
    } else {
        printf("Error: Failed to read file => { %s }\n", filepath);
    }
}
