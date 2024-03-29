/**
 * @file building.c
 * @author Hôa Le Luet, Lucas Dureau
 * @brief Permet la gestion des bâtiments, leur création, leur destruction
 * @version 1.1
 * @date 09/03/2023
 */

#include "building.h"

extern building_t *createBuilding(building_type_e type, SDL_Point *position, window_t *window)
{
    building_t *building = malloc(sizeof(building_t));

    building->type = type;
    building->position = *position;

    switch (type)
    {
    case HOUSE_1_BUILDING:
    case HOUSE_2_BUILDING:
    case HOUSE_3_BUILDING:
    case WELL_BUILDING:
        building->max_hp = 100;
        break;
    case MINE_BUILDING:
        building->max_hp = 210;
        break;
    case MILL_BUILDING:
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

extern int getBuildingGoldCost(building_type_e type)
{
    switch (type)
    {
    case WELL_BUILDING:
    case HOUSE_2_BUILDING:
        return 40;
    case HOUSE_1_BUILDING:
        return 20;
    case MILL_BUILDING:
    case MINE_BUILDING:
    case HOUSE_3_BUILDING:
        return 100;
    case CORNER_WALL_BUILDING:
    case VERTICAL_WALL_BUILDING:
    case HORIZONTAL_WALL_BUILDING:
    case FIELD_BUILDING:
        return 10;
    default:
        return 0;
    }
}

extern building_t ***createBuildingMatrix()
{
    building_t ***building_matrix = malloc(sizeof(building_t **) * MAP_SIZE);

    for (int i = 0; i < MAP_SIZE; i++)
    {
        building_matrix[i] = malloc(sizeof(building_t *) * MAP_SIZE);
    }

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            building_matrix[i][j] = NULL;
        }
    }

    return building_matrix;
}

extern void renderBuildingMatrix(window_t *window, building_t ***map_building, building_renderer_t *building_renderer)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            building_t *building = getBuilding(map_building, i, j);

            if (building != NULL)
            {
                if (building->position.x == i && building->position.y == j)
                    renderBuilding(window, building_renderer, &building->position, building->type);

                if (building->type == CORNER_WALL_BUILDING ||
                    building->type == VERTICAL_WALL_BUILDING ||
                    building->type == HORIZONTAL_WALL_BUILDING)
                {
                    building_t *top_building = getBuilding(map_building, i, j - 1);

                    if (top_building != NULL &&
                        top_building != building &&
                        (top_building->type == CORNER_WALL_BUILDING ||
                         top_building->type == VERTICAL_WALL_BUILDING ||
                         top_building->type == HORIZONTAL_WALL_BUILDING))
                    {
                        SDL_Point position = {i, j - 1};

                        renderBuilding(window, building_renderer, &position, VERTICAL_SPACE_FILLER_WALL_BUILDING);
                    }

                    building_t *left_building = getBuilding(map_building, i - 1, j);

                    if (left_building != NULL &&
                        left_building != building &&
                        (left_building->type == CORNER_WALL_BUILDING ||
                         left_building->type == VERTICAL_WALL_BUILDING ||
                         left_building->type == HORIZONTAL_WALL_BUILDING))
                    {
                        SDL_Point position = {i - 1, j};

                        renderBuilding(window, building_renderer, &position, HORIZONTAL_SPACE_FILLER_WALL_BUILDING);
                    }
                }
            }
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

extern int buildingTakesDamages(client_game_data_t *game_data, building_t *building, int damages)
{
    building->hp -= damages;

    if (building->hp <= 0)
    {
        game_data->opponent_gold_cost -= getBuildingGoldCost(building->type);
        removeBuildingFromMatrix(game_data->opponent_map_building, building);

        return 1;
    }

    return 0;
}

extern void clearBuildingMatrix(building_t ***building_matrix)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            building_t *building = getBuilding(building_matrix, i, j);

            if (building != NULL)
                removeBuildingFromMatrix(building_matrix, building);
        }
    }
}

extern void destroyBuildingMatrix(building_t ****building_matrix)
{
    clearBuildingMatrix(*building_matrix);

    for (int i = 0; i < MAP_SIZE; i++)
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

extern int canPlaceBuilding(building_renderer_t *building_renderer, building_type_e type, SDL_Point *position, building_t ***building_matrix)
{

    if (canRenderBuilding(building_renderer, position, type))
    {
        if (getBuildingWithPoint(building_matrix, position) == NULL)
        {
            switch (type)
            {
            case MILL_BUILDING:
            case VERTICAL_WALL_BUILDING:
                if (getBuilding(building_matrix, position->x, position->y + 1) == NULL)
                    return 1;
                break;

            case HORIZONTAL_WALL_BUILDING:
                if (getBuilding(building_matrix, position->x + 1, position->y) == NULL)
                    return 1;
                break;

            default:
                return 1;
            }
        }
    }
    return 0;
}

extern building_t *getNearestBuilding(building_t ***building_matrix, SDL_Point *position, int wall)
{
    building_t *nearest_building = NULL;
    float nearest_distance = 0;

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            building_t *building = getBuilding(building_matrix, i, j);

            if (building != NULL)
            {
                if (wall == 0 &&
                    (building->type == CORNER_WALL_BUILDING ||
                     building->type == VERTICAL_WALL_BUILDING ||
                     building->type == HORIZONTAL_WALL_BUILDING))
                    continue;

                float distance = sqrtf(powf(building->position.x - position->x, 2) + powf(building->position.y - position->y, 2));

                if (nearest_building == NULL || distance < nearest_distance)
                {
                    nearest_building = building;
                    nearest_distance = distance;
                }
            }
        }
    }

    return nearest_building;
}

extern building_t *getBuilding(building_t ***building_matrix, int x, int y)
{
    if (x < 0 || y < 0 || x >= MAP_SIZE || y >= MAP_SIZE)
        return NULL;

    return building_matrix[x][y];
}

extern building_t *getBuildingWithPoint(building_t ***building_matrix, SDL_Point *position)
{
    return getBuilding(building_matrix, position->x, position->y);
}

extern void buildingEventHandler(SDL_Event *event, client_game_data_t *game_data, building_renderer_t *building_renderer, window_t *window)
{
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        SDL_Point mouse_position = {event->button.x, event->button.y};
        SDL_Point tile_position = getTileCoord(&mouse_position, window, building_renderer->map_renderer);
        building_t *building = NULL;
        int gold_cost = getBuildingGoldCost(game_data->selected_building_type);

        if (
            canPlaceBuilding(building_renderer, game_data->selected_building_type, &tile_position, game_data->map_building) &&
            game_data->gold_count - game_data->gold_cost - gold_cost >= 0)
        {
            addBuildingInMatrix(game_data->map_building, createBuilding(game_data->selected_building_type, &tile_position, window));
            game_data->gold_cost += gold_cost;
        }
        else if ((building = getBuildingWithPoint(game_data->map_building, &tile_position)) != NULL)
        {
            game_data->gold_cost -= getBuildingGoldCost(building->type);
            removeBuildingFromMatrix(game_data->map_building, building);
        }

        serializeGameData(game_data);
    }
}