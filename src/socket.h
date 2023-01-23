#ifndef __SOCKET_H
#define __SOCKET_H

#include <netinet/in.h>

typedef struct
{
    uint8_t id;
    void *data;
    size_t data_length;
} packet_t;

extern int setupAddress(struct sockaddr_in *, size_t, char *, uint16_t);

#endif