/**
 * @file server.c
 * @author Lucas Dureau
 * @brief Permet de gérer la connexion des clients au serveur
 * @version 0.1
 * @date 05/02/2023
 *
 */

#include <stdlib.h>
#include <string.h>
#include "server.h"

int server_client_count = 0;
int server_client_capacity = 1;
int next_server_client_index = 0;
server_client_connection_t *server_client_connections = NULL;
int deleted_socket_fd_count = 0;
int *deleted_socket_fds = NULL;
int (*delete_server_client_data)(void **) = NULL;
server_client_t *server_client = NULL;
void **server_client_data = NULL;
server_client_connection_state_e server_client_state = SERVER_CLIENT_WAITING_HANDSHAKE;

extern void acceptClientConnections(server_t *server)
{
    server_client_t *client = NULL;

    while ((client = acceptServerClient(server)) != NULL)
    {
        if (server_client_connections == NULL)
            server_client_connections = malloc(sizeof(server_client_connection_t) * server_client_capacity);

        if (server_client_capacity == server_client_count)
        {
            server_client_capacity *= 2;
            server_client_connections = realloc(server_client_connections, sizeof(server_client_connection_t) * server_client_capacity);
        }

        server_client_connection_t connection;

        connection.client = client;
        connection.data = NULL;
        connection.state = SERVER_CLIENT_WAITING_HANDSHAKE;

        server_client_connections[server_client_count++] = connection;
    }
}

extern int nextClientConnection()
{
    // reset le tableau de client supprimé quand on commence à itérer
    if (next_server_client_index == 0 && deleted_socket_fds != NULL)
    {
        free(deleted_socket_fds);
        deleted_socket_fds = NULL;
        deleted_socket_fd_count = 0;
    }

    // dans le cas ou l'on arrive à la fin du tableau
    if (next_server_client_index >= server_client_count)
    {
        next_server_client_index = 0;
        server_client = NULL;
        server_client_data = NULL;
        server_client_state = SERVER_CLIENT_WAITING_HANDSHAKE;

        return 0;
    }

    int i = 0;

    // cherche la première connexion ouverte et supprime les connexions fermées
    while (next_server_client_index + i < server_client_count && isClientDown(server_client_connections[next_server_client_index + i].client))
    {
        // ajoute le client qui va être supprimé au tableau de client supprimé
        deleted_socket_fds = realloc(deleted_socket_fds, sizeof(server_client_connection_t) * ++deleted_socket_fd_count);
        deleted_socket_fds[deleted_socket_fd_count - 1] = server_client_connections[next_server_client_index + i].client->socket_fd;

        deleteServerClient(&server_client_connections[next_server_client_index + i].client);

        if (delete_server_client_data != NULL)
            delete_server_client_data(&server_client_connections[next_server_client_index + i].data);

        i++;
    }

    // si une connexion à été supprimé
    if (i)
    {
        server_client_count -= i;

        // supprime du tableau les connexions supprimé
        memmove(server_client_connections + next_server_client_index, server_client_connections + next_server_client_index + i, sizeof(server_client_connection_t) * (server_client_count - next_server_client_index));

        // dans le cas ou l'on arrive à la fin du tableau après réallocation
        if (next_server_client_index >= server_client_count)
        {
            next_server_client_index = 0;
            server_client = NULL;
            server_client_data = NULL;
            server_client_state = SERVER_CLIENT_WAITING_HANDSHAKE;

            return 0;
        }
    }

    server_client = server_client_connections[next_server_client_index].client;
    server_client_data = &server_client_connections[next_server_client_index].data;
    server_client_state = server_client_connections[next_server_client_index++].state;

    return 1;
}

extern int waitClientHandshake()
{
    if (server_client_state == SERVER_CLIENT_CONNECTED)
        return 1; // si la poignée de main est réussie

    packet_t *handshake_packet = recvFromServerClient(server_client);

    if (handshake_packet == NULL)
        return 0;

    int correct_packet = (handshake_packet->id == HANDSHAKE_PACKET_ID);

    if (correct_packet)
    {
        sendToServerClient(server_client, handshake_packet);

        server_client_connections[next_server_client_index - 1].state = SERVER_CLIENT_CONNECTED;
        server_client_state = SERVER_CLIENT_CONNECTED;
    }

    deletePacket(&handshake_packet);

    return correct_packet;
}

extern void closeClientConnections()
{
    for (int i = 0; i < server_client_count; i++)
    {
        deleteServerClient(&server_client_connections[i].client);

        if (delete_server_client_data != NULL)
            delete_server_client_data(&server_client_connections[i].data);
    }

    if (server_client_connections != NULL)
    {
        free(server_client_connections);
        server_client_connections = NULL;
        server_client_count = 0;
    }

    if (deleted_socket_fds != NULL)
    {
        free(deleted_socket_fds);
        deleted_socket_fds = NULL;
        deleted_socket_fd_count = 0;
    }

    delete_server_client_data = NULL;
    server_client = NULL;
    server_client_data = NULL;
    server_client_state = SERVER_CLIENT_WAITING_HANDSHAKE;
}