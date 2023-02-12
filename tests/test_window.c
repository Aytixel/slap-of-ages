#include <stdio.h>
#include <assert.h>
#include "timer.h"
#include "window.h"

int running = 1;

void windowEventHandler(SDL_Event *event)
{
    // gestion des évènements de la fenêtre
    switch (event->type)
    {
    case SDL_QUIT:
        running = 0;
        break;
    }
}

int main(int argc, char *argv[])
{
    printf("Test de window.h\n\n");

    window_t *window = createWindow("Test Window", 500, 400);

    assert(window != NULL);
    printf("Création du window OK\n");

    frame_timer_t *main_timer = createTimer(1000 / 60);

    // boucle principale
    while (running)
    {
        SDL_Event event;
        int time_left = timeLeft(main_timer);

        if (SDL_WaitEventTimeout(&event, time_left > 0 ? time_left : 0))
            windowEventHandler(&event);

        if (checkTime(main_timer))
        {
            SDL_RenderClear(window->renderer);

            SDL_RenderPresent(window->renderer);
        }
    }

    deleteTimer(&main_timer);

    assert(!destroyWindow(&window));
    assert(window == NULL);
    printf("\nDestruction du window OK\n");

    printf("\nTest de window.h succès\n");

    return 0;
}