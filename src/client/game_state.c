/**
 * @file game_state.c
 * @author Lucas Dureau
 * @brief Définition des différents états  du jeux
 * @version 0.1
 * @date 19/03/2023
 *
 */

#include <stdlib.h>
#include "packet/packet.h"
#include "game_state_enum.h"
#include "game_state.h"

extern void toggleMatchmaking(client_t *client, client_game_data_t *game_data)
{
    packet_t *packet = NULL;

    switch (game_data->state)
    {
    case PREPARATION_GAME_STATE:
        game_data->state = MATCHMAKING_GAME_STATE;

        packet = createSetMapPacket();

        sendToServer(client, packet);
        deletePacket(&packet);

        packet = createIsPlayerReadyPacket(1);

        sendToServer(client, packet);
        deletePacket(&packet);
        break;
    case MATCHMAKING_GAME_STATE:
        game_data->state = PREPARATION_GAME_STATE;

        packet = createIsPlayerReadyPacket(0);

        sendToServer(client, packet);
        deletePacket(&packet);
        break;
    default:
        break;
    }
}

extern void endGame(client_t *client, client_game_data_t *game_data)
{
    if (game_data->state == COMBAT_GAME_STATE)
    {
        game_data->state = WAITING_RESULT_GAME_STATE;

        packet_t *packet = createGameFinishedPacket(79, 248934);

        sendToServer(client, packet);
        deletePacket(&packet);
    }
}