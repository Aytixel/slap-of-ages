#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "window/window.h"
#include "timer/timer.h"
#include "connection/client.h"
#include "map/map_renderer.h"
#include "map/building_renderer.h"

#define MAP_SIZE 31

int running = 1;

void signalHandler(int s)
{
    running = 0;
}

void windowEventHandler(SDL_Event *event, window_t *window)
{
    // gestion des évènements de la fenêtre
    switch (event->type)
    {
    case SDL_QUIT:
        running = 0;
        break;
    case SDL_KEYDOWN:
        if (event->key.state == SDL_PRESSED)
        {
            packet_t *packet = NULL;

            switch (event->key.keysym.sym)
            {
            case SDLK_a:
                // temporaire
                packet = createSetMapPacket();

                sendToServer(client, packet);
                deletePacket(&packet);

                packet = createIsPlayerReadyPacket(1);

                sendToServer(client, packet);
                deletePacket(&packet);
                break;
            case SDLK_z:
                // temporaire
                packet = createIsPlayerReadyPacket(0);

                sendToServer(client, packet);
                deletePacket(&packet);
                break;
            case SDLK_e:
                // temporaire
                packet = createGameFinishedPacket(79, 248934);

                sendToServer(client, packet);
                deletePacket(&packet);
                break;
            }
        }
        break;
    case SDL_WINDOWEVENT:
        switch (event->window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
            window->width = event->window.data1;
            window->height = event->window.data2;
            break;
        }
        break;
    }
}

void handle_packet(packet_t *packet)
{
    switch (packet->id)
    {
    case SET_PSEUDO_PACKET_ID:
        char *pseudo;

        readSetPseudoPacket(packet, &pseudo);
        printf("Adversaire : %s\n", pseudo);
        break;
    case SET_MAP_PACKET_ID:
        printf("Partie lancé\n");
        break;
    case HAS_PLAYER_WON_PACKET_ID:
        int has_won;

        readHasPlayerWonPacket(packet, &has_won);
        printf("Gagné: %d\n", has_won);
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

    map_renderer_t *map_renderer = createMapRenderer(window, MAP_SIZE);

    if (map_renderer == NULL)
        return 1;

    building_renderer_t *building_renderer = createBuildingRenderer(window, map_renderer);

    if (building_renderer == NULL)
        return 1;

    initSocket();

    frame_timer_t *main_timer = createTimer(1000 / 30);

    char hostname[] = "localhost";
    uint16_t port = 4539;
    char pseudo[64] = {0};

    // boucle principale
    while (running)
    {
        SDL_Event event;
        int time_left = timeLeft(main_timer);

        if (SDL_WaitEventTimeout(&event, time_left > 0 ? time_left : 0))
            windowEventHandler(&event, window);

        if (checkTime(main_timer))
        {
            // gestion de quelle chose faire en fonction de l'état de la connexion
            switch (client_connection_state)
            {
            case CLIENT_WAITING_INFO:
                printf("Pseudo : ");
                scanf("%s", pseudo);

                if (!initClientConnection(hostname, port))
                {
                    packet_t *set_pseudo_packet = createSetPseudoPacket(pseudo);

                    sendToServer(client, set_pseudo_packet);
                    deletePacket(&set_pseudo_packet);
                }
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

                if (packet != NULL)
                {
                    handle_packet(packet);
                    deletePacket(&packet);
                }
                break;
            }

            SDL_RenderClear(window->renderer);

            renderMap(window, map_renderer);

            SDL_RenderPresent(window->renderer);
        }
    }

    closeClientConnection();
    deleteTimer(&main_timer);
    endSocket();
    deleteBuildingRenderer(&building_renderer);
    deleteMapRenderer(&map_renderer);
    destroyWindow(&window);

    return 0;
}