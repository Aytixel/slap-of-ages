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

extern void toggleMatchmaking(client_t *client, client_game_data_t *game_data);

extern void endGame(client_t *client, client_game_data_t *game_data);

#endif