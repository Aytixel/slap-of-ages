/**
 * @file game_data.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de client/game_data.c
 * @version 0.1
 * @date 17/03/2023
 *
 */

#ifndef __CLIENT_GAME_DATA_H
#define __CLIENT_GAME_DATA_H

#include <stdint.h>
#include "game_state_enum.h"

/**
 * @brief
 *
 */
typedef struct client_game_data_s
{
    client_game_state_e state; /**< états du jeux*/
    char hostname[1024];       /**< addresse du serveur*/
    uint16_t port;             /**< port du serveur*/
    char pseudo[64];           /**< pseudo du joueur*/
    char *opponent_pseudo;     /**< pseudo du joueur adverse*/
    int gold_count;            /**< nombre d'or maximum*/
    int victory_count;         /**< nombre de victoire*/
} client_game_data_t;

/**
 * @brief Créer les données du jeux
 *
 * @return un pointer sur les **données du jeux**
 */
extern client_game_data_t *createGameData();

/**
 * @brief Détruit les données du jeux
 *
 * @param game_data une référence d'un pointeur sur les données du jeux
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteGameData(client_game_data_t **game_data);

#endif
