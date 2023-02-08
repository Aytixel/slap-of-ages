#include <stdio.h>
#include <assert.h>
#include "timer.h"
#include "window.h"

int main(int argc, char *argv[])
{
    printf("Test de window.h\n\n");

    window_t *window = createWindow("Test Window", 500, 400);

    assert(window != NULL);
    printf("Création du window OK\n");

    sleepMs(3000);

    assert(!destroyWindow(&window));
    assert(window == NULL);
    printf("\nDestruction du window OK\n");

    printf("\nTest de window.h succès\n");
}