/**
 * @file game_state.c
 * @author Lucas Dureau
 * @brief Définition des différents états  du jeux
 * @version 1.1
 * @date 19/03/2023
 *
 */

#include "game_state.h"

extern void toggleMatchmaking(client_t *client, client_game_data_t *game_data)
{
    packet_t *packet = NULL;

    switch (game_data->state)
    {
    case PREPARATION_GAME_STATE:
        game_data->state = MATCHMAKING_GAME_STATE;

        packet = createSetMapPacket(game_data);

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

        packet_t *packet = createGameFinishedPacket(
            100 - (float)game_data->opponent_gold_cost / (float)game_data->initial_opponent_gold_cost * 100,
            timeLeft(game_data->timer));

        deleteTimer(&game_data->timer);

        sendToServer(client, packet);
        deletePacket(&packet);
    }
}