@echo off

set Flags=-O2 -mwindows 
set LibDir=-L./libs
set Libs=-lraylib -lgdi32 -lwinmm -lcimgui -lstdc++
set Includes=-I./includes/Raylib -I./includes
set COMILER=gcc


%COMILER% -o Emulator ./src/*.c %Flags% %Includes% %LibDir% %Libs%


