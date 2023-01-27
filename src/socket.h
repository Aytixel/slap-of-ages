#ifndef __SOCKET_H
#define __SOCKET_H

#ifdef WIN32

#include <stdint.h>
#include <winsock2.h>

typedef struct SOCKADDR_IN sockaddr_in;

#else

#include <netinet/in.h>

#endif

typedef struct
{
    uint8_t id;
    void *data;
    size_t data_length;
} packet_t;

extern int initSocket();

extern void endSocket();

extern int deletePacket(packet_t **);

extern int setupAddress(struct sockaddr_in *, size_t, char *, uint16_t);

#endif