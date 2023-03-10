/**
 * @file client.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de connection/client.c
 * @version 0.1
 * @date 05/02/2023
 *
 */

#ifndef __CLIENT_CONNECTION_H
#define __CLIENT_CONNECTION_H

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

extern int initClientConnection(char *hostname, uint16_t port);

extern int waitServerHandshake();

extern void closeClientConnection();

#endif