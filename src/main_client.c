#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "window.h"
#include "timer.h"
#include "client_connection.h"

int running = 1;

void signalHandler(int s)
{
    running = 0;
}

void windowEventHandler(SDL_Event *event)
{
    // gestion des évènements de la fenêtre
    switch (event->type)
    {
    case SDL_QUIT:
        running = 0;
        break;
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    signal(SIGINT, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGTERM, signalHandler);

    window_t *window = createWindow("Slap of Ages", 600, 600);

    if (window == NULL)
        return 1;

    initSocket();

    frame_timer_t *main_timer = createTimer(1000 / 60);

    char hostname[256] = {0};
    uint16_t port = 0;

    // boucle principale
    while (running)
    {
        SDL_Event event;
        int time_left = timeLeft(main_timer);

        if (SDL_WaitEventTimeout(&event, time_left > 0 ? time_left : 0))
            windowEventHandler(&event);

        if (checkTime(main_timer))
        {
            SDL_RenderClear(window->renderer);

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
                switch (waitServerHandshake())
                {
                case -1:
                    printf("(Erreur): Connexion impossible réessayer\n");
                    break;
                case 1:
                    printf("Connexion établie avec succès\n");
                    break;
                }
                break;
            case CLIENT_CONNECTED:
                if (isServerDown(client))
                {
                    printf("(Erreur): Déconnexion du serveur\n");
                    running = 0;
                    break;
                }

                packet_t *packet = recvFromServer(client);

                // code

                deletePacket(&packet);
                break;
            }

            SDL_RenderPresent(window->renderer);
        }
    }

    closeClientConnection();
    deleteTimer(&main_timer);
    endSocket();
    destroyWindow(&window);

    return 0;
}