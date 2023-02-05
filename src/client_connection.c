/**
 * @file client_connection.c
 * @author Lucas Dureau
 * @brief Permet de gérer la connexion du client au serveur
 * @version 0.1
 * @date 05/02/2023
 *
 */

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "packet_id.h"
#include "client_connection.h"

int handshake_data = 0;
client_t *client = NULL;
client_connection_state_e client_connection_state = CLIENT_WAITING_INFO;

/**
 * @brief Initialise la connexion au serveur avec les paramètres spécifiés
 *
 * Envoie aussi un packet pour la poignée de main avec le serveur
 *
 * @param hostname nom d'hôte du serveur
 * @param port port du serveur
 * @return **0** si tous se passe bien, **-1** si la connexion à échoué
 */
int initClientConnection(char *hostname, uint16_t port)
{
    if (client_connection_state == CLIENT_WAITING_INFO)
    {
        client = createClient(hostname, port);

        if (client == NULL)
            return -1;

        handshake_data = rand();

        packet_t handshake_packet = {HANDSHAKE_PACKET_ID, &handshake_data, sizeof(handshake_data)};

        if (sendToServer(client, &handshake_packet) == -1)
        {
            deleteClient(&client);

            return -1;
        }

        client_connection_state = CLIENT_WAITING_HANDSHAKE;
    }

    return 0;
}

/**
 * @brief Attend la poignée de main du serveur
 *
 * @return **0** tant que la poignée de main n'est pas effectuée, **1** si la poignée de main est réussie, **-1** si la poignée de main échoue
 */
int waitServerHandshake()
{
    if (client_connection_state == CLIENT_WAITING_INFO)
        return 0; // tant que la poignée de main n'est pas effectuée
    if (client_connection_state == CLIENT_CONNECTED)
        return 1; // si la poignée de main est réussie

    packet_t *packet = recvFromServer(client);

    if (packet == NULL)
        return 0; // tant que la poignée de main n'est pas effectuée

    if (packet->id == HANDSHAKE_PACKET_ID && sizeof(handshake_data) == packet->data_length && memcmp(&handshake_data, packet->data, sizeof(handshake_data)) == 0)
    {
        deletePacket(&packet);

        client_connection_state = CLIENT_CONNECTED;

        return 1; // si la poignée de main est réussie
    }

    deletePacket(&packet);
    closeClientConnection();

    client_connection_state = CLIENT_WAITING_INFO;

    return -1;
}

/**
 * @brief Ferme la connexion au serveur
 *
 * @return **0** si tous c'est bien passé, **-1** si il y a eu un problème
 */
int closeClientConnection()
{
    if (client_connection_state == CLIENT_CONNECTED || client_connection_state == CLIENT_WAITING_INFO)
        return 0;

    return deleteClient(&client);
}