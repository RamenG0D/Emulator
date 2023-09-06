#include "Emulator.h"
#include <assert.h>

#define DEBUG

#define GAMEBOY_WIDTH 160
#define GAMEBOY_HEIGHT 144

#define Loop(num, Block) \
    for(int I = 0; I < num; ++I) \
        Block; \

#define CreateWindow(name) \
    typedef struct { \
        Rectangle windowRect; \
        bool isDragging; \
    } name##Window; \
    name##Window name = \
    { \
        (Rectangle){200, 50, GAMEBOY_WIDTH, GAMEBOY_HEIGHT}, \
        false \
    }; \
    void name##WindowEvents(Texture* img) { \
        rlImGuiBegin(); \
        { \
            igSetNextWindowContentSize((ImVec2){ GameBoy.windowRect.width, GameBoy.windowRect.height }); \
            igSetNextWindowPos((ImVec2){ GameBoy.windowRect.x, GameBoy.windowRect.y }, ImGuiCond_Always, (ImVec2){ 0 }); \
            igBegin("Test", NULL, ImGuiWindowFlags_AlwaysAutoResize); \
                WindowEvents(name); \
            igEnd(); \
        } \
        rlImGuiEnd(); \
    }

#define WindowEvents(name) \
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { \
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){ name.windowRect.x, name.windowRect.y, name.windowRect.width, 18 })) { \
            name.isDragging = true; \
        } \
    } \
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { \
        name.isDragging = false; \
    } \
    if (name.isDragging) { \
        name.windowRect.x += GetMouseDelta().x; \
        name.windowRect.y += GetMouseDelta().y; \
    }

CreateWindow(GameBoy);

#ifdef DEBUG
    #define Log(msg, ...) igDebugLog("[LOG]"); igDebugLog(msg, __VA_ARGS__); igDebugLog("\n");

    #define debug if(!quit) GameBoyWindowEvents(&texture);
#else
    #define Log
    #define debug
#endif

bool quit = false;

inline void DrawScreen(Image* image) {
    //
}

int main(void) {
    Vector2 MousePos = { 0 };
    Gameboy gb = { 0 };

    init(700, 700);

    rlImGuiSetup(true);

    Texture background = LoadTextureFromImage(LoadImage("background.png"));

    int Width, Height; rlImGuiBeginInitImGui();

    ReadFileBytes("Roms/Pokemon.gb", gb.cartridge.EPROM, sizeof(gb.cartridge.EPROM));
    Image img = GenImageColor(400, 400, BLACK); ImageDrawText(&img, "Test", 0, 0, 20, WHITE);
    Texture texture = LoadTextureFromImage(img);

    while (!WindowShouldClose()) {
        Width = GetRenderWidth(); Height = GetRenderHeight();
        MousePos = GetMousePosition();

        BeginDrawing();
            ClearBackground(BLACK); DrawTextureScaled(background, 0, 0, Width, Height);
            //Purpose is to allow for checking to see if the game code (asm) has been loaded into the vRam correctly
            debug
        EndDrawing();
    }

    // igDestroyContext(NULL);

    // DE - INITIALIZATION

    rlImGuiShutdown();

    UnloadImage(img);

    UnLoadTextures(background, texture);

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
