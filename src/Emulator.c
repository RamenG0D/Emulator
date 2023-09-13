#include "Emulator.h"
#include "rlgl.h"

#define GAME_ROM "Roms/Pokemon.gb"

#include "ImGuiRaylib.h"

inline void DrawScreen(Image* image) {
    //
}

GenUnloader(Texture);
// GenUnloader(Image);

CreateWindow(GameBoyWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
InnerWindow* game_boy;

void windowing(void) {
    if(igBeginMainMenuBar()) {
        if(igBeginMenu("Windows", true)) {
            if(igMenuItem_BoolPtr("GameBoy", NULL, NULL, true)) {
                game_boy->open = true;
            }
            if(igMenuItem_BoolPtr("Exit", NULL, NULL, true)) {
                exit(0);
            }
            igEndMenu();
        }
        igEndMainMenuBar();
    }

    igShowDebugLogWindow(NULL); // Debug Window

    // GAMEBOY window
    if(game_boy->open) {
        game_boy->begin_render(GAMEBOY_WIDTH, GAMEBOY_HEIGHT);
            // window stuff
        game_boy->end_render();
    }
}


int main(void) {
    SetWindowState(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    InitWindow(700, 700, "GameBoy Emulator");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    ImGuiInit(true);

    igGetIO()->ConfigWindowsMoveFromTitleBarOnly = true;
    game_boy = GetGameBoyWindow();

    Texture background = LoadTextureFromImage(LoadImage("background.png"));
    int Width, Height;

    while(!WindowShouldClose()) {
        Width = GetRenderWidth(); Height = GetRenderHeight();

        BeginDrawing();
        {
            ClearBackground(BLACK); 
            DrawTextureScaled(background, 0, 0, Width, Height);

            printf("HELP\n");
            ImGuiBegin();
                windowing();
            ImGuiEnd();
            printf("STUCK\n");
        }
        EndDrawing();
    }

    // DE - INITIALIZATION

    //AutoUnloader(Image, ScreenImage);

    AutoUnloader(Texture, background);//, ScreenTexture);

    ImGuiShutdown();

    CloseWindow();

    return 0;
}

// Reads the Game Into the Gameboys Cartridge Mem
inline void ReadFileBytes(const char *filepath, const Byte *data, size_t size) {
    if (FileExists(filepath) == true) {
        FILE *file = fopen(filepath, "rb");
        fread((Byte*)data, sizeof(Byte), size, file);
        fclose(file);
    } else {
        printf("Error: Failed to read file => { %s }\n", filepath);
    }
}
