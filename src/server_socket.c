#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "server_socket.h"

extern server_t *createServer(uint16_t port)
{
    server_t *server = malloc(sizeof(server_t));

    server->address_length = sizeof(server->address);

    bzero(&server->address, server->address_length);

    server->address.sin_port = htons(port);
    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((server->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return NULL;

    // set the socket to non blocking
    fcntl(server->socket_fd, F_SETFL, O_NONBLOCK);

    bind(server->socket_fd, (struct sockaddr *)&server->address, server->address_length);
    listen(server->socket_fd, 5);

    return server;
}

extern server_client_t *acceptServerClient(server_t *server)
{
    if (server == NULL)
        return NULL;

    server_client_t *client = malloc(sizeof(server_client_t));

    client->address_length = sizeof(client->address);

    if ((client->socket_fd = accept(server->socket_fd, (struct sockaddr *)&client->address, &client->address_length)) == -1)
    {
        free(client);

        return NULL;
    }

    return client;
}

extern int deleteServerClient(server_client_t **client)
{
    if (client == NULL || *client == NULL)
        return -1;

    shutdown((*client)->socket_fd, SHUT_RDWR);
    close((*client)->socket_fd);

    free(*client);
    *client = NULL;

    return 0;
}

extern int deleteServer(server_t **server)
{
    if (server == NULL || *server == NULL)
        return -1;

    shutdown((*server)->socket_fd, SHUT_RDWR);
    close((*server)->socket_fd);

    free(*server);
    *server = NULL;

    return 0;
}