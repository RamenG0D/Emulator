
# My First Project to majorly involve C

This projects goal is to emulate the oh so popular gameboy :D
The project will use Raylib but everything else will be created for the project.

- Goal
  - Get virtual gameboy to execute code
    - Emulate the Gameboy `cpu`
      - Handle `OpCodes`
      - utilize the registers
    - Read code from Memory
      - Figure out next step?
    - ~~Draw the Screen~~
      - Figure out next step?
    - Get virtual gameboy to play `pokemon red`
      - ~~Read Game Rom into memory~~
      - Figure out next step?

## Build

to build the project you will need mingw with gcc at the bare minnimum, but i chose to use [w64-devkit](https://github.com/skeeto/w64devkit) because of its ease of use it will include all the programs required to build the project.

First we need to clone the project so run

```cmd
git clone https://github.com/drplague1/Emulator.git
```

Then move into the directory by running

```cmd
cd Emulator
```

Now we can build the program and get the executable by running (no linux build at the moment)

```cmd
./build.bat
```

Which will produce an executable called Emulator which is the program

## Side notes

the project also uses imgui via an adapter called cimgui and another adapter / bridge so that imgui can be used with raylib, called c99-raylib-cimgui-template

## Resources

hopefully this might be usefull to someone else in the future.

P.S. Probably me...

[Build-System](https://github.com/skeeto/w64devkit)

[Main GB Resource](https://github.com/gb-archive/salvage/blob/master/misc/8bit_wonderland.pdf)

[Raylib](https://github.com/raysan5/raylib)

[c99-raylib-cimgui-template](https://github.com/WEREMSOFT/c99-raylib-cimgui-template)
