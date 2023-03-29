/**
 * @file game_state.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de client/game_state.c
 * @version 0.1
 * @date 19/03/2023
 *
 */

#ifndef __CLIENT_GAME_STATE_H
#define __CLIENT_GAME_STATE_H

typedef struct client_game_data_s client_game_data_t;

#include "socket/client.h"
#include "game_data.h"

/**
 * @brief Ajoute, ou enlève le client à la file d'attente des parties
 *
 * @param client pointeur sur le client socket
 * @param game_data un pointeur sur les données du jeux
 * @param map_size taille de la carte
 */
extern void toggleMatchmaking(client_t *client, client_game_data_t *game_data, int map_size);

/**
 * @brief Lance la partie / phase de combat
 *
 * @param client pointeur sur le client socket
 * @param game_data pointeur sur les données du jeux
 */
extern void startGame(client_t *client, client_game_data_t *game_data);

/**
 * @brief Vérifie si la partie dépasse de le temps à partie, et envoie un paquet de fin de partie
 *
 * @param client pointeur sur le client socket
 * @param game_data pointeur sur les données du jeux
 */
extern void
checkClientGameTimeout(client_t *client, client_game_data_t *game_data);

/**
 * @brief Envoie le paquet de fin de partie
 *
 * @param client pointeur sur le client socket
 * @param game_data pointeur sur les données du jeux
 */
extern void endGame(client_t *client, client_game_data_t *game_data);

#endif