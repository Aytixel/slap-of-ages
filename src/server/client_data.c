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

extern server_client_data_t *createServerClientData()
{
    server_client_data_t *client_data = malloc(sizeof(server_client_data_t));

    client_data->pseudo = NULL;
    client_data->is_player_ready = 0;
    client_data->is_in_game = 0;
    client_data->game_state = NULL;
    client_data->map_packet = NULL;

    return client_data;
}

extern int deleteServerClientData(server_client_data_t **client_data)
{
    if (client_data == NULL || *client_data == NULL)
        return -1;

    if ((*client_data)->pseudo != NULL)
        free((*client_data)->pseudo);

    // si le joueur était en partie
    if ((*client_data)->game_state != NULL)
    {
        if (isGameStarted((*client_data)->game_state)) // si la partie à commencé on la supprime
        {
            for (int i = 0; i < (*client_data)->game_state->array->count; i++)
            {
                if ((*client_data)->game_state == (*client_data)->game_state->array->game_state[i])
                {
                    // on envoie un paquet à l'autre joueur pour dire qu'il a gagné par défaut
                    packet_t *packet = createHasPlayerWonPacket(1);

                    if ((*client_data)->game_state->player[0]->client_data == (*client_data))
                        sendToServerClient((*client_data)->game_state->player[1]->server_client, packet);
                    else
                        sendToServerClient((*client_data)->game_state->player[0]->server_client, packet);

                    deletePacket(&packet);
                    removeGameStateFromArray((*client_data)->game_state->array, i);
                    break;
                }
            }
        }
        else // sinon on supprime juste les donnéees du joueur si la partie n'a pas commencé
        {
            if ((*client_data)->game_state->player[0]->client_data == (*client_data))
                deletePlayerState(&(*client_data)->game_state->player[0]);
            else
                deletePlayerState(&(*client_data)->game_state->player[1]);
        }
    }

    deletePacket(&(*client_data)->map_packet);

    free(*client_data);
    *client_data = NULL;

    return 0;
}