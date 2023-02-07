/**
 * @file server_connection.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de server_connection.c
 * @version 0.1
 * @date 05/02/2023
 *
 */

#ifndef __SERVER_CONNECTION_H
#define __SERVER_CONNECTION_H

#include "server_socket.h"

/**
 * @brief Représente l'état d'une connexion
 *
 */
typedef enum
{
    SERVER_CLIENT_WAITING_HANDSHAKE, /**< attend la poignée de main du client*/
    SERVER_CLIENT_CONNECTED,         /**< connexion au client établie*/
} server_client_connection_state_e;

/**
 * @brief Représente une connexion au serveur
 *
 */
typedef struct
{
    server_client_t *client;                /**< client socket*/
    server_client_connection_state_e state; /**< état de la connexion*/
} server_client_connection_t;

/**
 * @brief Client serveur socket courant
 *
 * Connexion courante itérer par nextClientConnection()
 *
 */
extern server_client_t *server_client;

/**
 * @brief Représente l'état courant de la connexion coursante
 *
 */
extern server_client_connection_state_e server_client_connection_state;

extern void acceptClientConnections(server_t *);

extern int nextClientConnection();

extern int waitClientHandshake();

extern void closeClientConnections();

#endif