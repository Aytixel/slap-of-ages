#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "timer.h"
#include "client_socket.h"
#include "client_connection.h"

int running = 1;

void handler(int s)
{
    running = 0;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    signal(SIGINT, handler);
    signal(SIGABRT, handler);
    signal(SIGTERM, handler);

    initSocket();

    client_t *client = NULL;
    frame_timer_t *main_timer = createTimer(1000 / 60);

    char hostname[256] = {0};
    uint16_t port = 0;

    // boucle principale
    while (running)
    {
        if (checkTime(main_timer))
        {
            // gestion de quelle chose faire en fonction de l'état de la connexion
            switch (client_connection_state)
            {
            case CLIENT_WAITING_INFO:
                printf("Nom d'hôte du serveur : ");
                scanf("%s", hostname);
                printf("Port du serveur : ");
                scanf("%hd", &port);
                initClientConnection(hostname, port);
                break;
            case CLIENT_WAITING_HANDSHAKE:
                if (waitServerHandshake() == -1)
                    printf("Connexion impossible réessayer.\n");
                break;
            case CLIENT_CONNECTED:
                printf("Client\n");
                break;
            case CLIENT_CLOSED:
                running = 0;
                break;
            }

            sleepMs(timeLeft(main_timer));
        }
    }

    closeClientConnection();
    deleteTimer(&main_timer);
    deleteClient(&client);
    endSocket();

    return 0;
}