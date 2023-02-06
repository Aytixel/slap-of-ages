/**
 * @file client_connection.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de client_connection.c
 * @version 0.1
 * @date 05/02/2023
 *
 */

#ifndef __CLIENT_CONNECTION_H
#define __CLIENT_CONNECTION_H

#include "client_socket.h"

/**
 * @brief Représente l'état de la connexion
 *
 */
typedef enum
{
    CLIENT_WAITING_INFO,      /**< attend les informations de connexion*/
    CLIENT_WAITING_HANDSHAKE, /**< attend la poignée de main du serveur*/
    CLIENT_CONNECTED,         /**< connexion au serveur établie*/
    CLIENT_CLOSED,            /**< connexion au serveur fermé*/
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

extern int initClientConnection(char *, uint16_t);

extern int waitServerHandshake();

extern void closeClientConnection();

#endif