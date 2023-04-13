/**
 * @file main_client.c
 * @author Lucas Dureau
 * @brief Main du client
 * @version 0.1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "window/window.h"
#include "timer/timer.h"
#include "connection/client.h"
#include "map/map_renderer.h"
#include "building/building_renderer.h"
#include "building/building.h"
#include "building/building_hud.h"
#include "menu/menu.h"
#include "client/game_data.h"
#include "client/game_state.h"
#include "client/game_data_serialization.h"
#include "game/hud.h"
#include "character/character_hud.h"

int running = 1;

void signalHandler(int s)
{
    running = 0;
}

void windowEventHandler(
    SDL_Event *event,
    window_t *window,
    client_game_data_t *game_data,
    building_hud_t *building_hud,
    character_hud_t *character_hud,
    building_renderer_t *building_renderer,
    character_renderer_t *character_renderer,
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

            serializeGameData(game_data);

            if (!initClientConnection(game_data->hostname, game_data->port))
            {
                packet_t *set_pseudo_packet = createSetPseudoPacket(game_data->pseudo);

                sendToServer(client, set_pseudo_packet);
                deletePacket(&set_pseudo_packet);
            }
        }
        break;
    case CLIENT_CONNECTED:
        hudEventHandler(event, hud, client, game_data);

        switch (game_data->state)
        {
        case PREPARATION_GAME_STATE:
            buildingEventHandler(event, game_data, building_renderer, window);
            buildingHudEventHandler(event, building_hud, game_data);
            break;
        case COMBAT_GAME_STATE:
            characterEventHandler(event, game_data, character_renderer, window);
            characterHudEventHandler(event, character_hud, game_data);
            break;
        default:
            break;
        }
        break;
    }
}

void handle_packet(
    packet_t *packet,
    window_t *window,
    client_game_data_t *game_data,
    character_renderer_t *character_renderer,
    Mix_Music *combat_music,
    Mix_Music *field_music)
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
        readSetMapPacket(packet, window, game_data);
        startGame(client, game_data);
        addDefenceCharacter(character_renderer, game_data);
        Mix_FadeOutMusic(250);
        Mix_FadeInMusic(combat_music, 0, 250);
        Mix_VolumeMusic(32);
        printf("Partie lancé\n");
        break;
    case HAS_PLAYER_WON_PACKET_ID:
        game_data->state = PREPARATION_GAME_STATE;

        int has_won;

        readHasPlayerWonPacket(packet, &has_won);
        deleteTimer(&game_data->timer);

        switch (has_won)
        {
        case 2:
            game_data->gold_count += GOLD_GAIN;
        case 1:
            game_data->elixir_count += ELIXIR_GAIN;
            game_data->win_count++;
            break;
        default:
            game_data->gold_count += GOLD_GAIN;
            break;
        }

        serializeGameData(game_data);

        game_data->elixir_cost = 0;
        clearCharacterList(game_data->character_list);
        Mix_FadeOutMusic(250);
        Mix_FadeInMusic(field_music, 1, 250);
        Mix_VolumeMusic(32);

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

    Mix_Music *combat_music = loadMusic("./asset/audio/combat.mp3");

    if (combat_music == NULL)
    {
        destroyWindow(&window);

        return 1;
    }

    Mix_Music *field_music = loadMusic("./asset/audio/field.mp3");

    if (field_music == NULL)
    {
        Mix_FreeMusic(field_music);
        destroyWindow(&window);

        return 1;
    }

    map_renderer_t *map_renderer = createMapRenderer(window);

    if (map_renderer == NULL)
    {
        Mix_FreeMusic(field_music);
        Mix_FreeMusic(combat_music);
        destroyWindow(&window);
        return 1;
    }

    building_renderer_t *building_renderer = createBuildingRenderer(window, map_renderer);

    if (building_renderer == NULL)
    {
        Mix_FreeMusic(field_music);
        Mix_FreeMusic(combat_music);
        destroyWindow(&window);
        deleteMapRenderer(&map_renderer);
        return 1;
    }

    character_renderer_t *character_renderer = createCharacterRenderer(window, map_renderer);

    if (character_renderer == NULL)
    {
        Mix_FreeMusic(field_music);
        Mix_FreeMusic(combat_music);
        destroyWindow(&window);
        deleteMapRenderer(&map_renderer);
        deleteBuildingRenderer(&building_renderer);
        return 1;
    }

    initSocket();

    frame_timer_t *main_timer = createTimer(1000 / 30);
    client_game_data_t *game_data = createGameData();

    deserializeGameData(window, game_data);

    menu_t *menu = createMenu(window, game_data);

    if (menu == NULL)
    {
        Mix_FreeMusic(field_music);
        Mix_FreeMusic(combat_music);
        destroyWindow(&window);
        deleteMapRenderer(&map_renderer);
        deleteBuildingRenderer(&building_renderer);
        deleteCharacterRenderer(&character_renderer);
        return 1;
    }

    hud_t *hud = createHud(window);

    if (hud == NULL)
    {
        Mix_FreeMusic(field_music);
        Mix_FreeMusic(combat_music);
        destroyWindow(&window);
        deleteMapRenderer(&map_renderer);
        deleteBuildingRenderer(&building_renderer);
        deleteCharacterRenderer(&character_renderer);
        deleteMenu(&menu);
        return 1;
    }

    building_hud_t *building_hud = createBuildingHud(window);

    if (building_hud == NULL)
    {
        Mix_FreeMusic(field_music);
        Mix_FreeMusic(combat_music);
        destroyWindow(&window);
        deleteMapRenderer(&map_renderer);
        deleteBuildingRenderer(&building_renderer);
        deleteCharacterRenderer(&character_renderer);
        deleteMenu(&menu);
        deleteHud(&hud);
        return 1;
    }

    character_hud_t *character_hud = createCharacterHud(window, character_renderer);

    if (building_hud == NULL)
    {
        Mix_FreeMusic(field_music);
        Mix_FreeMusic(combat_music);
        destroyWindow(&window);
        deleteMapRenderer(&map_renderer);
        deleteBuildingRenderer(&building_renderer);
        deleteCharacterRenderer(&character_renderer);
        deleteMenu(&menu);
        deleteHud(&hud);
        deleteBuildingHud(&building_hud);
        return 1;
    }

    // boucle principale
    while (running)
    {
        SDL_Event event;
        int time_left = timeLeft(main_timer);

        if (SDL_WaitEventTimeout(&event, time_left > 0 ? time_left : 0))
            windowEventHandler(&event, window, game_data, building_hud, character_hud, building_renderer, character_renderer, menu, hud);

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

                    Mix_FadeInMusic(field_music, 1, 250);
                    Mix_VolumeMusic(32);

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

                if (game_data->state == COMBAT_GAME_STATE)
                {
                    checkClientGameTimeout(client, game_data);
                    updateCharacter(game_data);

                    if (game_data->opponent_gold_cost <= 0 || (game_data->elixir_cost >= game_data->elixir_count || game_data->character_list->count == 0))
                        endGame(client, game_data);
                }

                packet_t *packet = recvFromServer(client);

                if (packet != NULL)
                {
                    handle_packet(packet, window, game_data, character_renderer, combat_music, field_music);
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

                switch (game_data->state)
                {
                case PREPARATION_GAME_STATE:
                    renderBuildingHud(window, building_hud, building_renderer);
                case MATCHMAKING_GAME_STATE:
                    renderBuildingMatrix(window, game_data->map_building, building_renderer);
                    break;
                case COMBAT_GAME_STATE:
                    renderBuildingMatrix(window, game_data->opponent_map_building, building_renderer);
                    renderCharacterList(window, game_data->character_list, character_renderer);
                    renderCharacterHud(window, character_hud);
                    break;
                case WAITING_RESULT_GAME_STATE:
                    renderBuildingMatrix(window, game_data->opponent_map_building, building_renderer);
                    break;
                default:
                    break;
                }

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
    deleteCharacterHud(&character_hud);
    deleteBuildingHud(&building_hud);
    deleteHud(&hud);
    deleteMenu(&menu);
    deleteGameData(&game_data);
    deleteTimer(&main_timer);
    endSocket();
    deleteCharacterRenderer(&character_renderer);
    deleteBuildingRenderer(&building_renderer);
    deleteMapRenderer(&map_renderer);
    Mix_FreeMusic(field_music);
    Mix_FreeMusic(combat_music);
    destroyWindow(&window);

    return 0;
}