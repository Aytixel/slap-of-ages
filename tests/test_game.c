#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "client/common.h"
#include "timer/timer.h"
#include "window/window.h"
#include "window/input.h"
#include "map/building_renderer.h"
#include "map/building.h"
#include "game/game.h"
#include "window/animation.h"
#include "window/animation_states.h"

#define TILE_SIZE 16

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
    building_renderer_t *building_renderer = createBuildingRenderer(window, map_renderer);
    frame_timer_t *main_timer = createTimer(1000 / 30);

    building_t ***map_building = createBuildingMatrix(MAP_SIZE);

    if (window == NULL || map_renderer == NULL)
        return 1;

    SDL_Point rat_position = {100, 300};

    int rat_states[] = {4, 8, 12, 4, 5, -1};

    animation_t *rat = createAnimation(
        TILE_SIZE,
        rat_states,
        loadSprite(window, "asset/sprite/characters/ratfolk_axe.png"),
        13);

    int time_left;

    SDL_Point house_1_position = {6, 2};
    SDL_Point house_2_position = {17, 8};
    SDL_Point house_3_position = {6, 14};

    building_t *house_1 = createBuilding(HOUSE_1_BUILDING, &house_1_position, window);
    building_t *house_2 = createBuilding(FIELD_BUILDING, &house_2_position, window);
    building_t *house_3 = createBuilding(MINE_BUILDING, &house_3_position, window);

    addBuildingInMatrix(map_building, house_1);
    addBuildingInMatrix(map_building, house_2);
    addBuildingInMatrix(map_building, house_3);

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
                        renderBuilding(window, building_renderer, &(map_building[i][j]->position), map_building[i][j]->type);
                    }
                }
            }

            changeAnimationState(rat, RAT_IDLE_ANIM);
            updateAnimation(rat, map_renderer->tile_size, &rat_position, window, TRANSFORM_ORIGIN_CENTER);

            SDL_RenderPresent(window->renderer);
        }
    }

    deleteTimer(&main_timer);
    destroyBuildingMatrix(&map_building);
    deleteBuildingRenderer(&building_renderer);
    deleteMapRenderer(&map_renderer);
    destroyWindow(&window);

    return 0;
}