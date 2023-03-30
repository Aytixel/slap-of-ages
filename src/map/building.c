/**
 * @file building.c
 * @author Hôa Le Luet
 * @brief Permet la gestion des bâtiments, leur création, leur destruction
 * @version 0.1
 * @date 09/03/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "timer/timer.h"
#include "building.h"
#include "building_renderer.h"
#include "window/window.h"
#include "window/input.h"

extern building_t *createBuilding(building_type_e type, SDL_Point *position, window_t *window)
{
    building_t *building = malloc(sizeof(building_t));

    SDL_Rect rect = {0, 0, 0, 0};

    building->type = type;
    building->rect = rect;
    building->position = *position;

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

    switch (type)
    {
    case HOUSE_1_BUILDING:
    case HOUSE_2_BUILDING:
    case HOUSE_3_BUILDING:
        building->gold_cost = 30;
        break;
    case WELL_BUILDING:
        building->gold_cost = 20;
        break;
    case MILL_BUILDING:
    case MINE_BUILDING:
        building->gold_cost = 100;
        break;
    case CORNER_WALL_BUILDING:
    case VERTICAL_WALL_BUILDING:
    case HORIZONTAL_WALL_BUILDING:
    case FIELD_BUILDING:
        building->gold_cost = 10;
        break;
    default:
        building->gold_cost = 5;
        break;
    }

    return building;
}

extern building_t ***createBuildingMatrix(int map_size)
{
    building_t ***building_matrix = malloc(sizeof(building_t **) * map_size);

    for (int i = 0; i < map_size; i++)
    {
        building_matrix[i] = malloc(sizeof(building_t *) * map_size);
    }

    for (int i = 0; i < map_size; i++)
    {
        for (int j = 0; j < map_size; j++)
        {
            building_matrix[i][j] = NULL;
        }
    }

    return building_matrix;
}

extern void renderBuildingMatrix(window_t *window, building_t ***map_building, building_renderer_t *building_renderer, int map_size)
{
    for (int i = 0; i < map_size; i++)
    {
        for (int j = 0; j < map_size; j++)
        {
            if (map_building[i][j] != NULL)
                renderBuilding(window, building_renderer, &(map_building[i][j]->position), map_building[i][j]->type, &map_building[i][j]->rect);
        }
    }
}

extern void destroyBuilding(building_t **building)
{

    if (*building == NULL || building == NULL)
        return;
    free(*building);
    *building = NULL;
}

extern void buildingTakesDamages(building_t ***matrix, building_t *building, int damages)
{
    building->hp -= damages;

    if (building->hp <= 0)
    {
        removeBuildingFromMatrix(matrix, building);
    }
}

extern void clearMatrix(building_t ***building_matrix, int map_size)
{
    for (int i = 0; i < map_size; i++)
    {
        for (int j = 0; j < map_size; j++)
        {
            if (building_matrix[i][j] != NULL)
            {
                removeBuildingFromMatrix(building_matrix, building_matrix[i][j]);
            }
        }
    }
}

extern void destroyBuildingMatrix(building_t ****building_matrix, int map_size)
{
    clearMatrix(*building_matrix, map_size);

    for (int i = 0; i < map_size; i++)
    {
        free((*building_matrix)[i]);
    }

    free(*building_matrix);
    *building_matrix = NULL;
}

extern void addBuildingInMatrix(building_t ***building_matrix, building_t *building)
{

    switch (building->type)
    {
    case MILL_BUILDING:
    case VERTICAL_WALL_BUILDING:
        building_matrix[building->position.x][building->position.y] = building;
        building_matrix[building->position.x][building->position.y + 1] = building;
        break;

    case HORIZONTAL_WALL_BUILDING:
        building_matrix[building->position.x][building->position.y] = building;
        building_matrix[building->position.x + 1][building->position.y] = building;
        break;

    default:
        building_matrix[building->position.x][building->position.y] = building;
        break;
    }
}

extern void removeBuildingFromMatrix(building_t ***building_matrix, building_t *building)
{
    if (building == NULL)
        return;

    switch (building->type)
    {
    case MILL_BUILDING:
    case VERTICAL_WALL_BUILDING:
        building_matrix[building->position.x][building->position.y + 1] = NULL;
        destroyBuilding(&(building_matrix[building->position.x][building->position.y]));
        break;

    case HORIZONTAL_WALL_BUILDING:
        building_matrix[building->position.x + 1][building->position.y] = NULL;
        destroyBuilding(&(building_matrix[building->position.x][building->position.y]));
        break;

    default:
        destroyBuilding(&(building_matrix[building->position.x][building->position.y]));
        break;
    }
}

extern void updateBuildingCoord(building_t *building, SDL_Point *position)
{
    building->position = *position;
}

extern int canPlaceBuilding(building_renderer_t *building_renderer, building_t *building, SDL_Point *position, building_t ***building_matrix)
{

    if (canRenderBuilding(building_renderer, position, building->type))
    {
        if (building_matrix[position->x][position->y] == NULL)
        {
            switch (building->type)
            {
            case MILL_BUILDING:
            case VERTICAL_WALL_BUILDING:
                if (building_matrix[position->x][position->y + 1] == NULL)
                {
                    return 1;
                }
                break;

            case HORIZONTAL_WALL_BUILDING:
                if (building_matrix[position->x + 1][position->y] == NULL)
                {
                    return 1;
                }
                break;

            default:
                return 1;
                break;
            }
        }
    }
    return 0;
}

extern building_t *getBuilding(building_t ***building_matrix, SDL_Point *position)
{
    return building_matrix[position->x][position->y];
}

extern void buildingEventHandler(SDL_Event *event, client_game_data_t *game_data, building_renderer_t *building_renderer, window_t *window)
{
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        SDL_Point mouse_position = {event->button.x, event->button.y};
        SDL_Point tile_position = getTileCoord(&mouse_position, window, building_renderer->map_renderer);
        building_t *new = createBuilding(game_data->selected_building_type, &tile_position, window);

        if (canPlaceBuilding(building_renderer, new, &tile_position, game_data->map_building))
        {
            if (game_data->gold_count - game_data->gold_cost - new->gold_cost >= 0)
            {
                addBuildingInMatrix(game_data->map_building, new);
                game_data->gold_cost += new->gold_cost;
            }
            else
                destroyBuilding(&new);
        }
        else if (tile_position.x != -1 && tile_position.y != -1 && game_data->map_building[tile_position.x][tile_position.y] != NULL)
        {
            game_data->gold_cost -= game_data->map_building[tile_position.x][tile_position.y]->gold_cost;
            destroyBuilding(&new);
            removeBuildingFromMatrix(game_data->map_building, game_data->map_building[tile_position.x][tile_position.y]);
        }
        else
            destroyBuilding(&new);
    }
}