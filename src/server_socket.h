#ifndef __SERVER_SOCKET_H
#define __SERVER_SOCKET_H

#include <netinet/in.h>

typedef struct
{
    unsigned int address_length;
    struct sockaddr_in address;
    int socket_fd;
} server_t;

typedef struct
{
    unsigned int address_length;
    struct sockaddr_in address;
    int socket_fd;
} server_client_t;

extern server_t *createServer(uint16_t);

extern server_client_t *acceptServerClient(server_t *);

extern int deleteServerClient(server_client_t **);

extern int deleteServer(server_t **);

#endif