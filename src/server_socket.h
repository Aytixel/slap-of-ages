/**
 * @file server_socket.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de server_socket.c
 * @version 0.1
 * @date 31/01/2023
 *
 */

#ifndef __SERVER_SOCKET_H
#define __SERVER_SOCKET_H

#ifdef WIN32

#include <winsock2.h>

typedef struct SOCKADDR_IN sockaddr_in;

#else

#include <netinet/in.h>

#endif

#include "socket.h"

/**
 * @brief Serveur socket
 *
 */
typedef struct
{
    unsigned int address_length; /**< longueur de l'adresse serveur*/
    struct sockaddr_in address;  /**< adresse du serveur*/
    int socket_fd;               /**< descipteur du socket serveur*/
} server_t;

/**
 * @brief Client serveur socket
 *
 */
typedef struct
{
    unsigned int address_length; /**< longueur de l'adresse client*/
    struct sockaddr_in address;  /**< adresse du client*/
    int socket_fd;               /**< descipteur du socket client*/
    ssize_t recv_length;         /**< longueur des données reçues*/
    packet_t *packet_buffer;     /**< buffer de paquet*/
} server_client_t;

extern server_t *createServer(char *, uint16_t);

extern server_client_t *acceptServerClient(server_t *);

extern int isClientDown(server_client_t *);

extern int sendToServerClient(server_client_t *, packet_t *);

extern packet_t *recvFromServerClient(server_client_t *);

extern int deleteServerClient(server_client_t **);

extern int deleteServer(server_t **);

#endif