/**
 * @file client_data.c
 * @author Lucas Dureau
 * @brief Implémente les fonctions associées la structure de données pour les clients connectés au serveur
 * @version 0.1
 * @date 02/03/2023
 *
 */

#include <stdlib.h>
#include "client_data.h"

extern client_data_t *createClientData()
{
    client_data_t *client_data = malloc(sizeof(client_data_t));

    client_data->pseudo = NULL;
    client_data->is_player_ready = 0;
    client_data->is_in_game = 0;
    client_data->game_data = NULL;

    return client_data;
}

extern int deleteClientData(client_data_t **client_data)
{
    if (client_data == NULL || *client_data == NULL)
        return -1;

    if ((*client_data)->pseudo != NULL)
        free((*client_data)->pseudo);

    // si le joueur était en partie
    if ((*client_data)->game_data != NULL)
    {
        if (isGameStarted((*client_data)->game_data)) // si la partie à commencé on la supprime
        {
            for (int i = 0; i < (*client_data)->game_data->array->count; i++)
            {
                if ((*client_data)->game_data == (*client_data)->game_data->array->game_data[i])
                {
                    removeGameDataFromArray((*client_data)->game_data->array, i);
                    break;
                }
            }
        }
        else // sinon on supprime juste les donnéees du joueur si la partie n'a pas commencé
        {
            if ((*client_data)->game_data->player[0]->client_data == (*client_data))
                deletePlayerGameData(&(*client_data)->game_data->player[0]);
            else
                deletePlayerGameData(&(*client_data)->game_data->player[1]);
        }
    }

    free(*client_data);
    *client_data = NULL;

    return 0;
}