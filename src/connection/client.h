/**
 * @file client.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de connection/client.c
 * @version 1.1
 * @date 05/02/2023
 *
 */

#ifndef __CLIENT_CONNECTION_H
#define __CLIENT_CONNECTION_H

#include <stdlib.h>
#include <string.h>
#include "socket/client.h"

/**
 * @brief Représente l'état de la connexion
 *
 */
typedef enum
{
    CLIENT_WAITING_INFO,      /**< attend les informations de connexion*/
    CLIENT_WAITING_HANDSHAKE, /**< attend la poignée de main du serveur*/
    CLIENT_CONNECTED,         /**< connexion au serveur établie*/
} client_connection_state_e;

/**
 * @brief Client socket
 *
 */
extern client_t *client;

/**
 * @brief Représente l'état courant de la connexion
 *
 */
extern client_connection_state_e client_connection_state;

/**
 * @brief Initialise la connexion au serveur avec les paramètres spécifiés
 *
 * Envoie aussi un packet pour la poignée de main avec le serveur
 *
 * @param hostname nom d'hôte du serveur
 * @param port port du serveur
 * @return **0** si tous se passe bien, **-1** si la connexion à échoué
 */
extern int initClientConnection(char *hostname, uint16_t port);

/**
 * @brief Attend la poignée de main du serveur
 *
 * @return **0** tant que la poignée de main n'est pas effectuée, **1** si la poignée de main est réussie, **-1** si la poignée de main échoue
 */
extern int waitServerHandshake();

/**
 * @brief Ferme la connexion au serveur
 *
 */
extern void closeClientConnection();

#endif