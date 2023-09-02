@echo off

gcc -o Emulator -I./includes ./src/*.c -L./libs/ -lraylib -lopengl32 -lgdi32 -lwinmm 
REM -mwindows
