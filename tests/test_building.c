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

#define MAP_SIZE 31

int running = 1;
int building_created = 0;
int building_to_create = 0;
int building_to_destroy = 0;

SDL_Point mouse_position;

void signalHandler(int s)
{
    running = 0;
}

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
    case SDL_MOUSEBUTTONDOWN:
        switch (event->button.button)
        {
        case SDL_BUTTON_LEFT:
            mouse_position.x = event->button.x;
            mouse_position.y = event->button.y;
            if (!building_created)
                building_to_create = 1;
            break;
        case SDL_BUTTON_RIGHT:
            if (building_created)
                building_to_destroy = 1;
            building_created = 0;
            break;
        }
        break;
    }
}

int main(int argc, char *argv[])
{
    window_t *window = createWindow("Slap of Ages", 600, 600);
    map_renderer_t *map_renderer = createMapRenderer(window, MAP_SIZE);
    frame_timer_t *main_timer = createTimer(1000 / 30);

    building_t *building;

    int time_left;

    if (window == NULL || map_renderer == NULL)
        return 1;

    while (running)
    {

        SDL_Event event;
        time_left = timeLeft(main_timer);

        if (SDL_WaitEventTimeout(&event, time_left > 0 ? time_left : 0))
            windowEventHandler(&event, window);

        if (checkTime(main_timer))
        {

            if (building_to_create)
            {
                printf("Building created\n");
                building = createBuilding(HOUSE_1_BUILDING, &mouse_position, window, map_renderer);

                building_created = 1;
                building_to_create = 0;

                printf("Building hp: %d\n", building->max_hp);
            }

            SDL_RenderClear(window->renderer);
            renderMap(window, map_renderer);

            if (building_to_destroy)
            {
                printf("Building destroyed\n");
                destroyBuilding(&building);
                building_to_destroy = 0;
            }

            if (building_created)
            {
                printf("Building created\n");
                printf("%d\n", renderBuilding(window, building->building_renderer, building->position, building->type, &building->rect));
            }
            SDL_RenderPresent(window->renderer);
        }
    }

    deleteTimer(&main_timer);
    deleteMapRenderer(&map_renderer);
    destroyBuilding(&building);
    destroyWindow(&window);

    return 0;
}