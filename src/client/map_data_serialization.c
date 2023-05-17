/**
 * @file map_data_serialization.c
 * @author Lucas Dureau
 * @brief Fonction pour transformé la carte en données pouvant être stocker ou envoyer
 * @version 1.1
 * @date 28/03/2023
 *
 */

#include "map_data_serialization.h"

extern int serializeMapData(void **data, building_t ***map_building, int gold_cost)
{
    *data = malloc(sizeof(int) + sizeof(char) * MAP_SIZE * MAP_SIZE);

    int data_size = 0;
    memcpy(*data, &gold_cost, sizeof(int));
    data_size += sizeof(int);

    char type;

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            type = map_building[i][j] != NULL && map_building[i][j]->position.x == i && map_building[i][j]->position.y == j ? map_building[i][j]->type : -1;

            memcpy(*data + data_size, &type, sizeof(char));
            data_size += sizeof(char);
        }
    }

    return data_size;
}

extern int deserializeMapData(void *data, int data_length, window_t *window, building_t ***map_building, int *gold_cost)
{
    if (data_length < sizeof(int) + sizeof(char) * MAP_SIZE * MAP_SIZE)
        return -1;

    int data_size = 0;
    memcpy(gold_cost, data, sizeof(int));
    data_size += sizeof(int);

    clearBuildingMatrix(map_building);

    char type;

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            memcpy(&type, data + data_size, sizeof(char));
            data_size += sizeof(char);

            if (type > -1)
            {
                SDL_Point tile_position = {i, j};

                addBuildingInMatrix(map_building, createBuilding(type, &tile_position, window));
            }
        }
    }

    return 0;
}