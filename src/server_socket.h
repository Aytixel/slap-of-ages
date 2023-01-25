#ifndef __SERVER_SOCKET_H
#define __SERVER_SOCKET_H

#ifdef WIN32

#include <winsock2.h>

typedef struct SOCKADDR_IN sockaddr_in;

#else

#include <netinet/in.h>

#endif

#include "socket.h"

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
    ssize_t recv_length;
    packet_t *packet_buffer;
} server_client_t;

extern server_t *createServer(char *, uint16_t);

extern server_client_t *acceptServerClient(server_t *);

extern int sendToServerClient(server_client_t *, packet_t *);

extern packet_t *recvFromServerClient(server_client_t *);

extern int deleteServerClient(server_client_t **);

extern int deleteServer(server_t **);

#endif