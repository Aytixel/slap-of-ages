#include <stdio.h>
#include <assert.h>
#include "timer/timer.h"

int main()
{
    printf("Test de timer/timer.h\n\n");

    frame_timer_t *timer = createTimer(1000 / 30);

    assert(timer != NULL);
    printf("Création du timer OK\n");

    assert(timeLeft(timer) > 0);
    printf("\n\tTemps restant, avant OK\n");
    assert(!checkTime(timer));
    printf("\tTest du timer, avant OK\n");

    printf("\nAttend 1 seconde\n");
    sleepMs(1000);

    assert(timeLeft(timer) < 0);
    printf("\n\tTemps restant, après OK\n");
    assert(checkTime(timer) > 0);
    printf("\tTest du timer, après OK\n");

    assert(timeLeft(timer) > 0);
    printf("\n\tTemps restant, avant OK\n");
    assert(!checkTime(timer));
    printf("\tTest du timer, avant OK\n");

    assert(timer->frame_per_second > 0.95 && timer->frame_per_second < 1.05);
    printf("\n\tImage par seconde OK\n");

    assert(!deleteTimer(&timer));
    assert(timer == NULL);
    printf("\nDestruction du timer OK\n");

    printf("\nTest de timer/timer.h succès\n");

    return 0;
}