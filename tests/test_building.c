#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "timer/timer.h"
#include "window/window.h"
#include "window/input.h"
#include "map/building_renderer.h"
#include "map/building.h"

#define MAP_SIZE 20

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
    map_renderer_t *map_renderer = createMapRenderer(window, MAP_SIZE);
    frame_timer_t *main_timer = createTimer(1000 / 30);

    building_t ***map_building = createBuildingMatrix(MAP_SIZE);

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

                    building_t *new = createBuilding(MILL_BUILDING, &test_position, window, map_renderer);

                    if (canPlaceBuilding(new, &test_position, map_building))
                    {

                        addBuildingInMatrix(map_building, new);

                        printf("Building created\n");
                    }
                    else
                    {
                        printf("Building not created\n");
                        destroyBuilding(&new);
                    }

                    break;
                case SDL_BUTTON_RIGHT:
                    mouse_position.x = event.button.x;
                    mouse_position.y = event.button.y;

                    test_position = getTileCoord(&mouse_position, window, map_renderer);
                    if (test_position.x != -1 && test_position.y != -1 && map_building[test_position.x][test_position.y] != NULL)
                    {
                        removeBuildingFromMatrix(map_building, map_building[test_position.x][test_position.y]);
                        printf("Building destroyed\n");
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
                    if (map_building[i][j] != NULL)
                    {
                        renderBuilding(window, map_building[i][j]->building_renderer, &(map_building[i][j]->position), map_building[i][j]->type, &map_building[i][j]->rect);
                    }
                }
            }

            SDL_RenderPresent(window->renderer);
        }
    }

    deleteTimer(&main_timer);
    destroyBuildingMatrix(&map_building, MAP_SIZE);
    deleteMapRenderer(&map_renderer);
    destroyWindow(&window);

    return 0;
}