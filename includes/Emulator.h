#include "Types.h"
#include "utils.h"

#define GAMEBOY_WIDTH 256
#define GAMEBOY_HEIGHT 224

void ReadFileBytes(const char *filepath, Byte* data);

// ImGui gb window
static InnerWindow* game_boy;
// ImGui settings window
static InnerWindow* settings;
