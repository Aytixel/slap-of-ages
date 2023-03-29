/**
 * @file game_data.c
 * @author Lucas Dureau
 * @brief Implémente les fonctions associées la structure de données contenant les données du jeux
 * @version 0.1
 * @date 02/03/2023
 *
 */

#include <stdlib.h>
#include <string.h>
#include "game_data.h"

extern client_game_data_t *createGameData(int map_size)
{
    client_game_data_t *game_data = malloc(sizeof(client_game_data_t));

    game_data->port = 4539;
    game_data->state = CONNECTION_GAME_STATE;
    game_data->map_building = createBuildingMatrix(map_size);
    game_data->gold_count = 1000;
    game_data->gold_cost = 0;
    game_data->win_count = 0;
    game_data->timer = NULL;
    game_data->opponent_pseudo = NULL;
    game_data->opponent_gold_cost = 0;
    game_data->opponent_map_building = createBuildingMatrix(map_size);

    strcpy(game_data->hostname, "localhost");
    strcpy(game_data->pseudo, "");

    return game_data;
}

extern int deleteGameData(client_game_data_t **game_data, int map_size)
{
    if (game_data == NULL || *game_data == NULL)
        return -1;

    if ((*game_data)->opponent_pseudo != NULL)
        free((*game_data)->opponent_pseudo);

    deleteTimer(&(*game_data)->timer);
    destroyBuildingMatrix(&(*game_data)->map_building, map_size);
    destroyBuildingMatrix(&(*game_data)->opponent_map_building, map_size);

    free(*game_data);
    *game_data = NULL;

    return 0;
}