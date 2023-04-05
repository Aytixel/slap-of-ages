#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "socket/server.h"
#include "socket/client.h"

int main(int argc, char *argv[])
{
    printf("Test de socket/server.h socket/client.h socket/socket.h\n\n");

    assert(initSocket() == 0);
    printf("Initialisation du socket  OK\n");

    server_t *server = createServer("0.0.0.0", 4539);

    assert(server != NULL);
    printf("Création du serveur OK\n");

    server_client_t *server_client = acceptServerClient(server);

    assert(server_client == NULL);
    printf("Le serveur essait d'accepter un client sans bloquer OK\n");

    client_t *client = createClient("localhost", 4539);

    assert(client != NULL);
    printf("Création du client OK\n");

    // --------------------------------------------------------------------

    server_client = acceptServerClient(server);

    assert(server_client != NULL);
    printf("\n\tLe serveur accepte un client OK\n");

    // --------------------------------------------------------------------

    long test_data = 2352850823;
    packet_t test_packet = {10, &test_data, sizeof(test_data)};

    assert(!sendToServer(client, &test_packet));
    printf("\n\t\tDonnées envoyer au serveur OK\n");

    long data = 0;
    packet_t *packet = recvFromServerClient(server_client);

    assert(packet != NULL);

    memcpy(&data, packet->data, packet->data_length);

    assert(test_data == data);
    assert(deletePacket(&packet) != -1);
    printf("\t\tLe serveur reçoit les données OK\n");

    // --------------------------------------------------------------------

    assert(!sendToServerClient(server_client, &test_packet));
    printf("\n\t\tDonnées envoyer au client OK\n");

    data = 0;
    packet = recvFromServer(client);

    assert(packet != NULL);

    memcpy(&data, packet->data, packet->data_length);

    assert(test_data == data);
    assert(deletePacket(&packet) != -1);
    printf("\t\tLe client reçoit les données OK\n");

    // --------------------------------------------------------------------

    assert(!deleteServerClient(&server_client));
    assert(server_client == NULL);
    printf("\n\tDestruction du client côté serveur OK\n");

    // --------------------------------------------------------------------

    assert(!deleteClient(&client));
    assert(client == NULL);
    printf("\nDestruction du client OK\n");

    assert(!deleteServer(&server));
    assert(server == NULL);
    printf("Destruction du serveur OK\n");

    // --------------------------------------------------------------------

    printf("\nTest des fonctions pour vérifier l'état d'un connexion :\n");

    // --------------------------------------------------------------------

    server = createServer("0.0.0.0", 4539);
    client = createClient("localhost", 4539);
    server_client = acceptServerClient(server);

    assert(isServerDown(client) == 0);
    printf("\n\tConnexion au serveur ouverte OK\n");

    assert(isClientDown(server_client) == 0);
    printf("\tConnexion au client ouverte OK\n");

    // --------------------------------------------------------------------

    deleteClient(&client);

    assert(isClientDown(server_client) == 1);
    printf("\n\tConnexion au client fermée OK\n");

    deleteServerClient(&server_client);

    // --------------------------------------------------------------------

    client = createClient("localhost", 4539);
    server_client = acceptServerClient(server);

    deleteServerClient(&server_client);

    assert(isServerDown(client) == 1);
    printf("\n\tConnexion au serveur fermée OK\n");

    deleteClient(&client);
    deleteServer(&server);

    // --------------------------------------------------------------------

    assert(isClientDown(server_client) == 1);
    printf("\n\tConnexion au client fermée OK\n");

    assert(isServerDown(client) == 1);
    printf("\tConnexion au serveur fermée OK\n");

    endSocket();

    printf("\nTest de socket/server.h socket/client.h socket/socket.h succès\n");

    return 0;
}