#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

//gcc src/main.c src/menu.c src/menu.h -I include/windows -L lib/windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o bin/main

int main(int argc, char **argv) 
{
    int i;
    printf("Marqué 1 pour le menu principal, 2 pour le menu multi-joueur: ");
    scanf("%d", &i);
    if(i == 1)
    {
        menu_principal(argc, argv);
    }
    else if(i == 2)
    {
        menu_multi(argc, argv);
    }
    else
    {
        printf("Erreur, veuillez réessayer");
    }
    return 1;
}