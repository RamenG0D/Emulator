
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

// Name is the window name, size is a Vector2(struct with float x, y) or ImVec2( (*) cast Occurs within here beware)
#define CreateWindow(name, args, Block, ...) \
    typedef void(*name##RenderFunc)args; \
    void Render args { \
        Block; \
        igBegin(#name, NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration); \
        __VA_ARGS__ \
        igEnd(); \
    } \
    typedef struct { \
        name##RenderFunc render; \
    } name##Window; \
    name##Window name = { Render };

// Purpose is to allow for checking to see if the game code (asm) has been loaded into the vRam correctly
#define Log(loglevel, msg, ...) \
    char* tmp = Debugfmt(loglevel, msg); \
    igDebugLog(tmp, __VA_ARGS__); \
    free(tmp);

typedef enum {
    /*just prints your message to the log*/
    Normal,
    /*Not a "Fatal" error, just an error such "as player shouldnt be back here?" and "Item should be un-obtainable how?"*/
    Error,
    /*Will exit the program and print message out to log*/
    Panic
} LogLevel;

char* Debugfmt(LogLevel LogLevel, const char* msg);
void init(int width, int height);
void DrawTextureScaled(Texture2D img, int x, int y, int width, int height);

// Needed Due to issues with combining cimgui and rlImgui
inline void rlImGuiFixed(ImGuiIO* io, void(*func)(void)) {
    io->MousePos.x = (float)GetMouseX();
    io->MousePos.y = (float)GetMouseY();
    io->MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    io->MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    io->MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);
    {
        Vector2 mouseWheel = GetMouseWheelMoveV();io->MouseWheel += mouseWheel.y;io->MouseWheelH += mouseWheel.x;
    }
    if (IsWindowFullscreen()) {
        int monitor = GetCurrentMonitor(); io->DisplaySize.x = (float)(GetMonitorWidth(monitor)); io->DisplaySize.y = (float)(GetMonitorHeight(monitor));
    } else {
        io->DisplaySize.x = (float)(GetScreenWidth()); io->DisplaySize.y = (float)(GetScreenHeight());
    }
    int width = (int)(io->DisplaySize.x), height = (int)(io->DisplaySize.y);
    if (width > 0 && height > 0) {
        io->DisplayFramebufferScale = (ImVec2){width / io->DisplaySize.x, height / io->DisplaySize.y};
    } else {
        io->DisplayFramebufferScale = (ImVec2){1.0f, 1.0f};
    }

    for(int i = 0; i < 20; ++i) {
        rlImGuiBeginDelta(1.0f / GetFrameTime());
        func();
        rlImGuiEnd();
    }
}
