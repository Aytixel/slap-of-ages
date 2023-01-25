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

    if (setupAddress(&client->address, client->address_length, hostname, port) == -1)
    {
        free(client);

        return NULL;
    }

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

    client->packet_buffer = NULL;

    // set the socket to non blocking
    fcntl(client->socket_fd, F_SETFL, O_NONBLOCK);

    return client;
}

extern int sendToServer(client_t *client, packet_t *packet)
{
    if (send(client->socket_fd, &packet->data_length, sizeof(packet->data_length), 0) == -1)
        return -1;
    if (send(client->socket_fd, &packet->id, sizeof(packet->id), 0) == -1)
        return -1;
    if (send(client->socket_fd, packet->data, packet->data_length, 0) == -1)
        return -1;

    return 0;
}

extern packet_t *recvFromServer(client_t *client)
{
    // new packet setup
    if (client->packet_buffer == NULL)
    {
        size_t data_length = 0;

        if (recv(client->socket_fd, &data_length, sizeof(data_length), MSG_DONTWAIT) == -1)
            return NULL;

        client->recv_length = -1;
        client->packet_buffer = malloc(sizeof(packet_t));
        client->packet_buffer->data_length = data_length;
        client->packet_buffer->id = 0;
        client->packet_buffer->data = malloc(data_length);
    }

    // get packet id
    if (client->recv_length == -1)
    {
        uint8_t id = 0;

        if (recv(client->socket_fd, &id, sizeof(id), MSG_DONTWAIT) == -1)
            return NULL;

        client->recv_length = 0;
        client->packet_buffer->id = id;
    }

    // get packet data part
    if (client->recv_length < client->packet_buffer->data_length)
    {
        ssize_t recv_length = recv(client->socket_fd, client->packet_buffer->data + client->recv_length, client->packet_buffer->data_length - client->recv_length, MSG_DONTWAIT);

        if (recv_length > -1)
            client->recv_length += recv_length;

        if (client->recv_length < client->packet_buffer->data_length)
            return NULL;
    }

    // return completed packet
    packet_t *packet = client->packet_buffer;

    client->packet_buffer = NULL;

    return packet;
}

extern int deleteClient(client_t **client)
{
    if (client == NULL || *client == NULL)
        return -1;

    shutdown((*client)->socket_fd, SHUT_RDWR);
    close((*client)->socket_fd);

    if ((*client)->packet_buffer != NULL)
        deletePacket(&(*client)->packet_buffer);

    free(*client);
    *client = NULL;

    return 0;
}