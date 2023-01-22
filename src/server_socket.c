#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "server_socket.h"

extern server_t *createServer(uint16_t port)
{
    server_t *server = malloc(sizeof(server_t));

    bzero(&server->address, sizeof(struct sockaddr_in));

    server->address.sin_port = htons(port);
    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((server->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return NULL;

    bind(server->socket_fd, (struct sockaddr *)&server->address, sizeof(struct sockaddr_in));
    listen(server->socket_fd, 5);

    return server;
}

extern void deleteServer(server_t **server)
{
    shutdown((*server)->socket_fd, SHUT_RDWR);
    close((*server)->socket_fd);

    free(*server);
    *server = NULL;
}