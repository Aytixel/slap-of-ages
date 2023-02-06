/**
 * @file server_connection.c
 * @author Lucas Dureau
 * @brief Permet de gérer la connexion des clients au serveur
 * @version 0.1
 * @date 05/02/2023
 *
 */

#include <stdlib.h>
#include <string.h>
#include "server_connection.h"

int server_client_count = 0;
server_client_t **server_clients = NULL;
int current_server_client = 0;
server_client_t *server_client = NULL;

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
        int server_clients_size = sizeof(server_client_t *) * ++server_client_count;

        if (server_clients == NULL)
            server_clients = malloc(server_clients_size);
        else
            server_clients = realloc(server_clients, server_clients_size);

        server_clients[server_client_count - 1] = client;
    }
}

/**
 * @brief Permet d'itérer sur les connexions ouvertes
 *
 * Supprime aussi toute connexion fermé
 *
 * @return **1** si l'on peut encore itérer, **0** sinon
 */
extern int nextClientConnection()
{
    // dans le cas ou l'on arrive à la fin du tableau
    if (current_server_client == server_client_count)
    {
        current_server_client = 0;
        server_client = NULL;

        return 0;
    }

    int i = 0;

    // cherche la première connexion ouverte et supprime les autres
    while (current_server_client + i < server_client_count && isClientDown(server_clients[current_server_client + i]))
        deleteServerClient(server_clients + current_server_client + i++);

    // si une connexion à été supprimé
    if (i)
    {
        server_client_count -= i;

        if (server_client_count) // supprime du tableau les connexions supprimé
        {
            memmove(server_clients + current_server_client, server_clients + current_server_client + i, sizeof(*server_clients) * (server_client_count - current_server_client));

            server_clients = realloc(server_clients, sizeof(*server_clients) * server_client_count);
        }
        else // si il n'y a plus de connexion ouverte désalloue le tableau de connexion
        {
            free(server_clients);
            server_clients = NULL;
            current_server_client = 0;
            server_client = NULL;

            return 0;
        }
    }

    server_client = server_clients[current_server_client++];

    return 1;
}

/**
 * @brief Ferme toutes les connexions aux clients
 *
 */
extern void closeClientConnections()
{
    for (int i = 0; i < server_client_count; i++)
    {
        deleteServerClient(server_clients + i);
    }

    free(server_clients);
    server_client_count = 0;
    server_clients = NULL;
    server_client = NULL;
}