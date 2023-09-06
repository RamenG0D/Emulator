#include "Types.h"
#include "utils.h"

void init(int width, int height) {
    InitWindow(width, height, "Test");

    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    SetTargetFPS(60);
}

void UnLoadTextures(Texture texture, ...) {
    VariadicListTraverser(Texture, texture, {
        UnloadTexture(Variable);
    });
}

void DrawTextureScaled(Texture2D img, int x, int y, int width, int height) {
    img.width = width; img.height = height;
    DrawTexture(img, x, y, WHITE);
}
