/**
 * @file game_state.c
 * @author Lucas Dureau
 * @brief Implémente les fonctions permettant de gérer l'état d'une partie
 * @version 1.1
 * @date 12/03/2023
 *
 */

#include "game_state.h"

extern server_game_state_array_t *createGameStateArray()
{
    server_game_state_array_t *game_state_array = malloc(sizeof(server_game_state_array_t));

    game_state_array->capacity = 1;
    game_state_array->game_state = malloc(sizeof(void *) * game_state_array->capacity);
    game_state_array->count = 0;

    return game_state_array;
}

extern void addGameStateToArray(server_game_state_array_t *game_state_array)
{
    if (game_state_array->capacity == game_state_array->count)
    {
        game_state_array->capacity *= 2;
        game_state_array->game_state = realloc(game_state_array->game_state, sizeof(void *) * game_state_array->capacity);
    }

    game_state_array->game_state[game_state_array->count] = createGameState();
    game_state_array->game_state[game_state_array->count]->array = game_state_array;
    game_state_array->count++;
}

extern int findGame(server_game_state_array_t *game_state_array)
{
    if (game_state_array->count == 0)
        return -1;

    for (int i = 0; i < game_state_array->count; i++)
    {
        if (!isGameStarted(game_state_array->game_state[i]))
            return i;
    }

    return -1;
}

extern int removeGameStateFromArray(server_game_state_array_t *game_state_array, int index)
{
    if (index < 0 || index >= game_state_array->count)
        return 0;

    deleteGameState(game_state_array->game_state + index);

    game_state_array->game_state[index] = game_state_array->game_state[--game_state_array->count];

    return 1;
}

extern int deleteGameStateArray(server_game_state_array_t **game_state_array)
{
    if (game_state_array == NULL || *game_state_array == NULL)
        return -1;

    for (int i = 0; i < (*game_state_array)->count; i++)
    {
        deleteGameState((*game_state_array)->game_state + i);
    }

    free((*game_state_array)->game_state);
    free(*game_state_array);
    *game_state_array = NULL;

    return 0;
}

extern player_game_state_t *createPlayerState(int socket_fd, server_client_data_t *client_data, server_client_t *server_client)
{
    player_game_state_t *player_game_state = malloc(sizeof(player_game_state_t));

    player_game_state->socket_fd = socket_fd;
    player_game_state->destruction_percentage = -1;
    player_game_state->time_left = -1;
    player_game_state->has_finished = 0;
    player_game_state->client_data = client_data;
    player_game_state->server_client = server_client;

    return player_game_state;
}

extern int deletePlayerState(player_game_state_t **player_game_state)
{
    if (player_game_state == NULL || *player_game_state == NULL)
        return -1;

    if ((*player_game_state)->client_data != NULL)
    {
        (*player_game_state)->client_data->is_in_game = 0;
        (*player_game_state)->client_data->game_state = NULL;
    }

    free(*player_game_state);
    *player_game_state = NULL;

    return 0;
}

extern server_game_state_t *createGameState()
{
    server_game_state_t *game_state = malloc(sizeof(server_game_state_t));

    game_state->player[0] = NULL;
    game_state->player[1] = NULL;
    game_state->array = NULL;
    game_state->timer = NULL;

    return game_state;
}

extern int addPlayerToGame(server_game_state_t *game_state, int socket_fd, server_client_data_t *client_data, server_client_t *server_client)
{
    if (isGameStarted(game_state))
        return 0;

    game_state->player[game_state->player[0] != NULL] = createPlayerState(socket_fd, client_data, server_client);

    if (client_data != NULL)
        client_data->game_state = game_state;

    return 1;
}

extern int removePlayerFromGame(server_game_state_t *game_state, int socket_fd)
{
    int player = gameHasPlayer(game_state, socket_fd);

    if (player == -1)
        return 0;

    deletePlayerState(&game_state->player[player]);

    return 1;
}

extern int isGameEmpty(server_game_state_t *game_state)
{
    return game_state->player[0] == NULL && game_state->player[1] == NULL;
}

extern int isGameStarted(server_game_state_t *game_state)
{
    return game_state->player[0] != NULL && game_state->player[1] != NULL;
}

extern int isGameFinished(server_game_state_t *game_state)
{
    return isGameStarted(game_state) && game_state->player[0]->has_finished && game_state->player[1]->has_finished;
}

extern int gameHasPlayer(server_game_state_t *game_state, int socket_fd)
{
    if (game_state->player[0] != NULL && game_state->player[0]->socket_fd == socket_fd)
        return 0;
    if (game_state->player[1] != NULL && game_state->player[1]->socket_fd == socket_fd)
        return 1;

    return -1;
}

extern int setPlayerFinished(server_game_state_t *game_state, int socket_fd, packet_t *packet)
{
    int player = gameHasPlayer(game_state, socket_fd);

    if (player == -1)
        return -1;

    game_state->player[player]->has_finished = 1;
    readGameFinishedPacket(packet, &game_state->player[player]->destruction_percentage, &game_state->player[player]->time_left);

    if (game_state->player[player]->client_data != NULL)
        printf("%d : %s à finie, pourcentage de destruction : %d, temps restant en ms avant la fin normale de partie : %ld\n", game_state->player[player]->server_client->socket_fd, game_state->player[player]->client_data->pseudo, game_state->player[player]->destruction_percentage, game_state->player[player]->time_left);

    return 0;
}

extern int gameWinner(server_game_state_t *game_state)
{
    if (!isGameFinished(game_state))
        return -1;

    // le joueur avec le plus de desctruction gagne
    if (game_state->player[0]->destruction_percentage != game_state->player[1]->destruction_percentage)
        return game_state->player[0]->destruction_percentage < game_state->player[1]->destruction_percentage;

    // les deux joeurs n'ont pas puent finir la partie
    if (game_state->player[0]->time_left <= 0 && game_state->player[1]->time_left <= 0)
        return 2;

    // le joueur avec qui fini le plus vite gagne
    if (game_state->player[0]->time_left != game_state->player[1]->time_left)
        return game_state->player[0]->time_left < game_state->player[1]->time_left;

    // égalité
    return 2;
}

extern int deleteGameState(server_game_state_t **game_state)
{
    if (game_state == NULL || *game_state == NULL)
        return -1;

    deletePlayerState(&(*game_state)->player[0]);
    deletePlayerState(&(*game_state)->player[1]);
    deleteTimer(&(*game_state)->timer);

    free(*game_state);
    *game_state = NULL;

    return 0;
}

extern void setPlayerFinishedInArray(server_game_state_array_t *game_state_array, int socket_fd, packet_t *packet)
{
    for (int i = 0; i < game_state_array->count; i++)
    {
        if (!setPlayerFinished(game_state_array->game_state[i], socket_fd, packet))
        {
            if (isGameFinished(game_state_array->game_state[i]))
            {
                packet_t *packet_1 = NULL;
                packet_t *packet_2 = NULL;

                switch (gameWinner(game_state_array->game_state[i]))
                {
                case 0:
                    packet_1 = createHasPlayerWonPacket(1);
                    packet_2 = createHasPlayerWonPacket(0);
                    break;
                case 1:
                    packet_1 = createHasPlayerWonPacket(0);
                    packet_2 = createHasPlayerWonPacket(1);
                    break;
                case 2:
                default:
                    packet_1 = createHasPlayerWonPacket(2);
                    packet_2 = createHasPlayerWonPacket(2);
                    break;
                }

                sendToServerClient(game_state_array->game_state[i]->player[0]->server_client, packet_1);
                sendToServerClient(game_state_array->game_state[i]->player[1]->server_client, packet_2);

                deletePacket(&packet_1);
                deletePacket(&packet_2);

                printf("%d : Partie finie entre %s, et %s\n", socket_fd, game_state_array->game_state[i]->player[0]->client_data->pseudo, game_state_array->game_state[i]->player[1]->client_data->pseudo);

                removeGameStateFromArray(game_state_array, i);
            }
        }
    }
}

extern void checkServerGameTimeout(server_game_state_array_t *game_state_array)
{
    for (int i = 0; i < game_state_array->count; i++)
    {
        if (isGameStarted(game_state_array->game_state[i]) && timeLeft(game_state_array->game_state[i]->timer) <= 0)
        {
            packet_t *packet_1 = NULL;
            packet_t *packet_2 = NULL;

            if (game_state_array->game_state[i]->player[0]->has_finished && !game_state_array->game_state[i]->player[1]->has_finished)
            {
                packet_1 = createHasPlayerWonPacket(1);
                packet_2 = createHasPlayerWonPacket(0);
            }
            else if (!game_state_array->game_state[i]->player[0]->has_finished && game_state_array->game_state[i]->player[1]->has_finished)
            {
                packet_1 = createHasPlayerWonPacket(0);
                packet_2 = createHasPlayerWonPacket(1);
            }
            else
            {
                packet_1 = createHasPlayerWonPacket(2);
                packet_2 = createHasPlayerWonPacket(2);
            }

            sendToServerClient(game_state_array->game_state[i]->player[0]->server_client, packet_1);
            sendToServerClient(game_state_array->game_state[i]->player[1]->server_client, packet_2);

            deletePacket(&packet_1);
            deletePacket(&packet_2);

            printf("  : Partie finie entre %s, et %s\n", game_state_array->game_state[i]->player[0]->client_data->pseudo, game_state_array->game_state[i]->player[1]->client_data->pseudo);

            removeGameStateFromArray(game_state_array, i--);
        }
    }
}

extern void setPlayerIsReadyInArray(server_game_state_array_t *game_state_array, server_client_data_t *client_data, server_client_t *server_client, packet_t *packet)
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

        int game_index = findGame(game_state_array);

        if (game_index == -1)
        {
            addGameStateToArray(game_state_array);
            game_index = game_state_array->count - 1;
        }

        // empêche un joueur de se connecter 2 fois dans la même partie, ou d'être dans 2 partie
        if (gameHasPlayer(game_state_array->game_state[game_index], server_client->socket_fd) != -1)
            return;

        addPlayerToGame(game_state_array->game_state[game_index], server_client->socket_fd, client_data, server_client);

        printf("%d : %s est prêt à jouer\n", server_client->socket_fd, client_data->pseudo);

        if (isGameStarted(game_state_array->game_state[game_index]))
        {
            game_state_array->game_state[game_index]->player[0]->client_data->is_in_game = 1;
            game_state_array->game_state[game_index]->player[1]->client_data->is_in_game = 1;
            game_state_array->game_state[game_index]->timer = createTimer(1000. * 60. * 3.6); // initialisation du chronomètre, avec un délaie supplémentaire pour les temps de latence

            packet_t *packet = createSetPseudoPacket(game_state_array->game_state[game_index]->player[1]->client_data->pseudo);

            sendToServerClient(game_state_array->game_state[game_index]->player[0]->server_client, packet);
            deletePacket(&packet);

            packet = createSetPseudoPacket(game_state_array->game_state[game_index]->player[0]->client_data->pseudo);

            sendToServerClient(game_state_array->game_state[game_index]->player[1]->server_client, packet);
            deletePacket(&packet);

            sendToServerClient(game_state_array->game_state[game_index]->player[0]->server_client, game_state_array->game_state[game_index]->player[1]->client_data->map_packet);
            sendToServerClient(game_state_array->game_state[game_index]->player[1]->server_client, game_state_array->game_state[game_index]->player[0]->client_data->map_packet);

            printf("%d : Partie lancé entre %s, et %s\n", server_client->socket_fd, game_state_array->game_state[game_index]->player[0]->client_data->pseudo, game_state_array->game_state[game_index]->player[1]->client_data->pseudo);
        }
    }
    else
    {
        for (int i = 0; i < game_state_array->count; i++)
        {
            if (removePlayerFromGame(game_state_array->game_state[i], server_client->socket_fd))
            {
                if (isGameEmpty(game_state_array->game_state[i]))
                    removeGameStateFromArray(game_state_array, i);

                printf("%d : %s n'est pas prêt à jouer\n", server_client->socket_fd, client_data->pseudo);
                break;
            }
        }
    }
}