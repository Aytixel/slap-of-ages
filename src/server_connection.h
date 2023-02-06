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
 * @brief Client serveur socket courant
 *
 * Connexion courante itérer par nextClientConnection()
 *
 */
extern server_client_t *server_client;

extern void acceptClientConnections(server_t *);

extern int nextClientConnection();

extern void closeClientConnections();

#endif