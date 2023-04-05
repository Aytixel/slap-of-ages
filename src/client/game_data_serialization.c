/**
 * @file game_data_serialization.c
 * @author Lucas Dureau
 * @brief Fonction pour transformé les données du jeu, en données pouvant être stocker ou envoyer
 * @version 0.1
 * @date 01/04/2023
 *
 */

#include <stdio.h>
#include "common.h"
#include "game_data_serialization.h"

extern int serializeGameData(client_game_data_t *game_data)
{
    FILE *file = fopen(SAVE_FILENAME, "wb");

    if (file == NULL)
        return -1;

    if (fwrite(game_data->hostname, sizeof(char), HOSTNAME_SIZE, file) != HOSTNAME_SIZE)
    {
        fclose(file);
        return -1;
    }
    if (fwrite(&game_data->port, sizeof(uint16_t), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }
    if (fwrite(game_data->pseudo, sizeof(char), PSEUDO_SIZE, file) != PSEUDO_SIZE)
    {
        fclose(file);
        return -1;
    }
    if (fwrite(&game_data->gold_count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }
    if (fwrite(&game_data->elixir_count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }
    if (fwrite(&game_data->win_count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }

    void *map_data = NULL;
    int map_data_length = serializeMapData(&map_data, game_data->map_building, game_data->gold_cost);

    if (fwrite(map_data, sizeof(char), map_data_length, file) != map_data_length)
    {
        if (map_data != NULL)
            free(map_data);

        fclose(file);
        return -1;
    }

    free(map_data);
    fclose(file);

    return 0;
}

extern int deserializeGameData(window_t *window, client_game_data_t *game_data)
{
    FILE *file = fopen(SAVE_FILENAME, "rb");

    if (file == NULL)
        return -1;

    char hostname[HOSTNAME_SIZE];
    uint16_t port;
    char pseudo[PSEUDO_SIZE];
    int gold_count;
    int elixir_count;
    int win_count;

    if (fread(hostname, sizeof(char), HOSTNAME_SIZE, file) != HOSTNAME_SIZE)
    {
        fclose(file);
        return -1;
    }
    if (fread(&port, sizeof(uint16_t), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }
    if (fread(pseudo, sizeof(char), PSEUDO_SIZE, file) != PSEUDO_SIZE)
    {
        fclose(file);
        return -1;
    }
    if (fread(&gold_count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }
    if (fread(&elixir_count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }
    if (fread(&win_count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }

    int map_data_length = sizeof(int) + sizeof(char) * MAP_SIZE * MAP_SIZE;
    void *map_data = malloc(map_data_length);

    if (fread(map_data, sizeof(char), map_data_length, file) != map_data_length)
    {
        free(map_data);
        fclose(file);
        return -1;
    }

    if (deserializeMapData(map_data, map_data_length, window, game_data->map_building, &game_data->gold_cost) == -1)
    {
        free(map_data);
        fclose(file);
        return -1;
    }

    free(map_data);
    fclose(file);

    strcpy(game_data->hostname, hostname);
    game_data->port = port;
    strcpy(game_data->pseudo, pseudo);
    game_data->gold_count = gold_count;
    game_data->elixir_count = elixir_count;
    game_data->win_count = win_count;

    return 0;
}