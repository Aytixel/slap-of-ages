#include <stdio.h>
#include <assert.h>
#include "timer/timer.h"
#include "window/window.h"

int running = 1;

void windowEventHandler(SDL_Event *event, window_t *window)
{
    // gestion des évènements de la fenêtre
    switch (event->type)
    {
    case SDL_QUIT:
        running = 0;
        break;
    case SDL_WINDOWEVENT:
        switch (event->window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
            window->width = event->window.data1;
            window->height = event->window.data2;
            break;
        }
        break;
    }
}

int main(int argc, char *argv[])
{
    printf("Test de window/window.h\n\n");

    window_t *window = createWindow("Test Window", 500, 400);

    assert(window != NULL);
    printf("Création du window OK\n");

    frame_timer_t *main_timer = createTimer(1000 / 60);

    assert(loadSprite(window, "test") == NULL);
    printf("\n\tEssaie de chargement d'un sprite inexistant OK\n");

    sprite_t *sprite = loadSprite(window, "asset/PixelBooksVers1.0/RADL_Book4.png");

    assert(sprite != NULL);
    printf("\tChargement d'un sprite OK\n");

    // boucle principale
    while (running)
    {
        SDL_Event event;
        int time_left = timeLeft(main_timer);

        if (SDL_WaitEventTimeout(&event, time_left > 0 ? time_left : 0))
            windowEventHandler(&event, window);

        if (checkTime(main_timer))
        {
            SDL_RenderClear(window->renderer);

            SDL_Rect dest = {window->width / 2 - sprite->surface->w / 2, window->height / 2 - sprite->surface->h / 2, sprite->surface->w, sprite->surface->h};

            SDL_RenderCopy(window->renderer, sprite->texture, NULL, &dest);

            SDL_RenderPresent(window->renderer);
        }
    }

    deleteTimer(&main_timer);

    assert(!destroySprite(&sprite));
    assert(sprite == NULL);
    printf("\n\tDestruction du sprite OK\n");

    assert(!destroyWindow(&window));
    assert(window == NULL);
    printf("\nDestruction de window OK\n");

    printf("\nTest de window/window.h succès\n");

    return 0;
}