#define VariadicListTraverser(Type, FirstVarName, Block) \
    va_list ptr; \
    for(Type Variable = FirstVarName; !ptr; Variable = va_arg(ptr, Type)) { \
        Block; \
    }; \
    va_end(ptr);

#define GenUnloader(type) \
    void UnLoad##type##s(type val, ...) { \
        va_list ptr; \
        for(type Variable = val; !(ptr); Variable = va_arg(ptr, type)) { \
            Unload##type(Variable); \
        }; \
        va_end(ptr); \
    }

#define AutoUnloader(type, ...) \
    UnLoad##type##s(__VA_ARGS__);

typedef struct InnerWindow {
    void(*begin_render)(float, float);
    void(*end_render)(void);
    ImGuiWindowFlags flags;
    struct InnerWindow* self;
    bool open;
} InnerWindow;

// Name is the window name, size is a Vector2(struct with float x, y) or ImVec2( (*) cast Occurs within here beware)
#define CreateWindow(wname, wflags) \
    InnerWindow* Get##wname(void); \
    void wname##_begin_render(float width, float height) { \
        igSetNextWindowSize((ImVec2){ width, height }, ImGuiCond_Always); \
        InnerWindow* w = Get##wname(); \
        igBegin(#wname, &w->open, w->flags); \
    } \
    InnerWindow* Get##wname(void) { \
        static InnerWindow window; \
        if(!window.self) { \
            window.begin_render = wname##_begin_render; \
            window.end_render = igEnd; \
            window.flags = wflags; \
            window.open = true; \
            window.self = &window; \
        } \
        return window.self; \
    }

#ifdef DEBUG
    // Purpose is to allow for checking to see if the game code (asm) has been loaded into the vRam correctly
    #define Log(loglevel, msg, ...) \
        igDebugLog(Debugfmt(loglevel, msg), __VA_ARGS__);
#else
    #define Log(...)
#endif

#define LOOP_NUM 1

typedef enum {
    /*just prints your message to the log*/
    Normal,
    /*Not a "Fatal" error, just an error such "as player shouldnt be back here?" and "Item should be un-obtainable how?"*/
    Error,
    /*Will exit the program and print message out to log*/
    Panic
} LogLevel;

static inline void print_binary(Byte num) {
    while (num) {
        if (num & 1) printf("1");
        else printf("0");
        num >>= 1;
    }
}

char* Debugfmt(LogLevel LogLevel, const char* msg);
void DrawTextureScaled(Texture2D img, int x, int y, int width, int height);
