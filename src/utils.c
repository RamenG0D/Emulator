#include "Types.h"
#include "utils.h"

void DrawTextureScaled(Texture2D img, int x, int y, int width, int height) {
    img.width = width; img.height = height;
    DrawTexture(img, x, y, WHITE);
}

void prepend(char* s, const char* t)
{
    size_t len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    memcpy(s, t, len);
}

char* Debugfmt(LogLevel LogLevel, const char* msg) {
    const char* log; bool panik = false;
    switch (LogLevel) {
        case 0: log = "[LOG] "; break;
        case 1: log = "[ERROR] "; break;
        case 2: log = ""; panik = true; break;
    }
    char* result = (char*)malloc(sizeof(log)+sizeof(msg));
    result = strcpy(result, msg);
    prepend(result, log); strcat(result, "\n");
    if(panik) {
        // PAINK!
        printf("Panic on => %s\n", result);
        exit(1);
    }
    return result;
}
