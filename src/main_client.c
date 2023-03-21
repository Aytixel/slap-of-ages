#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "window/window.h"
#include "timer/timer.h"
#include "connection/client.h"
#include "map/map_renderer.h"
#include "map/building_renderer.h"
#include "menu/menu.h"
#include "client/game_data.h"
#include "client/game_state.h"

#define MAP_SIZE 31

int running = 1;

void signalHandler(int s)
{
    running = 0;
}

void windowEventHandler(SDL_Event *event, window_t *window, client_game_data_t *game_data)
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
            switch (event->key.keysym.sym)
            {
            case SDLK_a:
                // temporaire
                toggleMatchmaking(client, game_data);
                break;
            case SDLK_z:
                // temporaire
                endGame(client, game_data);
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

void handle_packet(packet_t *packet, client_game_data_t *game_data)
{
    switch (packet->id)
    {
    case SET_PSEUDO_PACKET_ID:
        readSetPseudoPacket(packet, &game_data->opponent_pseudo);
        printf("Adversaire : %s\n", game_data->opponent_pseudo);
        break;
    case SET_MAP_PACKET_ID:
        game_data->state = COMBAT_GAME_STATE;

        printf("Partie lancé\n");
        break;
    case HAS_PLAYER_WON_PACKET_ID:
        game_data->state = PREPARATION_GAME_STATE;

        int has_won;

        readHasPlayerWonPacket(packet, &has_won);

        if (has_won == 1 || has_won == 2)
            game_data->victory_count++;

        printf("Gagné : %d, Nombre de victoire : %d\n", has_won, game_data->victory_count);
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
    client_game_data_t *game_data = createGameData();

    connection_t connection;

    connection.port = 4539;
    strcpy(connection.pseudo, "localhost");
    strcpy(connection.ip, "");

    menu_t *menu = createMenu(window);

    if (menu == NULL)
        return 1;

    // boucle principale
    while (running)
    {
        SDL_Event event;
        int time_left = timeLeft(main_timer);

        if (SDL_WaitEventTimeout(&event, time_left > 0 ? time_left : 0))
        {
            windowEventHandler(&event, window, game_data);

            switch (menuEventHandler(&connection, &event, menu))
            {
            case 2:
                running = 0;
                break;
            case 1:
                if (!initClientConnection(connection.ip, connection.port))
                {
                    packet_t *set_pseudo_packet = createSetPseudoPacket(connection.pseudo);

                    sendToServer(client, set_pseudo_packet);
                    deletePacket(&set_pseudo_packet);
                }
                break;
            }
        }

        if (checkTime(main_timer))
        {
            // gestion de quelle chose faire en fonction de l'état de la connexion
            switch (client_connection_state)
            {
            case CLIENT_WAITING_INFO:
                break;
            case CLIENT_WAITING_HANDSHAKE:
                switch (waitServerHandshake())
                {
                case -1:
                    printf("(Erreur): Connexion impossible réessayer\n");
                    break;
                case 1:
                    game_data->state = PREPARATION_GAME_STATE;

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
                    handle_packet(packet, game_data);
                    deletePacket(&packet);
                }
                break;
            }

            SDL_RenderClear(window->renderer);

            // gestion de l'affichage
            switch (client_connection_state)
            {
            case CLIENT_CONNECTED:
                renderMap(window, map_renderer);
                break;
            default:
                menuRenderer(window, menu);
                break;
            }

            SDL_RenderPresent(window->renderer);
        }
    }

    closeClientConnection();
    deleteMenu(&menu);
    deleteGameData(&game_data);
    deleteTimer(&main_timer);
    endSocket();
    deleteBuildingRenderer(&building_renderer);
    deleteMapRenderer(&map_renderer);
    destroyWindow(&window);

    return 0;
}