@echo off

gcc -c invaders.c -I.\includes\Emulator

ar rvs invaders.lib invaders.o

rm *.o