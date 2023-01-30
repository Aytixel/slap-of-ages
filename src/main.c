#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

//gcc src/main.c src/menu.c src/menu.h -I include/windows -L lib/windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o bin/main

int main(int argc, char **argv) 
{
    menu(argc, argv);
    return 1;
}