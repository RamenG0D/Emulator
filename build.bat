@echo off

g++ -o Emulator ./src/*.c -I./includes -L./libs -lcimgui -lraylib -lrlImGui -lopengl32 -lgdi32 -lwinmm -mwindows
