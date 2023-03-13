/**
 * @file game_state.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de server/game_state.c
 * @version 0.1
 * @date 02/03/2023
 *
 */

#ifndef __GAME_DATA_H
#define __GAME_DATA_H

typedef struct client_data_s client_data_t;
typedef struct game_data_array_s game_data_array_t;

#include <string.h>
#include "client_data.h"
#include "connection/server.h"
#include "packet/packet.h"

/**
 * @brief Structure de données contenant les informations d'une partie pour un joueur
 *
 */
typedef struct
{
    int socket_fd;                  /**< descripteur du socket du joueur*/
    float destruction_percentage;   /**< pourcentage de destruction*/
    long time_left;                 /**< temps restant avant la fin de partie*/
    int has_finished;               /**< indique si le joueur a finie*/
    client_data_t *client_data;     /**< un pointeur sur les données client*/
    server_client_t *server_client; /**< un pointeur sur la connexion client*/
} player_game_data_t;

/**
 * @brief Structure de données contenant les informations d'une partie
 *
 */
typedef struct game_data_s
{
    player_game_data_t *player[2]; /**< joueurs*/
    game_data_array_t *array;      /**< pointeur sur la structure parente*/
} game_data_t;

/**
 * @brief Structure de données contenant un tableau des données de partie
 *
 */
typedef struct game_data_array_s
{
    game_data_t **game_data; /**< tableau des données de partie*/
    int count;               /**< nombre de partie*/
} game_data_array_t;

extern game_data_array_t *createGameDataArray();

extern void addGameDataToArray(game_data_array_t *game_data_array);

extern int findGame(game_data_array_t *game_data_array);

extern int removeGameDataFromArray(game_data_array_t *game_data_array, int index);

extern int deleteGameDataArray(game_data_array_t **game_data_array);

extern player_game_data_t *createPlayerGameData(int socket_fd, client_data_t *client_data, server_client_t *server_client);

extern int deletePlayerGameData(player_game_data_t **player_game_data);

extern game_data_t *createGameData();

extern int addPlayerToGame(game_data_t *game_data, int socket_fd, client_data_t *client_data, server_client_t *server_client);

extern int removePlayerFromGame(game_data_t *game_data, int socket_fd);

extern int isGameEmpty(game_data_t *game_data);

extern int isGameStarted(game_data_t *game_data);

extern int isGameFinished(game_data_t *game_data);

extern int gameHasPlayer(game_data_t *game_data, int socket_fd);

extern int setPlayerFinished(game_data_t *game_data, int socket_fd, packet_t *packet);

extern int deleteGameData(game_data_t **game_data);

#endif
