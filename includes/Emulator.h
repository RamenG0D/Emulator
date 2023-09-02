
#include "Gameboy.h"
#include <stdbool.h>

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define CreateMiniWindow(name) \
    Rectangle Window##name##Pos = { 0 }; bool Window##name##Exit = false; \
    Vector2 name##Offset = MousePos; \
    bool Drag##name##Window = false; \

inline void init(int, int);

inline void DrawTextureScaled(Texture2D, int, int, int, int);
inline void DrawGameScreen(const int, const int, const int, const int);
inline void read_game(const char*, Byte*, size_t);

void unload_textures(Texture, ...);

/* A neat idea but doesnt work
#ifndef CUSTOM_IMPLEMENTATION
    Gameboy gb = { 0 };

    void GameBoyInit();
#endif*/

#define ArrayLen(type) (sizeof(type*)/sizeof(type))

#define InnerWindowEvents(name) \
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !Drag##name##Window) { \
        if (CheckCollisionPointRec(MousePos, (Rectangle){ Window##name##Pos.x, Window##name##Pos.y, Window##name##Pos.width, 26 })) { \
            Drag##name##Window = true; \
            name##Offset = MousePos; \
        } \
    } \
    if(Drag##name##Window) { \
        Window##name##Pos.x = MousePos.x; \
        Window##name##Pos.y = MousePos.y; \
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) Drag##name##Window = false; \
    } \

Vector2 MousePos = { 0 };
