/**
 * @file game_state.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de client/game_state.c
 * @version 1.1
 * @date 19/03/2023
 *
 */

#ifndef __CLIENT_GAME_STATE_H
#define __CLIENT_GAME_STATE_H

typedef struct client_game_data_s client_game_data_t;

#include <stdlib.h>
#include "packet/packet.h"
#include "packet/client_packet.h"
#include "timer/timer.h"
#include "game_state_enum.h"
#include "socket/client.h"
#include "game_data.h"

/**
 * @brief Ajoute, ou enlève le client à la file d'attente des parties
 *
 * @param client pointeur sur le client socket
 * @param game_data un pointeur sur les données du jeu
 */
extern void toggleMatchmaking(client_t *client, client_game_data_t *game_data);

/**
 * @brief Lance la partie / phase de combat
 *
 * @param client pointeur sur le client socket
 * @param game_data pointeur sur les données du jeu
 */
extern void startGame(client_t *client, client_game_data_t *game_data);

/**
 * @brief Vérifie si la partie dépasse de le temps à partie, et envoie un paquet de fin de partie
 *
 * @param client pointeur sur le client socket
 * @param game_data pointeur sur les données du jeu
 */
extern void
checkClientGameTimeout(client_t *client, client_game_data_t *game_data);

/**
 * @brief Envoie le paquet de fin de partie
 *
 * @param client pointeur sur le client socket
 * @param game_data pointeur sur les données du jeu
 */
extern void endGame(client_t *client, client_game_data_t *game_data);

#endif