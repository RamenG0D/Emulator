#include "Emulator.h"

int main(void) {
    Gameboy gb = { 0 };

    init(700, 700);
    
    Texture a, b, c;
    Image img = LoadImage("background.png");
    Texture2D background = LoadTextureFromImage(img);
    a = LoadTextureFromImage(ImageCopy(img));
    b = LoadTextureFromImage(ImageCopy(img));
    c = LoadTextureFromImage(ImageCopy(img));
    
    CreateMiniWindow(Game); WindowGamePos = (Rectangle){ 20, 60, 200, 230 };
    CreateMiniWindow(Log); WindowLogPos = (Rectangle){ 300, 30, 300, 300 };
    int Width, Height;

    Byte GameData[KiloBytes(1024)];
    read_game("Roms/Pokemon.gb", GameData, sizeof(GameData));

    for(int i = 0; i < sizeof(gb.cartridge.EPROM); ++i)
        gb.cartridge.EPROM[i] = GameData[i];
    
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
            if(!WindowGameExit) {
                WindowGameExit = GuiWindowBox(WindowGamePos, "Game");
                DrawGameScreen(WindowGamePos.x, WindowGamePos.y+24, WindowGamePos.width, WindowGamePos.height);
                InnerWindowEvents(Game);           
            }
            if(!WindowLogExit) {
                WindowLogExit = GuiWindowBox(WindowLogPos, "Log");
                InnerWindowEvents(Log);
            }
            //if(!WindowMemVeiwExit) {  = GuiWindowBox(, "Mem"); }
        EndDrawing();
    }

    // DE - INITIALIZATION

    UnLoadTextures(background, a, b, c);

    CloseWindow();

    return 0;
}

void DrawGameScreen(const int x, const int y, const int width, const int height) {
    DrawRectangle(x, y, width, height, BLACK); // Clear game window / screen

    // Other stuff
    //
}

void read_game(const char* filepath, Byte* data, size_t size) {
    if(FileExists(filepath) == true) {
        FILE* file = fopen(filepath, "rb");
        fread(data, sizeof(Byte), size, file);
        fclose(file);
    } else {
        printf("Error: Failed to read file => { %s }\n", filepath);
    }
}

void UnLoadTextures(Texture texture, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, texture);
    UnloadTexture(texture);
    do {
        Texture t = va_arg(arg_ptr, Texture);
        UnloadTexture(t);
    } while(arg_ptr != NULL);
    va_end(arg_ptr);
}

void DrawTextureScaled(Texture2D img, int x, int y, int width, int height) {
    img.width = width; img.height = height;
    DrawTexture(img, x, y, WHITE);
}

void init(int width, int height) {
    InitWindow(width, height, "Test");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
}
