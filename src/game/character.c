#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "client/common.h"
#include "timer/timer.h"
#include "window/animation.h"
#include "window/animation_states.h"
#include "character.h"
//#include "character_renderer.h"
#include "window/window.h"
#include "window/input.h"

#define TILE_SIZE 8

extern character_t *createCharacter(character_type_e type, SDL_Point *position)
{
    character_t *character = malloc(sizeof(character_t));

    character->type = type;
    character->position = *position;

    switch (type)
    {
    case DAEMON_CHARACTER:
        character->hp = 100;
        character->attack = 0;
        character->speed = 1;
        character->gold_cost = 50;
        break;
    // Ajoutez d'autres types de personnages ici
    default:
        character->hp = 0;
        character->attack = 0;
        character->speed = 0;
        character->gold_cost = 0;
        break;
    }

    return character;
}


extern character_t ***createCharacterMatrix()
{
    character_t ***character_matrix = malloc(sizeof(character_t **) * MAP_SIZE);

    for (int i = 0; i < MAP_SIZE; i++)
    {
        character_matrix[i] = malloc(sizeof(character_t *) * MAP_SIZE);
    }

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            character_matrix[i][j] = NULL;
        }
    }

    return character_matrix;
}

extern void renderCharacterMatrix(window_t *window, character_t ***map_character, character_renderer_t *character_renderer)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (map_character[i][j] != NULL)
            {
                SDL_Rect destination_rect;
                renderCharacter(window, (character_renderer_t *)character_renderer, map_character[i][j], &destination_rect);
            }
        }
    }
}


extern void destroyCharacter(character_t **character)
{
    if (*character == NULL || character == NULL)
        return;
    free(*character);
    *character = NULL;
}

extern void clearCharacterMatrix(character_t ***character_matrix)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (character_matrix[i][j] != NULL)
            {
                destroyCharacter(&(character_matrix[i][j]));
            }
        }
    }
}

extern void destroyCharacterMatrix(character_t ****character_matrix)
{
    clearCharacterMatrix(*character_matrix);

    for (int i = 0; i < MAP_SIZE; i++)
    {
        free((*character_matrix)[i]);
    }

    free(*character_matrix);
    *character_matrix = NULL;
}

extern void addCharacterInMatrix(character_t ***character_matrix, character_t *character)
{
    character_matrix[character->position.x][character->position.y] = character;
}

extern void removeCharacterFromMatrix(character_t ***character_matrix, character_t *character)
{
    if (character == NULL)
        return;

    destroyCharacter(&(character_matrix[character->position.x][character->position.y]));
}

extern int canPlaceCharacter(character_renderer_t *character_renderer, character_t *character, SDL_Point *position, character_t ***character_matrix)
{
    if (character_matrix[position->x][position->y] == NULL)
    {
        return 1;
    }
    return 0;
}

extern character_t *getCharacter(character_t ***character_matrix, SDL_Point *position)
{
    return character_matrix[position->x][position->y];
}

extern void characterEventHandler(SDL_Event *event, client_game_data_t *game_data, character_t ***map_character, character_renderer_t *character_renderer, window_t *window)
{
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        SDL_Point mouse_position = {event->button.x, event->button.y};
        SDL_Point tile_position = getTileCoord(&mouse_position, window, character_renderer->map_renderer);
        character_t *new = createCharacter(DAEMON_CHARACTER, &tile_position);

        if (canPlaceCharacter(character_renderer, new, &tile_position, map_character))
        {
            if (game_data->gold_count - new->gold_cost >= 0)
            {
                addCharacterInMatrix(map_character, new);
                game_data->gold_count -= new->gold_cost;
                game_data->gold_cost += new->gold_cost;
            }
            else
                destroyCharacter(&new);
        }
        else if (tile_position.x != -1 && tile_position.y != -1 && map_character[tile_position.x][tile_position.y] != NULL)
        {
            game_data->gold_count += map_character[tile_position.x][tile_position.y]->gold_cost;
            game_data->gold_cost -= map_character[tile_position.x][tile_position.y]->gold_cost;
            destroyCharacter(&new);
            removeCharacterFromMatrix(map_character, map_character[tile_position.x][tile_position.y]);
        }
    }
}
