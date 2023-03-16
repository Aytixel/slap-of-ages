/**
 * @file game_state.c
 * @author Lucas Dureau
 * @brief Implémente les fonctions permettant de gérer l'état d'une partie
 * @version 0.1
 * @date 12/03/2023
 *
 */

#include <stdlib.h>
#include <stdio.h>
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
    printf("%d : %s à finie %% de destruction : %d, temps restant en ms avant la fin normale de partie : %ld\n", server_client->socket_fd, game_data->player[player]->client_data->pseudo, game_data->player[player]->destruction_percentage, game_data->player[player]->time_left);

    return 0;
}

extern int gameWinner(game_data_t *game_data)
{
    if (!isGameFinished(game_data))
        return -1;

    // le joueur avec le plus de desctruction gagne
    if (game_data->player[0]->destruction_percentage != game_data->player[1]->destruction_percentage)
        return game_data->player[0]->destruction_percentage < game_data->player[1]->destruction_percentage;

    // le joueur avec qui fini le plus vite gagne
    if (game_data->player[0]->time_left != game_data->player[1]->time_left)
        return game_data->player[0]->time_left > game_data->player[1]->time_left;

    // égalité
    return 2;
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

extern int setPlayerFinishedInArray(game_data_array_t *game_data_array, int socket_fd, packet_t *packet)
{
    for (int i = 0; i < game_data_array->count; i++)
    {
        if (!setPlayerFinished(game_data_array->game_data[i], socket_fd, packet))
        {
            if (isGameFinished(game_data_array->game_data[i]))
            {
                game_data_array->game_data[i]->player[0]->client_data->is_in_game = 0;
                game_data_array->game_data[i]->player[1]->client_data->is_in_game = 0;
                game_data_array->game_data[i]->player[0]->client_data->is_player_ready = 0;
                game_data_array->game_data[i]->player[1]->client_data->is_player_ready = 0;

                return i;
            }

            return -1;
        }
    }

    return -1;
}

extern void setPlayerIsReadyInArray(game_data_array_t *game_data_array, client_data_t *client_data, packet_t *packet)
{
    int is_player_ready;

    readIsPlayerReadyPacket(packet, &is_player_ready);

    // si le joueur est en partie impossible d'en rejoindre une
    if (client_data->is_in_game)
        return;

    if (client_data->is_player_ready != is_player_ready)
        client_data->is_player_ready = is_player_ready;

    if (client_data->is_player_ready)
    {

        int game_index = findGame(game_data_array);

        if (game_index == -1)
        {
            addGameDataToArray(game_data_array);
            game_index = game_data_array->count - 1;
        }

        // empêche un joueur de se connecter 2 fois dans la même partie, ou d'être dans 2 partie
        if (gameHasPlayer(game_data_array->game_data[game_index], server_client->socket_fd) != -1)
            return;

        addPlayerToGame(game_data_array->game_data[game_index], server_client->socket_fd, client_data, server_client);

        printf("%d : %s est prêt à jouer\n", server_client->socket_fd, client_data->pseudo);

        if (isGameStarted(game_data_array->game_data[game_index]))
        {
            game_data_array->game_data[game_index]->player[0]->client_data->is_in_game = 1;
            game_data_array->game_data[game_index]->player[1]->client_data->is_in_game = 1;

            packet_t *packet = createSetPseudoPacket(game_data_array->game_data[game_index]->player[1]->client_data->pseudo);

            sendToServerClient(game_data_array->game_data[game_index]->player[0]->server_client, packet);
            deletePacket(&packet);

            packet = createSetPseudoPacket(game_data_array->game_data[game_index]->player[0]->client_data->pseudo);

            sendToServerClient(game_data_array->game_data[game_index]->player[1]->server_client, packet);
            deletePacket(&packet);

            packet = createSetMapPacket();

            sendToServerClient(game_data_array->game_data[game_index]->player[0]->server_client, packet);
            deletePacket(&packet);

            packet = createSetMapPacket();

            sendToServerClient(game_data_array->game_data[game_index]->player[1]->server_client, packet);
            deletePacket(&packet);

            printf("%d : Parti lancé entre %s, et %s\n", server_client->socket_fd, game_data_array->game_data[game_index]->player[0]->client_data->pseudo, game_data_array->game_data[game_index]->player[1]->client_data->pseudo);
        }
    }
    else
    {
        for (int i = 0; i < game_data_array->count; i++)
        {
            if (removePlayerFromGame(game_data_array->game_data[i], server_client->socket_fd))
            {
                if (isGameEmpty(game_data_array->game_data[i]))
                    removeGameDataFromArray(game_data_array, i);

                printf("%d : %s n'est pas prêt à jouer\n", server_client->socket_fd, client_data->pseudo);
                break;
            }
        }
    }
}