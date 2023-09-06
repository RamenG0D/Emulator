#include "Types.h"
#include "utils.h"

void init(int width, int height) {
    SetWindowState(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    InitWindow(width, height, "Test");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    rlImGuiSetup(true);
}

void DrawTextureScaled(Texture2D img, int x, int y, int width, int height) {
    img.width = width; img.height = height;
    DrawTexture(img, x, y, WHITE);
}
