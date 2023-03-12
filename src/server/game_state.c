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

/**
 * @brief Créer un tableau avec les données de partie
 *
 * @return un pointer sur le **tableau des données de partie**
 */
extern game_data_array_t *createGameDataArray()
{
    game_data_array_t *game_data_array = malloc(sizeof(game_data_array_t));

    game_data_array->game_data = NULL;
    game_data_array->count = 0;

    return game_data_array;
}

/**
 * @brief Ajoute une nouvelle partie au tableau
 *
 * @param game_data_array une référence d'un pointeur sur un tableau avec les données de partie
 */
extern void addGameDataToArray(game_data_array_t *game_data_array)
{
    game_data_array->game_data = realloc(game_data_array->game_data, sizeof(void *) * ++game_data_array->count);
    game_data_array->game_data[game_data_array->count - 1] = createGameData();
}

/**
 * @brief Trouve une partie qui n'est pas commencé
 *
 * @param game_data_array une référence d'un pointeur sur un tableau avec les données de partie
 * @return l'**index** de la partie trouvé, **-1** sinon
 */
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

/**
 * @brief Enlève les données d'une partie du tableau
 *
 * @param game_data_array une référence d'un pointeur sur un tableau avec les données de partie
 * @param index dans le tableau
 * @return **1** si la partie a pu être enlevé, **0** sinon
 */
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

/**
 * @brief Détruit un tableau avec les données de partie
 *
 * @param game_data_array une référence d'un pointeur sur un tableau avec les données de partie
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
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

/**
 * @brief Créer les données d'un joueur pour la partie
 *
 * @param socket_fd id du socket client
 * @return un pointer sur les **données d'un joueur pour la partie**
 */
static player_game_data_t *createPlayerGameData(int socket_fd)
{
    player_game_data_t *player_game_data = malloc(sizeof(player_game_data_t));

    player_game_data->socket_fd = socket_fd;
    player_game_data->destruction_percentage = -1;
    player_game_data->time_left = -1;
    player_game_data->has_finished = 0;

    return player_game_data;
}

/**
 * @brief Détruit les données d'un joueur pour la partie
 *
 * @param player_game_data une référence d'un pointeur sur les données d'un joueur pour la partie
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
static int deletePlayerGameData(player_game_data_t **player_game_data)
{
    if (player_game_data == NULL || *player_game_data == NULL)
        return -1;

    free(*player_game_data);
    *player_game_data = NULL;

    return 0;
}

/**
 * @brief Créer les données d'un partie
 *
 * @return un pointer sur les **données d'une partie**
 */
extern game_data_t *createGameData()
{
    game_data_t *game_data = malloc(sizeof(game_data_t));

    game_data->player[0] = NULL;
    game_data->player[1] = NULL;

    return game_data;
}

/**
 * @brief Ajoute un joueur à la partie
 *
 * @param game_data un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @return **1** si le joueur a pu être ajouté, **0** sinon
 */
extern int addPlayerToGame(game_data_t *game_data, int socket_fd)
{
    if (isGameStarted(game_data))
        return 0;

    game_data->player[game_data->player[0] != NULL] = createPlayerGameData(socket_fd);

    return 1;
}

/**
 * @brief Enlève un joueur à la partie
 *
 * @param game_data un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @return **1** si le joueur a pu être enlevé, **0** sinon
 */
extern int removePlayerFromGame(game_data_t *game_data, int socket_fd)
{
    int player = gameHasPlayer(game_data, socket_fd);

    if (player == -1)
        return 0;

    deletePlayerGameData(&game_data->player[player]);

    return 1;
}

/**
 * @brief Récupère si la partie est commencé
 *
 * @param game_data un pointeur sur les données d'une partie
 * @return **1** si la partie est commencé, **0** sinon
 */
extern int isGameStarted(game_data_t *game_data)
{
    return game_data != NULL && game_data->player[0] != NULL && game_data->player[1] != NULL;
}

/**
 * @brief Récupère si la partie est finie
 *
 * @param game_data un pointeur sur les données d'une partie
 * @return **1** si la partie est finie, **0** sinon
 */
extern int isGameFinished(game_data_t *game_data)
{
    return isGameStarted(game_data) && game_data->player[0]->has_finished && game_data->player[1]->has_finished;
}

/**
 * @brief Récupère si ce joueur est dans cette partie
 *
 * @param game_data un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @return **0** si ce joueur est le joueur 1, **1** si il est le 2ème, **-1** si il n'en fait pas partie
 */
extern int gameHasPlayer(game_data_t *game_data, int socket_fd)
{
    if (game_data->player[0] != NULL && game_data->player[0]->socket_fd == socket_fd)
        return 0;
    if (game_data->player[1] != NULL && game_data->player[1]->socket_fd == socket_fd)
        return 1;

    return -1;
}

/**
 * @brief Définit un joueur comme ayant fini
 *
 * @param game_data un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @param packet un pointeur le paquet contenant les informations de fin de partie
 * @return **0** si le joueur est dans cette partie, **-1** sinon
 */
extern int setPlayerFinished(game_data_t *game_data, int socket_fd, packet_t *packet)
{
    int player = gameHasPlayer(game_data, socket_fd);

    if (player == -1)
        return -1;

    game_data->player[player]->has_finished = 1;
    readGameFinishedPacket(packet, &game_data->player[player]->destruction_percentage, &game_data->player[player]->time_left);

    return 0;
}

/**
 * @brief Détruit les données d'une partie
 *
 * @param game_data une référence d'un pointeur sur les données d'une partie
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
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