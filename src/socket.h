#ifndef __SOCKET_H
#define __SOCKET_H

#include <netinet/in.h>

extern int setupAddress(struct sockaddr_in *, size_t, char *, uint16_t);

#endif