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
#include "timer/timer.h"
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

extern void startGame(client_t *client, client_game_data_t *game_data)
{
    game_data->state = COMBAT_GAME_STATE;
    game_data->timer = createTimer(1000. * 60. * 3.5);
}

extern void checkClientGameTimeout(client_t *client, client_game_data_t *game_data)
{
    if (game_data->state == COMBAT_GAME_STATE && timeLeft(game_data->timer) <= 0)
        endGame(client, game_data);
}

extern void endGame(client_t *client, client_game_data_t *game_data)
{
    if (game_data->state == COMBAT_GAME_STATE)
    {
        game_data->state = WAITING_RESULT_GAME_STATE;

        packet_t *packet = createGameFinishedPacket(79, timeLeft(game_data->timer));

        deleteTimer(&game_data->timer);

        sendToServer(client, packet);
        deletePacket(&packet);
    }
}