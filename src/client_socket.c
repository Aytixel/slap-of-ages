#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"
#include "client_socket.h"

extern client_t *createClient(char *hostname, uint16_t port)
{
    client_t *client = malloc(sizeof(client_t));

    client->address_length = sizeof(client->address);

    setupAddress(&client->address, client->address_length, hostname, port);

    if ((client->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        free(client);

        return NULL;
    }

    if (connect(client->socket_fd, (struct sockaddr *)&client->address, client->address_length) == -1)
    {
        free(client);

        return NULL;
    }

    // set the socket to non blocking
    fcntl(client->socket_fd, F_SETFL, O_NONBLOCK);

    return client;
}

extern int deleteClient(client_t **client)
{
    if (client == NULL || *client == NULL)
        return -1;

    shutdown((*client)->socket_fd, SHUT_RDWR);
    close((*client)->socket_fd);

    free(*client);
    *client = NULL;

    return 0;
}