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

typedef struct building_s building_t;

#include <stdint.h>
#include "timer/timer.h"
#include "map/building.h"
#include "game_state_enum.h"

/**
 * @brief Structure de données contenant les données du jeux
 *
 */
typedef struct client_game_data_s
{
    client_game_state_e state;           /**< états du jeux*/
    char hostname[1024];                 /**< addresse du serveur*/
    uint16_t port;                       /**< port du serveur*/
    char pseudo[64];                     /**< pseudo du joueur*/
    building_t ***map_building;          /**< matrice contenant la totalité des bâtiments placés sur la carte*/
    int gold_count;                      /**< nombre d'or en la possession du joueur*/
    int gold_cost;                       /**< nombre d'or que coûte le village*/
    int win_count;                       /**< nombre de victoire*/
    frame_timer_t *timer;                /**< chronomètre de fin de partie*/
    char *opponent_pseudo;               /**< pseudo du joueur adverse*/
    int initial_opponent_gold_cost;      /**< nombre d'or que coûte le village adverse initiale*/
    int opponent_gold_cost;              /**< nombre d'or que coûte le village adverse*/
    building_t ***opponent_map_building; /**< matrice contenant la totalité des bâtiments placés sur la carte adverse*/
} client_game_data_t;

/**
 * @brief Créer les données du jeux
 *
 * @param map_size taille de la carte
 * @return un pointer sur les **données du jeux**
 */
extern client_game_data_t *createGameData(int map_size);

/**
 * @brief Détruit les données du jeux
 *
 * @param game_data une référence d'un pointeur sur les données du jeux
 * @param map_size taille de la carte
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteGameData(client_game_data_t **game_data, int map_size);

#endif
