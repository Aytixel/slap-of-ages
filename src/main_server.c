#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "timer/timer.h"
#include "connection/server.h"
#include "server/client_data.h"
#include "utils/getopt.h"

#define DEFAULT_HOSTNAME "0.0.0.0"
#define DEFAULT_PORT 4539

int running = 1;

void signalHandler(int s)
{
    running = 0;
}

void get_connection_info(int argc, char *argv[], char **hostname, uint16_t *port)
{
    struct option longopts[] = {
        {"hostname", required_argument, NULL, (int)'h'},
        {"port", required_argument, NULL, (int)'p'},
        {0, 0, 0, 0}};
    int opt;

    *hostname = NULL;
    *port = 0;

    while ((opt = getopt_long(argc, argv, "h:p:", longopts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'h':;
            ssize_t hostname_size = sizeof(char) * (strlen(optarg) + 1);

            *hostname = (*hostname == NULL)
                            ? malloc(hostname_size)
                            : realloc(*hostname, hostname_size);

            strcpy(*hostname, optarg);
            break;
        case 'p':
            *port = atoi(optarg);
            break;
        }
    }

    if (!*port)
        *port = DEFAULT_PORT;

    if (*hostname == NULL)
    {
        *hostname = malloc(strlen(DEFAULT_HOSTNAME) + 1);
        strcpy(*hostname, DEFAULT_HOSTNAME);
    }
}

void handle_packet(packet_t *packet)
{
    client_data_t *client_data = *server_client_data;

    switch (packet->id)
    {
    case SET_PSEUDO_PACKET_ID:
        readSetPseudoPacket(packet, &client_data->pseudo);
        printf("%d : Pseudo définie : %s\n", server_client->socket_fd, client_data->pseudo);
        break;
    case SET_MAP_PACKET_ID:
        printf("%d : %s a envoyer les données la de carte\n", server_client->socket_fd, client_data->pseudo);
        break;
    case IS_PLAYER_READY_PACKET_ID:
        readIsPlayerReadyPacket(packet, &client_data->is_player_ready);
        printf("%d : %s %sest prêt à jouer\n", server_client->socket_fd, client_data->pseudo, client_data->is_player_ready ? "" : "n'");
        break;
    case GAME_FINISHED_PACKET_ID:
        // temporaire
        float destruction_percentage;
        long time_left;
        readGameFinishedPacket(packet, &destruction_percentage, &time_left);
        printf("%d : %s %f %ld\n", server_client->socket_fd, client_data->pseudo, destruction_percentage, time_left);

        break;
    }
}

int main(int argc, char *argv[])
{
    char *hostname;
    uint16_t port;

    get_connection_info(argc, argv, &hostname, &port);

    printf("Ecoute sur %s:%d\n", hostname, port);

    signal(SIGINT, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGTERM, signalHandler);

    initSocket();

    server_t *server = createServer(hostname, port);

    delete_server_client_data = (void *)deleteClientData;

    if (server == NULL)
    {
        endSocket();
        free(hostname);

        return 2;
    }

    frame_timer_t *main_timer = createTimer(1000 / 60);

    // boucle principale
    while (running)
    {
        if (checkTime(main_timer))
        {
            acceptClientConnections(server);

            while (nextClientConnection())
            {
                switch (server_client_state)
                {
                case SERVER_CLIENT_WAITING_HANDSHAKE:
                    if (waitClientHandshake(server))
                    {
                        *server_client_data = createClientData();

                        printf("%d : Nouveau client connecté avec succès\n", server_client->socket_fd);
                    }
                    break;
                case SERVER_CLIENT_CONNECTED:; // Pour éviter l'erreur de compilation avec les anciennes versions de gcc
                    packet_t *packet = recvFromServerClient(server_client);

                    if (packet != NULL)
                    {
                        handle_packet(packet);
                        deletePacket(&packet);
                    }
                    break;
                }
            }

            for (int i = 0; i < deleted_socket_fd_count; i++)
            {
                printf("%d : (Erreur): Déconnexion du client\n", deleted_socket_fds[i]);
            }

            sleepMs(timeLeft(main_timer));
        }
    }

    deleteTimer(&main_timer);
    closeClientConnections();
    deleteServer(&server);
    endSocket();
    free(hostname);

    return 0;
}