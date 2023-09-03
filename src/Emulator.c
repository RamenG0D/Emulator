#include "Emulator.h"

#define DEBUG

void DrawConsoleLog(const char* text, const int x, const int y, const int width, const int height) {
    static char* Log = { 0 };
    Log = malloc(sizeof(Log)+sizeof(text));
    strcat(Log, text);
    //DrawRectangle(x, y, width, height, BLACK);
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_CENTER);
    GuiTextBox((Rectangle){ x, y, width, height }, Log, sizeof(Log), false);
}

int main(void) {
    Vector2 MousePos = { 0 };
    Gameboy gb = { 0 };

    init(700, 700);

    Texture background = LoadTextureFromImage(LoadImage("background.png"));
    
    CreateMiniWindow(Game, MousePos, 20, 60, 200, 230);
    CreateMiniWindow(Log, MousePos, 300, 30, 300, 300);
    int Width, Height;

    ReadFileBytes("Roms/Pokemon.gb", gb.cartridge.EPROM, sizeof(gb.cartridge.EPROM));
    
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
                //Purpose is to allow for checking to see if the game code (asm) has been loaded into the vRam correctly
                #ifdef DEBUG
                    /*for(int i = 0; i < sizeof(gb.cartridge.EPROM); ++i) {
                        if(i % 8 == 0) DrawConsoleLog("\n", Log.WindowPos.x, Log.WindowPos.y+24, Log.WindowPos.width, Log.WindowPos.height-24);
                        DrawConsoleLog(TextFormat("%c", gb.cartridge.EPROM[i]), Log.WindowPos.x, Log.WindowPos.y+24, Log.WindowPos.width, Log.WindowPos.height-24);
                    }*/
                    // DrawConsoleLog("\n", Log.WindowPos.x, Log.WindowPos.y+24, Log.WindowPos.width, Log.WindowPos.height-24);
                    //DrawConsoleLog(, Log.WindowPos.x, Log.WindowPos.y+24, Log.WindowPos.width, Log.WindowPos.height-24);
                    printf("%s\n", TextFormat("%x", gb.cartridge.EPROM));
                #endif
                //DrawConsoleLog("Hello!", Log.WindowPos.x, Log.WindowPos.y+24, Log.WindowPos.width, Log.WindowPos.height);
                InnerWindowEvents(Log);
            }
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
