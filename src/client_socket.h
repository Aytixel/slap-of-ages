/**
 * @file client_socket.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de client_socket.c
 * @version 0.1
 * @date 31/01/2023
 *
 */

#ifndef __CLIENT_SOCKET_H
#define __CLIENT_SOCKET_H

#ifndef WIN32

#include <netinet/in.h>

#endif

#include "socket.h"

/**
 * @brief Client socket
 *
 */
typedef struct
{
    unsigned int address_length; /**< longueur de l'adresse client*/
    struct sockaddr_in address;  /**< adresse du client*/
    int socket_fd;               /**< descipteur du socket client*/
    ssize_t recv_length;         /**< longueur des données reçues*/
    packet_t *packet_buffer;     /**< buffer de paquet*/
} client_t;

extern client_t *createClient(char *, uint16_t);

extern int sendToServer(client_t *, packet_t *);

extern packet_t *recvFromServer(client_t *);

extern int deleteClient(client_t **);

#endif