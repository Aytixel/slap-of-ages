/**
 * @file server.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de connection/server.c
 * @version 0.1
 * @date 05/02/2023
 *
 */

#ifndef __SERVER_CONNECTION_H
#define __SERVER_CONNECTION_H

#include "socket/server.h"

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
    void *data;                             /**< données associées à la connexion*/
} server_client_connection_t;

/**
 * @brief Nombre d'id socket client supprimés
 *
 */
extern int deleted_socket_fd_count;

/**
 * @brief Id des sockets clients supprimés
 *
 */
extern int *deleted_socket_fds;

/**
 * @brief Pointeur vers la fonction de destruction des données associées à une connexion
 *
 * Doit impérativement être définie si des données sont stockées dans la structure d'une connexion, avec l'aide du pointeur server_client_data
 *
 */
extern int (*delete_server_client_data)(void **);

/**
 * @brief Client serveur socket courant
 *
 * Connexion courante itérer par nextClientConnection()
 *
 */
extern server_client_t *server_client;

/**
 * @brief Contient les données associées à la connexion courante
 *
 */
extern void **server_client_data;

/**
 * @brief Représente l'état de la connexion courante
 *
 */
extern server_client_connection_state_e server_client_state;

/**
 * @brief Accept les nouvelles connexion de client
 *
 * @param server serveur à utiliser
 */
extern void acceptClientConnections(server_t *server);

/**
 * @brief Permet d'itérer sur les connexions ouvertes
 *
 * Supprime aussi toute connexion fermé, et met leur id de socket dans le tableau de client supprimé
 *
 * @return **1** si l'on peut encore itérer, **0** sinon
 */
extern int nextClientConnection();

/**
 * @brief Attend la poignée de main du client
 *
 * @param server serveur à utiliser
 * @return **0** tant que la poignée de main n'est pas effectuée, **1** si la poignée de main est réussie
 */
extern int waitClientHandshake();

/**
 * @brief Ferme toutes les connexions aux clients
 *
 */
extern void closeClientConnections();

#endif