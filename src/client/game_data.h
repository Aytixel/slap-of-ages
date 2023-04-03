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
#include "common.h"
#include "timer/timer.h"
#include "map/building.h"
#include "game/character.h"
#include "game_state_enum.h"

/**
 * @brief Structure de données contenant les données du jeu
 *
 */
typedef struct client_game_data_s
{
    client_game_state_e state;              /**< états du jeu*/
    char hostname[HOSTNAME_SIZE];           /**< addresse du serveur*/
    uint16_t port;                          /**< port du serveur*/
    char pseudo[PSEUDO_SIZE];               /**< pseudo du joueur*/
    building_t ***map_building;             /**< matrice contenant la totalité des bâtiments placés sur la carte*/
    character_t ***character_matrix;        /**< matrice contenant la totalité des troupes*/
    building_type_e selected_building_type; /**< type de bâtiment sélectionner*/
    int gold_count;                         /**< nombre d'or en la possession du joueur*/
    int gold_cost;                          /**< nombre d'or que coûte le village*/
    int win_count;                          /**< nombre de victoire*/
    frame_timer_t *timer;                   /**< chronomètre de fin de partie*/
    char *opponent_pseudo;                  /**< pseudo du joueur adverse*/
    int initial_opponent_gold_cost;         /**< nombre d'or que coûte le village adverse initiale*/
    int opponent_gold_cost;                 /**< nombre d'or que coûte le village adverse*/
    building_t ***opponent_map_building;    /**< matrice contenant la totalité des bâtiments placés sur la carte adverse*/
} client_game_data_t;

/**
 * @brief Créer les données du jeu
 *
 * @return un pointer sur les **données du jeu**
 */
extern client_game_data_t *createGameData();

/**
 * @brief Détruit les données du jeu
 *
 * @param game_data une référence d'un pointeur sur les données du jeu
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteGameData(client_game_data_t **game_data);

#endif
