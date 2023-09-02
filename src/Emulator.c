#include "Emulator.h"

Vector2 MousePos = { 0 };

int main(void) {
    Gameboy gb = { 0 };

    init(700, 700);

    Texture background = LoadTextureFromImage(LoadImage("background.png"));
    
    CreateMiniWindow(Game, MousePos, 20, 60, 200, 230);
    CreateMiniWindow(Log, MousePos, 300, 30, 300, 300);
    CreateMiniWindow(Extra, MousePos, 300, 230, 300, 300);
    int Width, Height;

    ReadFileBytes("Roms/Pokemon.gb", gb.cartridge.EPROM, sizeof(gb.cartridge.EPROM));
    
    //Purpose is to allow for checking to see if the game code (asm) has been loaded into the vRam correctly
    #ifdef DEBUG
        for(int i = 0; i < sizeof(gb.cartridge.EPROM); ++i) {
            if(i % 8 == 0) printf("\n");
            printf("%x ", (int)gb.cartridge.EPROM[i]);
        }
        printf("\n");
    #endif
    
    while (!WindowShouldClose()) {
        Width = GetRenderWidth(); Height = GetRenderHeight();
        MousePos = GetMousePosition();

        BeginDrawing();
            ClearBackground(BLACK); DrawTextureScaled(background, 0, 0, Width, Height);
            if(!Game.WindowExit) {
                Game.WindowExit = GuiWindowBox(Game.WindowPos, "Game");
                DrawGameScreen(Game.WindowPos.x, Game.WindowPos.y+24, Game.WindowPos.width, Game.WindowPos.height);
                InnerWindowEvents(Game);     
            }
            if(!Log.WindowExit) {
                Log.WindowExit = GuiWindowBox(Log.WindowPos, "Log");
                InnerWindowEvents(Log);
            }
            if(!Extra.WindowExit) {
                Log.WindowExit = GuiWindowBox(Log.WindowPos, "Log");
                InnerWindowEvents(Extra);
            }
            //if(!WindowMemVeiwExit) {  = GuiWindowBox(, "Mem"); }
        EndDrawing();
    }

    // DE - INITIALIZATION

    UnLoadTextures(background);

    CloseWindow();

    return 0;
}

void DrawGameScreen(const int x, const int y, const int width, const int height) {
    DrawRectangle(x, y, width, height, BLACK); // Clear game window / screen

    // Other stuff
    //
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
