#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "timer/timer.h"
#include "window/window.h"
#include "window/input.h"
#include "map/map_renderer.h"
#include "game/character_renderer.h"
#include "game/character.h"
#include "client/common.h"

int running = 1;

SDL_Point test_position = {0, 0};

SDL_Point mouse_position;

void signalHandler(int s)
{
    running = 0;
}

void windowEventHandler(SDL_Event *event, window_t *window)
{
    // gestion des évènements de la fenêtre
}

int main(int argc, char *argv[])
{
    window_t *window = createWindow("Slap of Ages", 600, 600);
    map_renderer_t *map_renderer = createMapRenderer(window);
    character_renderer_t *character_renderer = createCharacterRenderer(window, map_renderer);
    frame_timer_t *main_timer = createTimer(1000 / 30);

    character_t ***map_character = createCharacterMatrix();

    int time_left;

    if (window == NULL || map_renderer == NULL)
        return 1;

    while (running)
    {

        SDL_Event event;
        time_left = timeLeft(main_timer);

        if (SDL_WaitEventTimeout(&event, time_left > 0 ? time_left : 0))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    window->width = event.window.data1;
                    window->height = event.window.data2;
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    mouse_position.x = event.button.x;
                    mouse_position.y = event.button.y;

                    test_position = getTileCoord(&mouse_position, window, map_renderer);

                    printf("%d %d\n", test_position.x, test_position.y);

                    if (canPlaceCharacter(character_renderer, &test_position, map_character))
                        addCharacterInMatrix(map_character, createCharacter(character_renderer, DAEMON_CHARACTER, &test_position));

                    break;
                case SDL_BUTTON_RIGHT:
                    mouse_position.x = event.button.x;
                    mouse_position.y = event.button.y;
                    break;
                }
                break;
            }
        }

        if (checkTime(main_timer))
        {

            SDL_RenderClear(window->renderer);
            renderMap(window, map_renderer);
            renderCharacterMatrix(window, map_character, character_renderer);

            SDL_RenderPresent(window->renderer);
        }
    }

    deleteTimer(&main_timer);
    destroyCharacterMatrix(&map_character);
    deleteCharacterRenderer(&character_renderer);
    deleteMapRenderer(&map_renderer);
    destroyWindow(&window);

    return 0;
}
