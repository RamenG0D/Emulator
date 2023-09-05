#!/bin/bash

gcc -o Emulator -I./includes ./src/*.c -L./libs/ -lstdc++ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11