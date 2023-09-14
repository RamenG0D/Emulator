#include "Emulator.h"

#define GAME_ROM "Roms/Pokemon.gb"

#include "ImGuiRaylib.h"
#include "rlImGuiColors.h"

GenUnloader(RenderTexture);
GenUnloader(Texture);
GenUnloader(Image);

CreateWindow(GameBoyWindow, GAMEBOY_WIDTH, GAMEBOY_HEIGHT, ImGuiWindowFlags_AlwaysAutoResize);
CreateWindowSlim(SettingsWindow, 300, 140, ImGuiWindowFlags_AlwaysAutoResize);
InnerWindow* game_boy;
InnerWindow* settings;

static int WindowScale = 0;

void ImGuiUpdate(void) {
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
        if(!IsRenderTextureReady(game_boy->screen)) return;
        game_boy->BeginRender();
            igImage(
                (ImTextureID)&game_boy->screen.texture, 
                (ImVec2){ (float)game_boy->screen.texture.width+WindowScale, (float)game_boy->screen.texture.height+WindowScale }, 
                (ImVec2){ 0, 1 }, (ImVec2){ 1, 0 }, 
                Convert(WHITE), (ImVec4){ 0 }
            );
        game_boy->EndRender();
    }

    if(settings->open) {
        settings->BeginRender();
            igSliderInt("Test", &WindowScale, 0, 400, "Test_test", ImGuiSliderFlags_None);
        settings->EndRender();
    }
}

void UpdateScreen(void) {
    BeginTextureMode(game_boy->screen);
    {
        ClearBackground(RAYWHITE);
        DrawText("HELLO", 0, 0, 18, RED);
    }
    EndTextureMode();
}

int main(void) {
    SetWindowState(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    InitWindow(700, 700, "GameBoy Emulator");

    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(60);

    ImGuiInit(true);

    igGetIO()->ConfigWindowsMoveFromTitleBarOnly = true;
    game_boy = GetGameBoyWindow();
    settings = GetSettingsWindow();

    Texture background = LoadTexture("resources/background.png");
    Byte Mem[KiloBytes(32)];

    Log(Normal, "Loading Game Rom", NULL);
    ReadFileBytes("Roms/Pokemon.gb", Mem, sizeof(Mem));
    Log(Normal, "Rom Loaded Succesfully", NULL);

    while(!WindowShouldClose()) {
        UpdateScreen();

        BeginDrawing();
        {
            ClearBackground(BLACK);
            DrawTextureScaled(background, 0, 0, GetRenderWidth(), GetRenderHeight());

            ImGuiBegin();
            {
                ImGuiUpdate();
            }
            ImGuiEnd();
        }
        EndDrawing();
    }

    // DE - INITIALIZATION

    AutoUnloader(RenderTexture, game_boy->screen);

    AutoUnloader(Texture, background);

    ImGuiShutdown();

    CloseWindow();

    return 0;
}

// Reads the Game Into the Gameboys Cartridge Mem
inline void ReadFileBytes(const char *filepath, Byte *data, size_t size) {
    if (FileExists(filepath) == true) {
        FILE *file = fopen(filepath, "rb");
        fread(data, sizeof(Byte), size, file);
        fclose(file);
    } else {
        Log(Panic, "Error: Failed to read file => { %s }\n", filepath);
    }
}
