/**
 * @file client.c
 * @author Lucas Dureau
 * @brief Permet de gérer la connexion du client au serveur
 * @version 1.1
 * @date 05/02/2023
 *
 */

#include "client.h"

int handshake_data = 0;
client_t *client = NULL;
client_connection_state_e client_connection_state = CLIENT_WAITING_INFO;

extern int initClientConnection(char *hostname, uint16_t port)
{
    if (client_connection_state == CLIENT_WAITING_INFO)
    {
        client = createClient(hostname, port);

        if (client == NULL)
            return -1;

        packet_t *handshake_packet = createHandshakePacket(handshake_data = rand());

        if (sendToServer(client, handshake_packet) == -1)
        {
            deleteClient(&client);

            return -1;
        }

        deletePacket(&handshake_packet);

        client_connection_state = CLIENT_WAITING_HANDSHAKE;
    }

    return 0;
}

extern int waitServerHandshake()
{
    if (client_connection_state == CLIENT_WAITING_INFO)
        return 0; // tant que la poignée de main n'est pas effectuée
    if (client_connection_state == CLIENT_CONNECTED)
        return 1; // si la poignée de main est réussie

    packet_t *packet = recvFromServer(client);

    if (packet == NULL)
        return 0; // tant que la poignée de main n'est pas effectuée

    if (packet->id == HANDSHAKE_PACKET_ID)
    {
        int handshake_received_data;

        readHandshakePacket(packet, &handshake_received_data);

        if (handshake_received_data == handshake_data)
        {
            deletePacket(&packet);

            client_connection_state = CLIENT_CONNECTED;

            return 1; // si la poignée de main est réussie
        }
    }

    deletePacket(&packet);
    closeClientConnection();

    client_connection_state = CLIENT_WAITING_INFO;

    return -1;
}

extern void closeClientConnection()
{
    if (client_connection_state == CLIENT_WAITING_INFO)
        return;

    deleteClient(&client);
}