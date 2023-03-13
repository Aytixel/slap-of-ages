/**
 * @file socket.c
 * @author Lucas Dureau
 * @brief Enssemble de fonctions utilitaire pour l'utilisation des sockets
 * @version 0.1
 * @date 31/01/2023
 *
 */

#ifdef WIN32

#include <winsock2.h>

#else

#include <netdb.h>
#include <arpa/inet.h>

#endif

#include <stdlib.h>
#include <string.h>
#include "packet/packet.h"
#include "socket.h"

extern int initSocket()
{
#ifdef WIN32

    WSADATA wsa;

    return WSAStartup(MAKEWORD(2, 2), &wsa);

#endif

    return 0;
}

extern void endSocket()
{
#ifdef WIN32

    WSACleanup();

#endif
}

extern int deletePacket(packet_t **packet)
{
    if (packet == NULL || *packet == NULL)
        return -1;

    free((*packet)->data);
    free(*packet);
    *packet = NULL;

    return 0;
}

extern int setupAddress(struct sockaddr_in *address, size_t address_length, char *hostname, uint16_t port)
{
    memset(address, 0, address_length);

    address->sin_port = htons(port);
    address->sin_family = AF_INET;

    struct hostent *host_info = gethostbyname(hostname);

    if (host_info != NULL)
    {
        memcpy(&address->sin_addr, host_info->h_addr_list[0], host_info->h_length);

        return 0;
    }

    long host_address = inet_addr(hostname);

    if (host_address == -1)
        return -1;

    memcpy(&address->sin_addr, &host_address, sizeof(host_address));

    return 0;
}