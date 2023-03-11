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

/**
 * @brief Structure de données associée à un client du serveur
 *
 */
typedef struct
{
    char *pseudo;        /**< pseudo du joueur*/
    int is_player_ready; /**< le joueur est-il prêt ?*/
    int is_in_game;      /**< le joueur est-il en jeu ?*/
} client_data_t;

extern client_data_t *createClientData();

extern int deleteClientData(client_data_t **client_data);

#endif
