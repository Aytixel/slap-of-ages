/**
 * @file server_socket.c
 * @author Lucas Dureau
 * @brief Serveur socket simple permettant l'échange de paquet de manière non bloquante
 * @version 0.1
 * @date 31/01/2023
 *
 */

#ifdef WIN32

#include <winsock2.h>

#define close closesocket
#define SHUT_RDWR SD_BOTH

#else

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

#endif

#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include "socket.h"
#include "server_socket.h"

/**
 * @brief Créer un server
 *
 * @param hostname nom d'hôte à écouter
 * @param port port à écouter
 * @return un pointer sur un **server**
 */
extern server_t *createServer(char *hostname, uint16_t port)
{
    server_t *server = malloc(sizeof(server_t));

    server->address_length = sizeof(server->address);

    if (setupAddress(&server->address, server->address_length, hostname, port) == -1)
    {
        free(server);

        return NULL;
    }

    if ((server->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        free(server);

        return NULL;
    }

    // dit au socket qu'il peut réutiliser la même adresse
    int optval = 1;

    setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // définit le socket comme non bloquant
#ifdef WIN32
    u_long mode = 1;

    ioctlsocket(server->socket_fd, FIONBIO, &mode);
#else
    fcntl(server->socket_fd, F_SETFL, O_NONBLOCK);
#endif

    bind(server->socket_fd, (struct sockaddr *)&server->address, server->address_length);
    listen(server->socket_fd, 5);

    return server;
}

/**
 * @brief Vérifie si il y a des connexions à accepter, et les accepte
 *
 * @param server serveur à utiliser
 * @return un pointeur sur un **client serveur**
 */
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

/**
 * @brief Vérifie si la connexion au client est fermée
 *
 * @param client client serveur à utiliser
 * @return **1 ou 0** en fonction se si la connexion est fermer ou non
 */
extern int isClientDown(server_client_t *client)
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
        if (recv(client->socket_fd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0)
            return 1;
    }

    return 0;
}

/**
 * @brief Envoie un paquet au client
 *
 * @param client client serveur à utiliser
 * @param packet paquet à envoyer
 * @return **0** si tous se passe bien, **-1** si il y a un problème durant l'envoie
 */
extern int sendToServerClient(server_client_t *client, packet_t *packet)
{
    void *buffer = malloc(sizeof(packet->data_length) + sizeof(packet->id) + packet->data_length);

    memcpy(buffer, &packet->data_length, sizeof(packet->data_length));
    memcpy(buffer + sizeof(packet->data_length), &packet->id, sizeof(packet->data_length));
    memcpy(buffer + sizeof(packet->data_length) + sizeof(packet->id), packet->data, sizeof(packet->data_length));

    if (send(client->socket_fd, buffer, sizeof(packet->data_length) + sizeof(packet->id) + packet->data_length, 0) == -1)
    {
        free(buffer);

        return -1;
    }

    free(buffer);

    return 0;
}

/**
 * @brief Essaye de recevoir un paquet du client
 *
 * @param client client serveur à utiliser
 * @return un pointeur sur un **paquet**, et si la réception n'est pas terminé, ou rien n'a été envoyer renvoie **null**
 */
extern packet_t *recvFromServerClient(server_client_t *client)
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

/**
 * @brief Détruit un client serveur
 *
 * @param client une référence d'un pointeur sur un client serveur
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteServerClient(server_client_t **client)
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

/**
 * @brief Détruit un seveur
 *
 * @param server une référence d'un pointeur sur un serveur
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
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