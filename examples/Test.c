
#include "ImGuiRaylib.h"

void update_frame() {
    
    BeginDrawing();
    {
        ImGuiBegin();

        igBeginMainMenuBar();
            if(igBeginMenu("Files", true)) {
                igEndMenu();
            }
        igEndMainMenuBar();

        ClearBackground(RED);

        igShowDemoWindow(NULL);

        ImGuiEnd();
    }
    EndDrawing();
}

int main(void) {
    // Init Raylib
    InitWindow(600, 600, "This is a imgui test");
    SetTargetFPS(60);

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Initialize keyboard and mouse events
    ImGuiInit(true);

    igGetIO()->ConfigWindowsMoveFromTitleBarOnly = true;

    while (!WindowShouldClose())
    {
        update_frame();
    }
    CloseWindow();

    return 0;
}
