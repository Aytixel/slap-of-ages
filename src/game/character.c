/**
 * @file character.c
 * @author Dureau Arthur, Lucas Dureau
 * @brief Permet de gérer les troupes
 * @version 1.0
 * @date 29/03/2023
 *
 */

#include <stdlib.h>
#include "client/common.h"
#include "timer/timer.h"
#include "window/animation.h"
#include "window/animation_states.h"
#include "character.h"
#include "window/window.h"
#include "window/input.h"

extern character_t *createCharacter(character_renderer_t *character_renderer, character_type_e type, SDL_Point *position, int is_defender)
{
    character_t *character = malloc(sizeof(character_t));

    character->type = type;
    character->position = *position;
    character->is_defender = is_defender;

    float health_multiplier = is_defender ? 1.7 : 1;
    float attack_multiplier = is_defender ? 1.4 : 1;

    switch (type)
    {
    case GIANT_CHARACTER:
        character->hp = 350.0 * health_multiplier;
        character->attack = 30.0 * attack_multiplier;
        character->speed = 1;
        character->animation = createAnimation(
            character_renderer->animations.giant.tile_size,
            character_renderer->animations.giant.state_frame_count,
            character_renderer->animations.giant.sprite,
            character_renderer->animations.giant.fps);
        break;
    case DAEMON_CHARACTER:
        character->hp = 90.0 * health_multiplier;
        character->attack = 45.0 * attack_multiplier;
        character->speed = 2;
        character->animation = createAnimation(
            character_renderer->animations.deamon.tile_size,
            character_renderer->animations.deamon.state_frame_count,
            character_renderer->animations.deamon.sprite,
            character_renderer->animations.deamon.fps);
        break;
    case RAT_CHARACTER:
        character->hp = 35.0 * health_multiplier;
        character->attack = 50.0 * attack_multiplier;
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

extern int getCharacterElixirCost(character_type_e type)
{
    switch (type)
    {
    case GIANT_CHARACTER:
        return 30;
    case DAEMON_CHARACTER:
        return 20;
    case RAT_CHARACTER:
        return 10;
    default:
        return 0;
    }
}

extern character_list_t *createCharacterList()
{
    character_list_t *character_list = malloc(sizeof(character_list_t));

    character_list->count = 0;
    character_list->capacity = 1;
    character_list->list = malloc(sizeof(character_t *) * character_list->capacity);

    return character_list;
}

extern void renderCharacterList(window_t *window, character_list_t *character_list, character_renderer_t *character_renderer)
{
    for (int i = 0; i < character_list->count; i++)
    {
        renderCharacter(window, character_renderer, character_list->list[i]);
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

extern void clearCharacterList(character_list_t *character_list)
{
    for (int i = 0; i < character_list->count; i++)
    {
        destroyCharacter(&character_list->list[i]);
    }

    character_list->count = 0;
}

extern void destroyCharacterList(character_list_t **character_list)
{
    clearCharacterList(*character_list);

    free((*character_list)->list);
    free(*character_list);
    *character_list = NULL;
}

extern void addCharacterInList(character_list_t *character_list, character_t *character)
{
    character_list->list[character_list->count++] = character;

    if (character_list->count == character_list->capacity)
    {
        character_list->capacity *= 2;
        character_list->list = realloc(character_list->list, sizeof(character_t *) * character_list->capacity);
    }
}

extern void removeCharacterFromList(character_list_t *character_list, character_t *character)
{
    if (character == NULL)
        return;

    for (int i = 0; i < character_list->count; i++)
    {
        if (character_list->list[i] == character)
        {
            destroyCharacter(&character_list->list[i]);
            character_list->list[i] = character_list->list[--character_list->count];
        }
    }
}

extern int canPlaceCharacter(character_renderer_t *character_renderer, SDL_Point *position, building_t ***building_matrix, character_list_t *character_list)
{
    if (canRenderCharacter(character_renderer, position) &&
        getBuildingWithPoint(building_matrix, position) == NULL)
        return 1;

    return 0;
}

extern character_t *getCharacter(character_list_t *character_list, int x, int y)
{
    if (x < 0 || y < 0 || x >= MAP_SIZE || y >= MAP_SIZE)
        return NULL;

    for (int i = 0; i < character_list->count; i++)
    {
        if (character_list->list[i]->position.x == x && character_list->list[i]->position.y == y)
            return character_list->list[i];
    }

    return NULL;
}

extern character_t *getCharacterWithPoint(character_list_t *character_list, SDL_Point *position)
{
    return getCharacter(character_list, position->x, position->y);
}

extern void addDefenceCharacter(character_renderer_t *character_renderer, client_game_data_t *game_data)
{
    for (int x = 0; x < MAP_SIZE; x++)
    {
        for (int y = 0; y < MAP_SIZE; y++)
        {
            SDL_Point position = {x, y};
            building_t *building = getBuildingWithPoint(game_data->opponent_map_building, &position);

            if (building != NULL && building->position.x == x && building->position.y == y)
            {
                switch (building->type)
                {
                case HOUSE_1_BUILDING:
                    addCharacterInList(game_data->character_list, createCharacter(character_renderer, RAT_CHARACTER, &position, 1));
                    break;
                case HOUSE_2_BUILDING:
                    addCharacterInList(game_data->character_list, createCharacter(character_renderer, DAEMON_CHARACTER, &position, 1));
                    break;
                case HOUSE_3_BUILDING:
                    addCharacterInList(game_data->character_list, createCharacter(character_renderer, GIANT_CHARACTER, &position, 1));
                    break;
                default:
                    break;
                }
            }
        }
    }
}

extern void characterEventHandler(SDL_Event *event, client_game_data_t *game_data, character_renderer_t *character_renderer, window_t *window)
{
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        SDL_Point mouse_position = {event->button.x, event->button.y};
        SDL_Point tile_position = getTileCoord(&mouse_position, window, character_renderer->map_renderer);
        int elixir_cost = getCharacterElixirCost(DAEMON_CHARACTER);

        if (canPlaceCharacter(character_renderer, &tile_position, game_data->map_building, game_data->character_list) &&
            game_data->elixir_count - game_data->elixir_cost - elixir_cost >= 0)
        {
            addCharacterInList(game_data->character_list, createCharacter(character_renderer, DAEMON_CHARACTER, &tile_position, 0));
            game_data->elixir_cost += elixir_cost;
        }
    }
}
