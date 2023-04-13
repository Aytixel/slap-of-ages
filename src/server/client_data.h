/**
 * @file client_data.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de server/client_data.c
 * @version 1.1
 * @date 02/03/2023
 *
 */

#ifndef __SERVER_CLIENT_DATA_H
#define __SERVER_CLIENT_DATA_H

typedef struct server_game_state_s server_game_state_t;

#include "packet/packet.h"
#include "game_state.h"

/**
 * @brief Structure de données associée à un client du serveur
 *
 */
typedef struct server_client_data_s
{
    char *pseudo;                    /**< pseudo du joueur*/
    int is_player_ready;             /**< le joueur est-il prêt ?*/
    int is_in_game;                  /**< le joueur est-il en jeu ?*/
    server_game_state_t *game_state; /**< un pointeur sur les données de partie*/
    packet_t *map_packet;            /**< paquet contenant les données de la carte*/
} server_client_data_t;

/**
 * @brief Créer les données d'un client
 *
 * @return un pointer sur les **données client**
 */
extern server_client_data_t *createServerClientData();

/**
 * @brief Détruit les données d'un client
 *
 * @param client_data une référence d'un pointeur sur les données client
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteServerClientData(server_client_data_t **client_data);

#endif
