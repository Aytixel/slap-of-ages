#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "window/window.h"
#include "timer/timer.h"
#include "connection/client.h"
#include "map/map_renderer.h"
#include "map/building_renderer.h"
#include "map/building.h"
#include "menu/menu.h"
#include "client/game_data.h"
#include "client/game_state.h"
#include "game/hud.h"

#define MAP_SIZE 24

int running = 1;

void signalHandler(int s)
{
    running = 0;
}

void windowEventHandler(
    SDL_Event *event,
    window_t *window,
    client_game_data_t *game_data,
    building_renderer_t *building_renderer,
    building_t ***map_building,
    menu_t *menu,
    hud_t *hud)
{
    // gestion des évènements de la fenêtre
    updateWindowSize(window, event);

    switch (event->type)
    {
    case SDL_QUIT:
        if (client_connection_state == CLIENT_CONNECTED)
            running = 0;
        break;
    case SDL_KEYDOWN:
        if (event->key.state == SDL_PRESSED)
        {
            switch (event->key.keysym.sym)
            {
            case SDLK_z:
                // temporaire
                endGame(client, game_data);
                break;
            }
        }
        break;
    }

    switch (client_connection_state)
    {
    case CLIENT_WAITING_INFO:
    case CLIENT_WAITING_HANDSHAKE:
        if (menuEventHandler(game_data, event, menu))
        {
            char title[90] = "";
            sprintf(title, "Slap of Ages : %s", game_data->pseudo);
            SDL_SetWindowTitle(window->window, title);

            if (!initClientConnection(game_data->hostname, game_data->port))
            {
                packet_t *set_pseudo_packet = createSetPseudoPacket(game_data->pseudo);

                sendToServer(client, set_pseudo_packet);
                deletePacket(&set_pseudo_packet);
            }
        }
        break;
    case CLIENT_CONNECTED:
        hudEventHandler(event, hud, client, map_building, game_data, MAP_SIZE);

        switch (game_data->state)
        {
        case PREPARATION_GAME_STATE:
            buildingEventHandler(event, game_data, map_building, building_renderer, window);
            break;
        default:
            break;
        }
        break;
    }
}

void handle_packet(packet_t *packet, window_t *window, building_t ***map_building, client_game_data_t *game_data)
{
    char title[150] = "";

    switch (packet->id)
    {
    case SET_PSEUDO_PACKET_ID:
        readSetPseudoPacket(packet, &game_data->opponent_pseudo);
        printf("Adversaire : %s\n", game_data->opponent_pseudo);

        sprintf(title, "Slap of Ages : %s vs %s", game_data->pseudo, game_data->opponent_pseudo);
        SDL_SetWindowTitle(window->window, title);
        break;
    case SET_MAP_PACKET_ID:
        readSetMapPacket(packet, window, map_building, game_data, MAP_SIZE);
        startGame(client, game_data);
        printf("Partie lancé\n");
        break;
    case HAS_PLAYER_WON_PACKET_ID:
        game_data->state = PREPARATION_GAME_STATE;

        int has_won;

        readHasPlayerWonPacket(packet, &has_won);
        deleteTimer(&game_data->timer);

        if (has_won == 1 || has_won == 2)
            game_data->win_count++;

        printf("Gagné : %d, Nombre de victoire : %d\n", has_won, game_data->win_count);

        sprintf(title, "Slap of Ages : %s", game_data->pseudo);
        SDL_SetWindowTitle(window->window, title);
        break;
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    signal(SIGINT, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGTERM, signalHandler);

    window_t *window = createWindow("Slap of Ages", 700, 600);

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
    building_t ***map_building = createBuildingMatrix(MAP_SIZE);
    client_game_data_t *game_data = createGameData();
    menu_t *menu = createMenu(window, game_data);
    hud_t *hud = createHud(window);

    if (menu == NULL)
        return 1;

    // boucle principale
    while (running)
    {
        SDL_Event event;
        int time_left = timeLeft(main_timer);

        if (SDL_WaitEventTimeout(&event, time_left > 0 ? time_left : 0))
            windowEventHandler(&event, window, game_data, building_renderer, map_building, menu, hud);

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

                checkClientGameTimeout(client, game_data);

                packet_t *packet = recvFromServer(client);

                if (packet != NULL)
                {
                    handle_packet(packet, window, map_building, game_data);
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
                renderBuildingMatrix(window, map_building, building_renderer, MAP_SIZE);
                renderHud(window, hud, map_renderer, game_data);
                break;
            default:
                running = menuRenderer(window, menu);
                break;
            }

            SDL_RenderPresent(window->renderer);
        }
    }

    closeClientConnection();
    deleteHud(&hud);
    deleteMenu(&menu);
    deleteGameData(&game_data);
    destroyBuildingMatrix(&map_building, MAP_SIZE);
    deleteTimer(&main_timer);
    endSocket();
    deleteBuildingRenderer(&building_renderer);
    deleteMapRenderer(&map_renderer);
    destroyWindow(&window);

    return 0;
}