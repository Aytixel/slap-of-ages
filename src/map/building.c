#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "timer/timer.h"
#include "building.h"
/**
 * @file building.c
 * @author Hôa Le Luet
 * @brief Permet la gestion des bâtiments, leur création, leur destruction
 * @version 0.1
 * @date 09/03/2023
 */

#include "building_renderer.h"
#include "window/window.h"
#include "window/input.h"

extern building_t *createBuilding(building_type_e type, SDL_Point *position, window_t *window, map_renderer_t *map_renderer)
{
    building_t *building = malloc(sizeof(building_t));

    SDL_Rect rect = {0, 0, 0, 0};

    building->type = type;
    building->building_renderer = createBuildingRenderer(window, map_renderer);
    building->rect = rect;
    building->position = position;

    switch (type)
    {
    case HOUSE_1_BUILDING:
    case HOUSE_2_BUILDING:
    case HOUSE_3_BUILDING:
    case WELL_BUILDING:
        building->max_hp = 100;
        break;
    case MILL_BUILDING:
    case MINE_BUILDING:
        building->max_hp = 150;
        break;
    case FIELD_BUILDING:
        building->max_hp = 50;
        break;
    case CORNER_WALL_BUILDING:
    case VERTICAL_WALL_BUILDING:
    case HORIZONTAL_WALL_BUILDING:
        building->max_hp = 300;

    default:
        building->max_hp = 0;
        break;
    }

    building->hp = building->max_hp;

    return building;
}

extern void destroyBuilding(building_t **building)
{

    if (*building == NULL || building == NULL)
        return;
    deleteBuildingRenderer(&((*building)->building_renderer));
    free(*building);
}

extern void buildingTakesDamages(building_t *building, int damages)
{
    building->hp -= damages;

    if (building->hp <= 0)
    {
        destroyBuilding(&building);
    }
}