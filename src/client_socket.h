#ifndef __CLIENT_SOCKET_H
#define __CLIENT_SOCKET_H

#include <netinet/in.h>
#include "socket.h"

typedef struct
{
    unsigned int address_length;
    struct sockaddr_in address;
    int socket_fd;
} client_t;

extern client_t *createClient(char *, uint16_t);

extern int sendToServer(client_t *, packet_t *);

extern int deleteClient(client_t **);

#endif