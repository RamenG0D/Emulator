#include <stdio.h>

#include "Types.h"
#include "utils.h"

#define GAMEBOY_WIDTH 160
#define GAMEBOY_HEIGHT 144

void ReadFileBytes(const char *filepath, Byte *data, size_t size);

// The GameBoys screen which is an image well draw into an imgui window after finishing the frame
// Image ScreenImage;
// The Texture that acctually gets Drawn as the GB screen
// Texture ScreenTexture;

// Clears the Image / Drawable screen to the specified color
// inline void ClearGameBoyScreen(Color color) {
    // UnloadImage(ScreenImage); ScreenImage = GenImageColor(GAMEBOY_WIDTH, GAMEBOY_HEIGHT, color);
// }
