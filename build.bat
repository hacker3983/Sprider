@echo off
echo Building Sprider - Chat Application...
gcc -c sprider-*.c
gcc main.c *.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o Sprider
echo Running Sprider - Chat application...
sprider.exe