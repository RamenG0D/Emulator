
#define ArrayLen(type) (sizeof(type*)/sizeof(type))

#define VariadicListTraverser(Type, FirstVarName, Block) \
    va_list ptr; \
    for(Type Variable = FirstVarName; ptr != NULL; Variable = va_arg(ptr, Type)) { \
        Block; \
    }; va_end(ptr);

void init(int, int);
void DrawTextureScaled(Texture2D, int, int, int, int);
void DrawGameScreen(const int, const int, const int, const int);
void UnLoadTextures(Texture, ...);

#define CreateMiniWindow(Name, mpos, x, y, width, height) \
    Window Name = { \
        (Rectangle){ x, y, width, height }, \
        mpos, \
        false, \
        false \
    };

#define InnerWindowEvents(name) \
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !name.DragWindow) { \
        if (CheckCollisionPointRec(MousePos, (Rectangle){ name.WindowPos.x-12, name.WindowPos.y, name.WindowPos.width, 26 })) { \
            name.DragWindow = true; \
            name.Offset = MousePos; \
        } \
    } \
    if(name.DragWindow) { \
        name.WindowPos.x = MousePos.x; \
        name.WindowPos.y = MousePos.y; \
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) name.DragWindow = false; \
    }
