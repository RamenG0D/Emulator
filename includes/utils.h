#define VariadicListTraverser(Type, FirstVarName, Block) \
    va_list ptr; \
    for(Type Variable = FirstVarName; !ptr; Variable = va_arg(ptr, Type)) { \
        Block; \
    }; \
    va_end(ptr);

#define GenUnloader(type) \
    void UnLoad##type##s(type val, ...) { \
        va_list ptr = NULL; \
        for(type Variable = val; ptr!=NULL; Variable = va_arg(ptr, type)) { \
            Unload##type(Variable); \
        }; \
        va_end(ptr); \
    }

typedef struct InnerWindow {
    void(*BeginRender)(void);
    void(*EndRender)(void);
    RenderTexture2D screen;
    struct InnerWindow* self;
    bool open;
} InnerWindow;

// Name is the window name, size is a Vector2(struct with float x, y) or ImVec2( (*) cast Occurs within here beware)
// BRUH this is pretty much a class... you can evevn call a method from the object...
#define CreateWindow(wname, width, height, wflags) \
    InnerWindow* Get##wname(void); \
    void wname##_begin_render(void) { \
        InnerWindow* window = Get##wname(); \
        igBegin(#wname, &window->open, wflags); \
    } \
    InnerWindow* Get##wname(void) { \
        static InnerWindow window; \
        if(!window.self) { \
            window.BeginRender = wname##_begin_render; \
            window.EndRender = igEnd; \
            window.screen = LoadRenderTexture(width, height); \
            window.open = true; \
            window.self = &window; \
        } \
        return window.self; \
    }
    
#define OpenMenuItem(wname, wobj) \
    if(igMenuItem_BoolPtr(#wname, NULL, NULL, true)) { \
        wobj->open = true; \
    }

// Purpose is to allow for checking to see if the game code (asm) has been loaded into the vRam correctly
#define Log(loglevel, msg, ...) \
    igDebugLog(Debugfmt(loglevel, TextFormat(msg, __VA_ARGS__)));

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
