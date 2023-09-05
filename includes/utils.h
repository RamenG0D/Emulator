
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
