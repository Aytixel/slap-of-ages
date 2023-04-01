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
    character_renderer_t *character_renderer = createCharacterRenderer(window, map_renderer, DAEMON_CHARACTER);
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

                    character_t *new = createCharacter(DAEMON_CHARACTER, &test_position);

                    if (canPlaceCharacter(character_renderer, new, &test_position, map_character))
                    {

                        addCharacterInMatrix(map_character, new);

                        printf("Character created\n");
                    }
                    else
                    {
                        printf("Character not created\n");
                        destroyCharacter(&new);
                    }

                    break;
                case SDL_BUTTON_RIGHT:
                    mouse_position.x = event.button.x;
                    mouse_position.y = event.button.y;

                    test_position = getTileCoord(&mouse_position, window, map_renderer);
                    if (test_position.x != -1 && test_position.y != -1 && map_character[test_position.x][test_position.y] != NULL)
                    {
                        removeCharacterFromMatrix(map_character, map_character[test_position.x][test_position.y]);
                        printf("Character removed\n");
                    }
                    break;
                }
                break;
            }
        }

        if (checkTime(main_timer))
        {

            SDL_RenderClear(window->renderer);
            renderMap(window, map_renderer);

            for (int i = 0; i < MAP_SIZE; i++)
            {
                for (int j = 0; j < MAP_SIZE; j++)
                {
                    if (map_character[i][j] != NULL)
                    {
                        // character_t *character = map_character[i][j];

                        int new_state;
                        if (/* Condition si le character move */ 1)
                        {
                            new_state = DAEMON_MOVE_ANIM;
                        }
                        else
                        {
                            new_state = DAEMON_IDLE_ANIM;
                        }

                        if (new_state != character_renderer->animation->current_state)
                        {
                            character_renderer->animation->current_state = new_state;
                        }

                        SDL_Rect destination_rect;
                        renderCharacter(window, (character_renderer_t *)character_renderer, map_character[i][j], &destination_rect);
                    }
                }
            }

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
