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
#include "packet_id.h"
#include "server.h"

int server_client_count = 0;
int next_server_client_index = 0;
server_client_connection_t *server_client_connections = NULL;
int deleted_socket_fd_count = 0;
int *deleted_socket_fds = NULL;
server_client_t *server_client = NULL;
server_client_connection_state_e server_client_connection_state = SERVER_CLIENT_WAITING_HANDSHAKE;

/**
 * @brief Accept les nouvelles connexion de client
 *
 * @param server serveur à utiliser
 */
extern void acceptClientConnections(server_t *server)
{
    server_client_t *client = NULL;

    while ((client = acceptServerClient(server)) != NULL)
    {
        int server_client_connections_size = sizeof(server_client_connection_t) * ++server_client_count;

        if (server_client_connections == NULL)
            server_client_connections = malloc(server_client_connections_size);
        else
            server_client_connections = realloc(server_client_connections, server_client_connections_size);

        server_client_connection_t connection;

        connection.client = client;
        connection.state = SERVER_CLIENT_WAITING_HANDSHAKE;

        server_client_connections[server_client_count - 1] = connection;
    }
}

/**
 * @brief Permet d'itérer sur les connexions ouvertes
 *
 * Supprime aussi toute connexion fermé, et met leur id de socket dans le tableau de client supprimé
 *
 * @return **1** si l'on peut encore itérer, **0** sinon
 */
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
        server_client_connection_state = SERVER_CLIENT_WAITING_HANDSHAKE;

        return 0;
    }

    int i = 0;

    // cherche la première connexion ouverte et supprime les autres
    while (next_server_client_index + i < server_client_count && isClientDown(server_client_connections[next_server_client_index + i].client))
    {
        // ajoute le client qui va être supprimé au tableau de client supprimé
        int deleted_server_clients_size = sizeof(server_client_connection_t) * ++deleted_socket_fd_count;

        if (deleted_socket_fds == NULL)
            deleted_socket_fds = malloc(deleted_server_clients_size);
        else
            deleted_socket_fds = realloc(deleted_socket_fds, deleted_server_clients_size);

        deleted_socket_fds[deleted_socket_fd_count - 1] = server_client_connections[next_server_client_index + i].client->socket_fd;

        deleteServerClient(&server_client_connections[next_server_client_index + i++].client);
    }

    // si une connexion à été supprimé
    if (i)
    {
        server_client_count -= i;

        if (server_client_count) // supprime du tableau les connexions supprimé
        {
            memmove(server_client_connections + next_server_client_index, server_client_connections + next_server_client_index + i, sizeof(server_client_connection_t) * (server_client_count - next_server_client_index));

            server_client_connections = realloc(server_client_connections, sizeof(server_client_connection_t) * server_client_count);

            // dans le cas ou l'on arrive à la fin du tableau après réallocation
            if (next_server_client_index >= server_client_count)
            {
                next_server_client_index = 0;
                server_client = NULL;
                server_client_connection_state = SERVER_CLIENT_WAITING_HANDSHAKE;

                return 0;
            }
        }
        else // si il n'y a plus de connexion ouverte désalloue le tableau de connexion
        {
            free(server_client_connections);
            next_server_client_index = 0;
            server_client_connections = NULL;
            server_client = NULL;
            server_client_connection_state = SERVER_CLIENT_WAITING_HANDSHAKE;

            return 0;
        }
    }

    server_client = server_client_connections[next_server_client_index].client;
    server_client_connection_state = server_client_connections[next_server_client_index++].state;

    return 1;
}

/**
 * @brief Attend la poignée de main du client
 *
 * @param server serveur à utiliser
 * @return **0** tant que la poignée de main n'est pas effectuée, **1** si la poignée de main est réussie
 */
extern int waitClientHandshake()
{
    if (server_client_connection_state == SERVER_CLIENT_CONNECTED)
        return 1; // si la poignée de main est réussie

    packet_t *handshake_packet = recvFromServerClient(server_client);

    if (handshake_packet == NULL)
        return 0;

    int correct_packet = (handshake_packet->id == HANDSHAKE_PACKET_ID);

    if (correct_packet)
    {
        sendToServerClient(server_client, handshake_packet);

        server_client_connections[next_server_client_index - 1].state = SERVER_CLIENT_CONNECTED;
        server_client_connection_state = SERVER_CLIENT_CONNECTED;
    }

    deletePacket(&handshake_packet);

    return correct_packet;
}

/**
 * @brief Ferme toutes les connexions aux clients
 *
 */
extern void closeClientConnections()
{
    for (int i = 0; i < server_client_count; i++)
    {
        deleteServerClient(&server_client_connections[i].client);
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

    server_client = NULL;
    server_client_connection_state = SERVER_CLIENT_WAITING_HANDSHAKE;
}