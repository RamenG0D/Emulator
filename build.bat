@echo off

gcc -o Emulator ./src/*.c -I./includes -L./libs/ -lraylib -lopengl32 -lgdi32 -lwinmm 
REM -mwindows
