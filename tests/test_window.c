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
    printf("\n\tEssaie de chargement d'un sprite d'image inexistante OK\n");

    sprite_t *image_sprite = loadSprite(window, "asset/PixelBooksVers1.0/RADL_Book4.png");

    assert(image_sprite != NULL);
    printf("\tChargement d'un sprite d'image OK\n");

    TTF_Font *font = TTF_OpenFont("asset/font/8-BIT-WONDER.ttf", 10);
    SDL_Color text_color = {52, 36, 20, 0};
    sprite_t *text_sprite = createTextSprite(window, font, "test", text_color);

    assert(text_sprite != NULL);
    printf("\n\tCréation du sprite d'un texte OK\n");

    SDL_Rect test_dest = surfaceFromCenter(window, text_sprite->surface, -35, -20, TRANSFORM_ORIGIN_CENTER);

    printf("\n\tTest de la fonction surfaceFromCenter :\n");

    assert(window->width / 2 - text_sprite->surface->w / 2 - 35 == test_dest.x);
    assert(window->height / 2 - text_sprite->surface->h / 2 - 20 == test_dest.y);
    printf("\n\t\tPoint d'origine centré OK\n");

    test_dest = surfaceFromCenter(window, text_sprite->surface, -35, -20, TRANSFORM_ORIGIN_TOP_LEFT);

    assert(window->width / 2 - 35 == test_dest.x);
    assert(window->height / 2 - 20 == test_dest.y);
    printf("\t\tPoint d'origine en haut à gauche OK\n");

    test_dest = surfaceFromCenter(window, text_sprite->surface, -35, -20, TRANSFORM_ORIGIN_TOP_RIGHT);

    assert(window->width / 2 - text_sprite->surface->w - 35 == test_dest.x);
    assert(window->height / 2 - 20 == test_dest.y);
    printf("\t\tPoint d'origine en haut à droite OK\n");

    test_dest = surfaceFromCenter(window, text_sprite->surface, -35, -20, TRANSFORM_ORIGIN_BOTTOM_LEFT);

    assert(window->width / 2 - 35 == test_dest.x);
    assert(window->height / 2 - text_sprite->surface->h - 20 == test_dest.y);
    printf("\t\tPoint d'origine en bas à gauche OK\n");

    test_dest = surfaceFromCenter(window, text_sprite->surface, -35, -20, TRANSFORM_ORIGIN_BOTTOM_RIGHT);

    assert(window->width / 2 - text_sprite->surface->w - 35 == test_dest.x);
    assert(window->height / 2 - text_sprite->surface->h - 20 == test_dest.y);
    printf("\t\tPoint d'origine en bas à droite OK\n");

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

            SDL_Rect image_dest = surfaceToCenter(window, image_sprite->surface);

            assert(window->width / 2 - image_sprite->surface->w / 2 == image_dest.x);
            assert(window->height / 2 - image_sprite->surface->h / 2 == image_dest.y);

            SDL_RenderCopy(window->renderer, image_sprite->texture, NULL, &image_dest);

            SDL_Rect text_dest = surfaceFromCenter(window, text_sprite->surface, -35, -20, TRANSFORM_ORIGIN_CENTER);

            assert(window->width / 2 - text_sprite->surface->w / 2 - 35 == text_dest.x);
            assert(window->height / 2 - text_sprite->surface->h / 2 - 20 == text_dest.y);

            SDL_RenderCopy(window->renderer, text_sprite->texture, NULL, &text_dest);

            SDL_RenderPresent(window->renderer);
        }
    }

    deleteTimer(&main_timer);

    assert(!destroySprite(&text_sprite));
    assert(text_sprite == NULL);
    printf("\n\tDestruction du sprite d'un texte OK\n");

    TTF_CloseFont(font);

    assert(!destroySprite(&image_sprite));
    assert(image_sprite == NULL);
    printf("\n\tDestruction du sprite d'image OK\n");

    assert(!destroyWindow(&window));
    assert(window == NULL);
    printf("\nDestruction de window OK\n");

    printf("\nTest de window/window.h succès\n");

    return 0;
}