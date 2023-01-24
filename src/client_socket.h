#ifndef __CLIENT_SOCKET_H
#define __CLIENT_SOCKET_H

#include <netinet/in.h>
#include "socket.h"

typedef struct
{
    unsigned int address_length;
    struct sockaddr_in address;
    int socket_fd;
    ssize_t recv_length;
    packet_t *packet_buffer;
} client_t;

extern client_t *createClient(char *, uint16_t);

extern int sendToServer(client_t *, packet_t *);

extern packet_t *recvFromServer(client_t *);

extern int deleteClient(client_t **);

#endif