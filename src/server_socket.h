#ifndef __SERVER_SOCKET_H
#define __SERVER_SOCKET_H

#include <netinet/in.h>

typedef struct
{
    struct sockaddr_in address;
} server_client_t;

typedef struct
{
    struct sockaddr_in address;
    int socket_fd;
} server_t;

extern server_t *createServer(uint16_t);

extern void deleteServer(server_t **);

#endif