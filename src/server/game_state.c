/**
 * @file game_state.c
 * @author Lucas Dureau
 * @brief Implémente les fonctions permettant de gérer l'état d'une partie
 * @version 0.1
 * @date 12/03/2023
 *
 */

#include <stdlib.h>
#include "game_state.h"

extern game_data_array_t *createGameDataArray()
{
    game_data_array_t *game_data_array = malloc(sizeof(game_data_array_t));

    game_data_array->game_data = NULL;
    game_data_array->count = 0;

    return game_data_array;
}

extern void addGameDataToArray(game_data_array_t *game_data_array)
{
    game_data_array->game_data = realloc(game_data_array->game_data, sizeof(void *) * ++game_data_array->count);
    game_data_array->game_data[game_data_array->count - 1] = createGameData();
    game_data_array->game_data[game_data_array->count - 1]->array = game_data_array;
}

extern int findGame(game_data_array_t *game_data_array)
{
    if (game_data_array->count == 0)
        return -1;

    for (int i = 0; i < game_data_array->count; i++)
    {
        if (!isGameStarted(game_data_array->game_data[i]))
            return i;
    }

    return -1;
}

extern int removeGameDataFromArray(game_data_array_t *game_data_array, int index)
{
    if (index < 0 || index >= game_data_array->count)
        return 0;

    deleteGameData(game_data_array->game_data + index);

    game_data_array->count--;

    if (game_data_array->count > 0)
    {
        memmove(game_data_array->game_data + index, game_data_array->game_data + index + 1, sizeof(void *) * (game_data_array->count - index));

        game_data_array->game_data = realloc(game_data_array->game_data, sizeof(void *) * game_data_array->count);
    }
    else
    {
        free(game_data_array->game_data);
        game_data_array->game_data = NULL;
    }

    return 1;
}

extern int deleteGameDataArray(game_data_array_t **game_data_array)
{
    if (game_data_array == NULL || *game_data_array == NULL)
        return -1;

    for (int i = 0; i < (*game_data_array)->count; i++)
    {
        deleteGameData((*game_data_array)->game_data + i);
    }

    free((*game_data_array)->game_data);
    free(*game_data_array);
    *game_data_array = NULL;

    return 0;
}

extern player_game_data_t *createPlayerGameData(int socket_fd, client_data_t *client_data, server_client_t *server_client)
{
    player_game_data_t *player_game_data = malloc(sizeof(player_game_data_t));

    player_game_data->socket_fd = socket_fd;
    player_game_data->destruction_percentage = -1;
    player_game_data->time_left = -1;
    player_game_data->has_finished = 0;
    player_game_data->client_data = client_data;
    player_game_data->server_client = server_client;

    return player_game_data;
}

extern int deletePlayerGameData(player_game_data_t **player_game_data)
{
    if (player_game_data == NULL || *player_game_data == NULL)
        return -1;

    if ((*player_game_data)->client_data != NULL)
    {
        (*player_game_data)->client_data->is_in_game = 0;
        (*player_game_data)->client_data->game_data = NULL;
    }

    free(*player_game_data);
    *player_game_data = NULL;

    return 0;
}

extern game_data_t *createGameData()
{
    game_data_t *game_data = malloc(sizeof(game_data_t));

    game_data->player[0] = NULL;
    game_data->player[1] = NULL;
    game_data->array = NULL;

    return game_data;
}

extern int addPlayerToGame(game_data_t *game_data, int socket_fd, client_data_t *client_data, server_client_t *server_client)
{
    if (isGameStarted(game_data))
        return 0;

    game_data->player[game_data->player[0] != NULL] = createPlayerGameData(socket_fd, client_data, server_client);

    if (client_data != NULL)
        client_data->game_data = game_data;

    return 1;
}

extern int removePlayerFromGame(game_data_t *game_data, int socket_fd)
{
    int player = gameHasPlayer(game_data, socket_fd);

    if (player == -1)
        return 0;

    deletePlayerGameData(&game_data->player[player]);

    return 1;
}

extern int isGameEmpty(game_data_t *game_data)
{
    return game_data->player[0] == NULL && game_data->player[1] == NULL;
}

extern int isGameStarted(game_data_t *game_data)
{
    return game_data->player[0] != NULL && game_data->player[1] != NULL;
}

extern int isGameFinished(game_data_t *game_data)
{
    return isGameStarted(game_data) && game_data->player[0]->has_finished && game_data->player[1]->has_finished;
}

extern int gameHasPlayer(game_data_t *game_data, int socket_fd)
{
    if (game_data->player[0] != NULL && game_data->player[0]->socket_fd == socket_fd)
        return 0;
    if (game_data->player[1] != NULL && game_data->player[1]->socket_fd == socket_fd)
        return 1;

    return -1;
}

extern int setPlayerFinished(game_data_t *game_data, int socket_fd, packet_t *packet)
{
    int player = gameHasPlayer(game_data, socket_fd);

    if (player == -1)
        return -1;

    game_data->player[player]->has_finished = 1;
    readGameFinishedPacket(packet, &game_data->player[player]->destruction_percentage, &game_data->player[player]->time_left);

    return 0;
}

extern int deleteGameData(game_data_t **game_data)
{
    if (game_data == NULL || *game_data == NULL)
        return -1;

    deletePlayerGameData(&(*game_data)->player[0]);
    deletePlayerGameData(&(*game_data)->player[1]);

    free(*game_data);
    *game_data = NULL;

    return 0;
}