/**
 * @file client.c
 * @author Lucas Dureau
 * @brief Client socket simple permettant l'échange de paquet de manière non bloquante
 * @version 1.1
 * @date 31/01/2023
 *
 */

#include "client.h"

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
    client->recv_length = -1;

    // définit le socket comme non bloquant
#ifdef WIN32
    u_long mode = 1;

    ioctlsocket(client->socket_fd, FIONBIO, &mode);
#else
    fcntl(client->socket_fd, F_SETFL, O_NONBLOCK);
#endif

    return client;
}

extern int isServerDown(client_t *client)
{
    if (client == NULL)
        return 1;

    struct pollfd pollfd;

    pollfd.fd = client->socket_fd;
    pollfd.events = POLLIN | POLLHUP;
    pollfd.revents = 0;

    if (poll(&pollfd, 1, 0) > 0)
    {
        char buffer[8];

        if (recv(client->socket_fd, buffer, sizeof(buffer), MSG_PEEK) == 0)
            return 1;
    }

    return 0;
}

extern int sendToServer(client_t *client, packet_t *packet)
{
    void *buffer = malloc(sizeof(packet->data_length) + sizeof(packet->id) + packet->data_length);

    memcpy(buffer, &packet->data_length, sizeof(packet->data_length));
    memcpy(buffer + sizeof(packet->data_length), &packet->id, sizeof(packet->id));
    memcpy(buffer + sizeof(packet->data_length) + sizeof(packet->id), packet->data, packet->data_length);

    if (send(client->socket_fd, buffer, sizeof(packet->data_length) + sizeof(packet->id) + packet->data_length, 0) == -1)
    {
        free(buffer);

        return -1;
    }

    free(buffer);

    return 0;
}

extern packet_t *recvFromServer(client_t *client)
{
    // création d'un nouveau paquet
    if (client->packet_buffer == NULL)
    {
        size_t data_length = 0;

        if (recv(client->socket_fd, (char *)&data_length, sizeof(data_length), 0) == -1)
            return NULL;

        client->recv_length = -1;
        client->packet_buffer = malloc(sizeof(packet_t));
        client->packet_buffer->data_length = data_length;
        client->packet_buffer->id = 0;
        client->packet_buffer->data = malloc(data_length);
    }

    // récupération de l'id du paquet
    if (client->recv_length == -1)
    {
        uint8_t id = 0;

        if (recv(client->socket_fd, (char *)&id, sizeof(id), 0) == -1)
            return NULL;

        client->recv_length = 0;
        client->packet_buffer->id = id;
    }

    // récupération des données du paquet
    if (client->recv_length < client->packet_buffer->data_length)
    {
        ssize_t recv_length = recv(client->socket_fd, client->packet_buffer->data + client->recv_length, client->packet_buffer->data_length - client->recv_length, 0);

        if (recv_length > -1)
            client->recv_length += recv_length;

        if (client->recv_length < client->packet_buffer->data_length)
            return NULL;
    }

    // retour du paquet compléter
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