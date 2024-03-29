/**
 * @file game_data.c
 * @author Lucas Dureau
 * @brief Implémente les fonctions associées la structure de données contenant les données du jeux
 * @version 1.1
 * @date 02/03/2023
 *
 */

#include "game_data.h"

extern client_game_data_t *createGameData()
{
    client_game_data_t *game_data = malloc(sizeof(client_game_data_t));

    game_data->port = CLIENT_DEFAULT_PORT;
    game_data->state = CONNECTION_GAME_STATE;
    game_data->selected_building_type = HOUSE_1_BUILDING;
    game_data->selected_character_type = RAT_CHARACTER;
    game_data->map_building = createBuildingMatrix();
    game_data->character_list = createCharacterList();
    game_data->gold_count = DEFAULT_GOLD_COUNT;
    game_data->gold_cost = 0;
    game_data->elixir_count = DEFAULT_ELIXIR_COUNT;
    game_data->elixir_cost = 0;
    game_data->win_count = 0;
    game_data->timer = NULL;
    game_data->opponent_pseudo = NULL;
    game_data->initial_opponent_gold_cost = 0;
    game_data->opponent_gold_cost = 0;
    game_data->opponent_map_building = createBuildingMatrix();

    memset(game_data->hostname, 0, HOSTNAME_SIZE);
    strcpy(game_data->hostname, CLIENT_DEFAULT_HOSTNAME);
    memset(game_data->pseudo, 0, PSEUDO_SIZE);
    strcpy(game_data->pseudo, "");

    return game_data;
}

extern int deleteGameData(client_game_data_t **game_data)
{
    if (game_data == NULL || *game_data == NULL)
        return -1;

    if ((*game_data)->opponent_pseudo != NULL)
        free((*game_data)->opponent_pseudo);

    deleteTimer(&(*game_data)->timer);
    destroyBuildingMatrix(&(*game_data)->map_building);
    destroyCharacterList(&(*game_data)->character_list);
    destroyBuildingMatrix(&(*game_data)->opponent_map_building);

    free(*game_data);
    *game_data = NULL;

    return 0;
}