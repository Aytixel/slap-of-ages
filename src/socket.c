#include <stdlib.h>
#include <strings.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "socket.h"

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
    bzero(address, address_length);

    address->sin_port = htons(port);
    address->sin_family = AF_INET;

    struct hostent *host_info = gethostbyname(hostname);

    if (host_info != NULL)
    {
        bcopy(host_info->h_addr_list[0], &address->sin_addr, host_info->h_length);

        return 0;
    }

    long host_address = inet_addr(hostname);

    if (host_address == -1)
        return -1;

    bcopy(&host_address, &address->sin_addr, sizeof(host_address));

    return 0;
}