/**
 * @file character.c
 * @author Dureau Arthur, Lucas Dureau
 * @brief Permet de gérer les personnages
 * @version 1.0
 * @date 29/03/2023
 *
 */

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
#include "window/window.h"
#include "window/input.h"

extern character_t *createCharacter(character_renderer_t *character_renderer, character_type_e type, SDL_Point *position)
{
    character_t *character = malloc(sizeof(character_t));

    character->type = type;
    character->position = *position;

    switch (type)
    {
    case GIANT_CHARACTER:
        character->hp = 350;
        character->attack = 30;
        character->speed = 1;
        character->animation = createAnimation(
            character_renderer->animations.giant.tile_size,
            character_renderer->animations.giant.state_frame_count,
            character_renderer->animations.giant.sprite,
            character_renderer->animations.giant.fps);
        break;
    case DAEMON_CHARACTER:
        character->hp = 90;
        character->attack = 45;
        character->speed = 2;
        character->animation = createAnimation(
            character_renderer->animations.deamon.tile_size,
            character_renderer->animations.deamon.state_frame_count,
            character_renderer->animations.deamon.sprite,
            character_renderer->animations.deamon.fps);
        break;
    case RAT_CHARACTER:
        character->hp = 50;
        character->attack = 60;
        character->speed = 3;
        character->animation = createAnimation(
            character_renderer->animations.rat.tile_size,
            character_renderer->animations.rat.state_frame_count,
            character_renderer->animations.rat.sprite,
            character_renderer->animations.rat.fps);
        break;
    }

    return character;
}

extern character_t ***createCharacterMatrix()
{
    character_t ***character_matrix = malloc(sizeof(character_t **) * (MAP_SIZE + CHARACTER_PLACEMENT_MARGIN * 2));

    character_matrix += 2;

    for (int i = -CHARACTER_PLACEMENT_MARGIN; i < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; i++)
    {
        character_matrix[i] = malloc(sizeof(character_t *) * (MAP_SIZE + CHARACTER_PLACEMENT_MARGIN * 2));
        character_matrix[i] += 2;

        for (int j = -CHARACTER_PLACEMENT_MARGIN; j < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; j++)
        {
            character_matrix[i][j] = NULL;
        }
    }

    return character_matrix;
}

extern void renderCharacterMatrix(window_t *window, character_t ***map_character, character_renderer_t *character_renderer)
{
    for (int i = -CHARACTER_PLACEMENT_MARGIN; i < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; i++)
    {
        for (int j = -CHARACTER_PLACEMENT_MARGIN; j < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; j++)
        {
            if (map_character[i][j] != NULL)
                renderCharacter(window, character_renderer, map_character[i][j]);
        }
    }
}

extern void destroyCharacter(character_t **character)
{
    if (*character == NULL || character == NULL)
        return;

    destroyAnimationWithoutSprite(&(*character)->animation);

    free(*character);
    *character = NULL;
}

extern void clearCharacterMatrix(character_t ***character_matrix)
{
    for (int i = -CHARACTER_PLACEMENT_MARGIN; i < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; i++)
    {
        for (int j = -CHARACTER_PLACEMENT_MARGIN; j < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; j++)
        {
            if (character_matrix[i][j] != NULL)
            {
                destroyCharacter(&(character_matrix[i][j]));
            }

            character_t *character = getCharacter(character_matrix, i, j);

            if (character != NULL)
                removeCharacterFromMatrix(character_matrix, character);
        }
    }
}

extern void destroyCharacterMatrix(character_t ****character_matrix)
{
    clearCharacterMatrix(*character_matrix);

    for (int i = -CHARACTER_PLACEMENT_MARGIN; i < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; i++)
    {
        free((*character_matrix)[i] - CHARACTER_PLACEMENT_MARGIN);
    }

    free(*character_matrix - CHARACTER_PLACEMENT_MARGIN);
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

extern int canPlaceCharacter(character_renderer_t *character_renderer, SDL_Point *position, character_t ***character_matrix)
{

    if (canRenderCharacter(character_renderer, position))
    {
        if (getCharacterWithPoint(character_matrix, position) == NULL)
            return 1;
    }
    return 0;
}

extern character_t *getCharacter(character_t ***character_matrix, int x, int y)
{
    if (x < 0 || y < 0 || x >= MAP_SIZE || y >= MAP_SIZE)
        return NULL;

    return character_matrix[x][y];
}

extern character_t *getCharacterWithPoint(character_t ***character_matrix, SDL_Point *position)
{
    return getCharacter(character_matrix, position->x, position->y);
}

extern void characterEventHandler(SDL_Event *event, client_game_data_t *game_data, character_renderer_t *character_renderer, window_t *window)
{
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        SDL_Point mouse_position = {event->button.x, event->button.y};
        SDL_Point tile_position = getTileCoord(&mouse_position, window, character_renderer->map_renderer);
        character_t *character = NULL;

        if (canPlaceCharacter(character_renderer, &tile_position, game_data->character_matrix))
            addCharacterInMatrix(game_data->character_matrix, createCharacter(character_renderer, DAEMON_CHARACTER, &tile_position));
        else if ((character = getCharacterWithPoint(game_data->character_matrix, &tile_position)) != NULL)
            removeCharacterFromMatrix(game_data->character_matrix, character);
    }
}
