#include <stdio.h>

#define GAMEBOY_WIDTH 160
#define GAMEBOY_HEIGHT 144

#include "Types.h"
#include "Gameboy.h"

void ReadFileBytes(const char *filepath, Byte *data, size_t size);
void BootGameBoy(GameBoy* gb, const char* GameFile);

// thinking of using global state now to ensure i have the Gameboy ill just init it values later
#ifndef CUSTOM_IMPLEMENTATION
    GameBoy gb = { 0 };

    // The GameBoys screen which is an image well draw into an imgui window after finishing the frame
    Image ScreenImage;
    // The Texture that acctually gets Drawn as the GB screen
    Texture ScreenTexture;

    // Clears the Image / Drawable screen to the specified color
    inline void ClearGameBoyScreen(Color color) {
        UnloadImage(ScreenImage);
        ScreenImage = GenImageColor(GAMEBOY_WIDTH, GAMEBOY_HEIGHT, color);
    }

    // only clears the screen to blank. for now at least
    void BeginDraw(void) {
        ClearGameBoyScreen(BLUE);
    }

    // Finishes by Moveing the image over to the Texture (which is what is drawn to the screen during rlImGui call)
    void EndDraw(void) {
        UnloadTexture(ScreenTexture);
        ScreenTexture = LoadTextureFromImage(ScreenImage);
    }
#endif
