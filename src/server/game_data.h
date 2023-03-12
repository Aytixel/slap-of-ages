/**
 * @file game_data.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de server/game_data.c
 * @version 0.1
 * @date 02/03/2023
 *
 */

#ifndef __GAME_DATA_H
#define __GAME_DATA_H

#include "packet/packet.h"

/**
 * @brief Structure de données contetant les informations d'une partie pour un joueur
 *
 */
typedef struct
{
    int socket_fd;                /**< descripteur du socket du joueur*/
    float destruction_percentage; /**< pourcentage de destruction*/
    long time_left;               /**< temps restant avant la fin de partie*/
    int has_finished;             /**< indique si le joueur a finie*/
} player_game_data_t;

/**
 * @brief Structure de données contetant les informations d'une partie
 *
 */
typedef struct
{
    player_game_data_t *player[2]; /**< joueurs*/
} game_data_t;

extern game_data_t *createGameData();

extern int addPlayerToGame(game_data_t *game_data, int socket_fd);

extern int removePlayerFromGame(game_data_t *game_data, int socket_fd);

extern int isGameStarted(game_data_t *game_data);

extern int isGameFinished(game_data_t *game_data);

extern int gameHasPlayer(game_data_t *game_data, int socket_fd);

extern int setPlayerFinished(game_data_t *game_data, int socket_fd, packet_t *packet);

extern int deleteGameData(game_data_t **game_data);

#endif
