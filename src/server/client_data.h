/**
 * @file client_data.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de server/client_data.c
 * @version 0.1
 * @date 02/03/2023
 *
 */

#ifndef __CLIENT_DATA_H
#define __CLIENT_DATA_H

typedef struct game_data_s game_data_t;

#include "game_state.h"

/**
 * @brief Structure de données associée à un client du serveur
 *
 */
typedef struct client_data_s
{
    char *pseudo;           /**< pseudo du joueur*/
    int is_player_ready;    /**< le joueur est-il prêt ?*/
    int is_in_game;         /**< le joueur est-il en jeu ?*/
    game_data_t *game_data; /**< un pointeur sur les données de partie*/
} client_data_t;

/**
 * @brief Créer les données d'un client
 *
 * @return un pointer sur les **données client**
 */
extern client_data_t *createClientData();

/**
 * @brief Détruit les données d'un client
 *
 * @param server une référence d'un pointeur sur les données client
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteClientData(client_data_t **client_data);

#endif
