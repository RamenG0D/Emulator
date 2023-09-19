#include "Emulator.h"

#define GAME_ROM "Roms/invaders"

#include "ImGuiRaylib.h"
#include "rlImGuiColors.h"

#include "background.h"

#include <stdio.h>

#include "ExtEmulator.h"

// First num is the range start and the second is the range end. Ex: 0, 100, is a range of 0 to 100
#define GameBoyScaleRange 0, 400

CreateWindow(GameBoyWindow, GAMEBOY_WIDTH, GAMEBOY_HEIGHT, ImGuiWindowFlags_AlwaysAutoResize);
CreateWindow(SettingsWindow, 300, 140, ImGuiWindowFlags_AlwaysAutoResize);

// STATIC LAND

static int WindowScale = 0;
static bool ShowGbFps = false;

// LEAVING THE LAND OF STATIC

static void ImGuiUpdate(void);

static void UpdateScreen(invaders* const cpu) {
    BeginTextureMode(game_boy->screen);
    {
        ClearBackground(BLACK);
        for (int x = 0; x < 256; ++x) for(int y = 0; y < 224; ++y) 
            DrawPixel(y, x, cpu->screen_buffer[x][y]);
    }
    
    if(ShowGbFps) {
        DrawText(TextFormat("FPS: %d", (int)igGetIO()->Framerate), 0, 0, 12, RED);
    }
    EndTextureMode();
}

GenUnloader(RenderTexture);

// Load ROM into allocated memory
void load_rom(invaders* si, const char* Path, Word addr) {
	// Attempt getting file to work with
	FILE *f;
	if ((f = fopen(Path, "rb")) == NULL)
	{
		printf("Error finding file\n\n");
		return;
	}

    int size = 0;
    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    fseek(f, 0L, SEEK_SET);

	// Gather all data into rom buffer
	fread(&si->memory[addr], sizeof(Byte), size, f);
	fclose(f);
    
    //memset(si->memory + 0x2000, 0, 0x16000 - 0x2000);// Zero-out RAM (0x2000 - 0x7fff)
}

int main(void) {
    // SetWindowState(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    SetTraceLogCallback(NULL);

    SetWindowState(/*FLAG_WINDOW_MAXIMIZED |*/ FLAG_WINDOW_RESIZABLE);
    InitWindow(700, 700, "GameBoy Emulator");
    SetTargetFPS(60);

    ImGuiInit(true);
    
    {
        game_boy = GetGameBoyWindow();
        settings = GetSettingsWindow();
    }

    igGetIO()->IniFilename = NULL; igGetIO()->ConfigWindowsMoveFromTitleBarOnly = true;
    Texture2D background = LoadTextureFromImage( // Load the background image
        (Image){ .data = BACKGROUND_DATA, .width = BACKGROUND_WIDTH, .height = BACKGROUND_HEIGHT, .format = BACKGROUND_FORMAT, .mipmaps = 1 }
    );

    invaders game;

    invaders_init(&game);
    game.update_screen = UpdateScreen;

    load_rom(&game, GAME_ROM, 0x0000);

    invaders_set_hiscore(&game, (Byte[2]){0, 0});

    while(!WindowShouldClose()) {

        if(IsKeyPressed(KEY_C)) {
            game.port1 |= 1 << 0; // coin
        } else if(IsKeyPressed(KEY_ENTER)) {
            game.port1 |= 1 << 2; // P1 start button
        } else if(IsKeyDown(KEY_SPACE)) {
            game.port1 |= 1 << 4; // P1 shoot button
            game.port2 |= 1 << 4; // P2 shoot button
        } else if(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            printf("Key Pressed\n");
            game.port1 |= 1 << 5; // P1 joystick left
            game.port2 |= 1 << 5; // P2 joystick left
        } else if(IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
            game.port1 |= 1 << 6; // P1 joystick right
            game.port2 |= 1 << 6; // P2 joystick right
        } else if(IsKeyPressed(KEY_T)) {
            game.port2 |= 1 << 2; // tilt
        } else if(IsKeyPressed(KEY_F9)) { // to toggle between b&w / color
            game.colored_screen = !game.colored_screen;
        }

        if(IsKeyReleased(KEY_C)) {
            game.port1 &= 0b11111110; // coin
        } else if(IsKeyReleased(KEY_ENTER)) {
            game.port1 &= 0b11111011; // P1 start button
        } else if(IsKeyReleased(KEY_SPACE)) {
            game.port1 &= 0b11101111; // P1 shoot button
            game.port2 &= 0b11101111; // P2 shoot button
        } else if(IsKeyReleased(KEY_A) || IsKeyReleased(KEY_LEFT)) {
            printf("Key released\n");
            game.port1 &= 0b11011111; // P1 joystick left
            game.port2 &= 0b11011111; // P2 joystick left
        } else if(IsKeyReleased(KEY_D) || IsKeyReleased(KEY_RIGHT)) {
            game.port1 &= 0b10111111; // P1 joystick right
            game.port2 &= 0b10111111; // P2 joystick right
        } else if(IsKeyReleased(KEY_T)) {
            game.port2 &= 0b11111011; // tilt
        }

        BeginDrawing();
        {
            ClearBackground(BLACK);
            DrawTextureScaled(background, 0, 0, GetRenderWidth(), GetRenderHeight());
            
            invaders_update(&game, 24);
            
            ImGuiBegin();
            {
                ImGuiUpdate();
            }
            ImGuiEnd();
        }
        EndDrawing();
    }

    // DE - INITIALIZATION
    UnLoadRenderTextures(game_boy->screen, settings->screen);
    UnloadTexture(background);

    ImGuiShutdown();

    CloseWindow();

    return 0;
}

static inline void ImGuiUpdate(void) {
    if(igBeginMainMenuBar()) 
    {
        if(igBeginMenu("Windows", true)) 
        {
            OpenMenuItem(GameBoyWindow, game_boy);
            OpenMenuItem(SettingsWindow, settings);

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
            // The GB screen being rendered
            igImage(
                (ImTextureID)&game_boy->screen.texture,
                (ImVec2){ (float)(game_boy->screen.texture.width + WindowScale), (float)(game_boy->screen.texture.height + WindowScale) }, 
                (ImVec2){ 0, 1 }, (ImVec2){ 1, 0 }, 
                Convert(WHITE), (ImVec4){ 0 }
            );
        game_boy->EndRender();
    }

    if(settings->open) {
        settings->BeginRender();
            igTextColored(Convert(BLUE), "General");
            igSliderInt("##", &WindowScale, GameBoyScaleRange, "GameScale: %d", ImGuiSliderFlags_None);
            if(igButton("Show Fps", (ImVec2){ 300, 30 })) ShowGbFps = !ShowGbFps;
        settings->EndRender();
    }
}
