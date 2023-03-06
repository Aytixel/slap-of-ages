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

    char hostname[256] = {0};
    uint16_t port = 0;
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
                printf("Nom d'hôte du serveur : ");
                scanf("%s", hostname);
                printf("Port du serveur : ");
                scanf("%hd", &port);
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
            renderBuilding(window, building_renderer, 0, 0, HOUSE_1_BUILDING);
            renderBuilding(window, building_renderer, 1, 0, HOUSE_2_BUILDING);
            renderBuilding(window, building_renderer, 2, 0, HOUSE_3_BUILDING);
            renderBuilding(window, building_renderer, 3, 0, CORNER_WALL_BUILDING);
            renderBuilding(window, building_renderer, 4, 0, MINE_BUILDING);
            renderBuilding(window, building_renderer, 0, 1, MILL_BUILDING);
            renderBuilding(window, building_renderer, 1, 1, VERTICAL_WALL_BUILDING);
            renderBuilding(window, building_renderer, 2, 1, SPACE_FILLER_WALL_BUILDING);
            renderBuilding(window, building_renderer, 3, 1, HORIZONTAL_WALL_BUILDING);
            renderBuilding(window, building_renderer, 3, 2, WELL_BUILDING);
            renderBuilding(window, building_renderer, 4, 2, FIELD_BUILDING);

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