
#define GenUnloader(type) \
    inline void UnLoad##type##s(type val, ...) { \
        VariadicListTraverser(type, val, { \
            Unload##type(Variable); \
        }); \
    }

#define AutoUnloader(type, ...) \
    UnLoad##type##s(__VA_ARGS__);

#define ArrayLen(type) (sizeof(type*)/sizeof(type))

#define VariadicListTraverser(Type, FirstVarName, Block) \
    va_list ptr; \
    for(Type Variable = FirstVarName; ptr != NULL; Variable = va_arg(ptr, Type)) { \
        Block; \
    }; \
    va_end(ptr);

void init(int width, int height);
void DrawTextureScaled(Texture2D img, int x, int y, int width, int height);

#define WindowEvents(name) \
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { \
        Vector2 mPos = GetMousePosition(); \
        if (CheckCollisionPointRec(mPos, (Rectangle){ name.windowRect.x, name.windowRect.y, name.windowRect.width, 18 })) { \
            name.isDragging = true; \
        } else if (CheckCollisionPointRec( mPos, (Rectangle){ (name.windowRect.x + name.windowRect.width)-4, name.windowRect.y, 20, 18 })) { \
            name.open = false; \
        } \
    } \
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { \
        name.isDragging = false; \
    } \
    if (name.isDragging) { \
        name.windowRect.x += GetMouseDelta().x; \
        name.windowRect.y += GetMouseDelta().y; \
    }

#define CreateWindow(name, ix, iy, args, Block) \
    typedef struct { \
        Rectangle windowRect; \
        bool isDragging; \
        bool open; \
    } name##Window; \
    name##Window name = \
    { \
        (Rectangle){ ix, iy, GAMEBOY_WIDTH, GAMEBOY_HEIGHT }, \
        false, \
        true \
    }; \
    void name##WindowEvents args { \
        if(name.open) { \
            igSetNextWindowContentSize((ImVec2){ name.windowRect.width, name.windowRect.height }); \
        } else { \
            igSetNextWindowContentSize((ImVec2){ name.windowRect.width, name.windowRect.height-GAMEBOY_HEIGHT }); \
        } \
        igSetNextWindowPos( (ImVec2){ name.windowRect.x, name.windowRect.y }, ImGuiCond_Always, (ImVec2){ 0 } ); \
        igBegin(#name, &name.open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings); \
            WindowEvents(name); \
            Block; \
        igEnd(); \
    }
