#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "timer.h"
#include "server_connection.h"

int running = 1;

void signalHandler(int s)
{
    running = 0;
}

int get_connection_info(int argc, char *argv[], char **hostname, uint16_t *port)
{
    for (int i = 1; i < argc; i++)
    {
        if (*hostname == NULL && (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--hostname") == 0) && ++i < argc)
        {
            *hostname = malloc(sizeof(char) * (strlen(argv[i]) + 1));
            strcpy(*hostname, argv[i]);
        }
        else if (*port == 0 && (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) && ++i < argc)
            *port = atoi(argv[i]);
    }

    if (*hostname == NULL)
        return -1;
    if (*port == 0)
    {
        free(*hostname);

        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    char *hostname = NULL;
    uint16_t port = 0;

    if (get_connection_info(argc, argv, &hostname, &port) == -1)
    {
        printf("Erreur : Nom d'hôte ou port non spécifié\n");

        return 1;
    }

    printf("Ecoute sur %s:%d\n", hostname, port);

    signal(SIGINT, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGTERM, signalHandler);

    initSocket();

    server_t *server = createServer(hostname, port);

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
                switch (server_client_connection_state)
                {
                case SERVER_CLIENT_WAITING_HANDSHAKE:
                    if (waitClientHandshake(server))
                        printf("Nouveau client connecté avec succès : %d\n", server_client->socket_fd);
                    break;
                case SERVER_CLIENT_CONNECTED:; //Pour éviter l'erreur de compilation avec les anciennes versions de gcc
                    packet_t *packet = recvFromServerClient(server_client);

                    // code

                    deletePacket(&packet);
                    break;
                }
            }

            for (int i = 0; i < deleted_socket_fd_count; i++)
            {
                printf("(Erreur): Déconnexion du client : %d\n", deleted_socket_fds[i]);
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