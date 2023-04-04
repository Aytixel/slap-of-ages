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
    character->position.x = position->x;
    character->position.y = position->y;
    character->is_defender = is_defender;
    character->path = NULL;
    character->targeted_building = NULL;
    character->targeted_character = NULL;

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
            character_renderer->animations.daemon.tile_size,
            character_renderer->animations.daemon.state_frame_count,
            character_renderer->animations.daemon.sprite,
            character_renderer->animations.daemon.fps);
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

    if ((*character)->path != NULL)
        freeNodePath((*character)->path);

    free(*character);
    *character = NULL;
}

extern int characterTakesDamages(client_game_data_t *game_data, character_t *character, int damages)
{
    character->hp -= damages;

    if (character->hp <= 0)
    {
        removeCharacterFromList(game_data->character_list, character);

        return 1;
    }

    return 0;
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

extern character_t *getNearestCharacter(character_list_t *character_list, character_t *character)
{
    character_t *nearest_character = NULL;
    float nearest_distance = 0;

    for (int i = -CHARACTER_PLACEMENT_MARGIN; i < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; i++)
    {
        for (int j = -CHARACTER_PLACEMENT_MARGIN; j < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; j++)
        {
            character_t *character_ = getCharacter(character_list, i, j);

            if (character_ != NULL && character_ != character && character_->is_defender != character->is_defender)
            {
                float distance = sqrtf(powf(character_->position.x - (int)character->position.x, 2) + powf(character_->position.y - (int)character->position.y, 2));

                if (nearest_character == NULL || distance < nearest_distance)
                {
                    nearest_character = character_;
                    nearest_distance = distance;
                }
            }
        }
    }

    return nearest_character;
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

/**
 * @brief Récupère si la troupe est en train d'attacker un bâtiment
 *
 * @param character un pointeur sur une troupe
 * @return int
 */
static int characterAttackingBuilding(character_t *character)
{
    return character != NULL &&
           !character->is_defender &&
           character->targeted_building != NULL &&
           (int)character->position.x == character->targeted_building->position.x &&
           (int)character->position.y == character->targeted_building->position.y;
}

/**
 * @brief Récupère si la troupe est en train d'attacker une troupe
 *
 * @param character un pointeur sur une troupe
 * @return int
 */
static int characterAttackingCharacter(character_t *character)
{
    return character != NULL &&
           character->targeted_character != NULL &&
           (int)character->position.x == (int)character->targeted_character->position.x &&
           (int)character->position.y == (int)character->targeted_character->position.y;
}

/**
 * @brief Inflige des dégats à la cible de la troupe
 *
 * @param game_data un pointeur sur les données du jeu
 * @param character un pointeur sur une troupe
 * @return int
 */
static int characterAttackTarget(client_game_data_t *game_data, character_t *character, int attack_animation_state)
{
    if (isAnimationCycleEnded(character->animation))
    {
        if (characterAttackingBuilding(character) && buildingTakesDamages(game_data, character->targeted_building, character->attack))
        {
            for (int i = 0; i < game_data->character_list->count; i++)
            {
                if (game_data->character_list->list[i] != character &&
                    character->targeted_building == game_data->character_list->list[i]->targeted_building)
                    game_data->character_list->list[i]->targeted_building = NULL;
            }

            character->targeted_building = NULL;
        }

        if (characterAttackingCharacter(character) && characterTakesDamages(game_data, character->targeted_character, character->attack))
        {
            for (int i = 0; i < game_data->character_list->count; i++)
            {
                if (game_data->character_list->list[i] != character &&
                    character->targeted_character == game_data->character_list->list[i]->targeted_character)
                    game_data->character_list->list[i]->targeted_character = NULL;
            }

            character->targeted_character = NULL;
        }
    }

    if (characterAttackingBuilding(character) || characterAttackingCharacter(character))
    {
        changeAnimationState(character->animation, attack_animation_state);

        return 1;
    }

    return 0;
}

/**
 * @brief Met à jour la cible d'une troupe
 *
 * @param game_data un pointeur sur les données du jeu
 * @param character un pointeur sur une troupe
 */
static void
updateCharacterTarget(client_game_data_t *game_data, character_t *character)
{
    SDL_Point start = {character->position.x, character->position.y};

    if (!character->is_defender) // si la troupe est défenseure
    {
        character_t *nearest_character = getNearestCharacter(game_data->character_list, character);
        node_t *nearest_character_path = NULL;

        if (nearest_character != NULL)
        {
            SDL_Point goal = {nearest_character->position.x, nearest_character->position.y};

            nearest_character_path = aStar(start, goal, game_data->map_building, 1);

            if (nearest_character_path == NULL)
            {
                freeNodePath(nearest_character_path);
                nearest_character_path = aStar(start, goal, game_data->map_building, 0);
            }
        }

        building_t *nearest_building = getNearestBuilding(game_data->opponent_map_building, &start, 0);

        if (nearest_building == NULL)
            nearest_building = getNearestBuilding(game_data->opponent_map_building, &start, 1);

        node_t *nearest_building_path = NULL;

        if (nearest_building != NULL)
        {
            nearest_building_path = aStar(start, nearest_building->position, game_data->map_building, 1);

            if (nearest_building_path == NULL)
            {
                freeNodePath(nearest_building_path);
                nearest_building_path = aStar(start, nearest_building->position, game_data->map_building, 0);
            }
        }

        character->targeted_building = NULL;
        character->targeted_character = NULL;
        freeNodePath(character->path);
        character->path = NULL;

        if (nearest_building == NULL && nearest_character == NULL)
            return;

        int attack_animation_state = 0;

        switch (character->type)
        {
        case GIANT_CHARACTER:
            attack_animation_state = GOBLIN_GIANT_ATTACK_ANIM;

            if (nearest_building != NULL)
            {
                character->targeted_building = nearest_building;
                character->path = nearest_building_path;
            }
            else
            {
                character->targeted_character = nearest_character;
                character->path = nearest_character_path;
            }
            break;
        case DAEMON_CHARACTER:
            attack_animation_state = DAEMON_ATTACK_ANIM;

            if (nearest_building != NULL)
            {
                character->targeted_building = nearest_building;
                character->path = nearest_building_path;
            }
            else
            {
                character->targeted_character = nearest_character;
                character->path = nearest_character_path;
            }
            break;
        case RAT_CHARACTER:
            attack_animation_state = RAT_ATTACK_ANIM;

            if (nearest_character != NULL)
            {
                character->targeted_character = nearest_character;
                character->path = nearest_character_path;
            }
            else
            {
                character->targeted_building = nearest_building;
                character->path = nearest_building_path;
            }
            break;
        }

        if (character->targeted_character == NULL)
            freeNodePath(nearest_character_path);
        if (character->targeted_building == NULL)
            freeNodePath(nearest_building_path);

        characterAttackTarget(game_data, character, attack_animation_state);
    }
    else // si la troupe n'est défenseure pas
    {
        character_t *nearest_character = getNearestCharacter(game_data->character_list, character);
        node_t *nearest_character_path = NULL;

        if (nearest_character != NULL)
        {
            SDL_Point goal = {nearest_character->position.x, nearest_character->position.y};

            nearest_character_path = aStar(start, goal, game_data->map_building, 0);
        }

        int attack_animation_state = 0;

        switch (character->type)
        {
        case GIANT_CHARACTER:
            attack_animation_state = GOBLIN_GIANT_ATTACK_ANIM;
            break;
        case DAEMON_CHARACTER:
            attack_animation_state = DAEMON_ATTACK_ANIM;
            break;
        case RAT_CHARACTER:
            attack_animation_state = RAT_ATTACK_ANIM;
            break;
        }

        character->targeted_character = nearest_character;
        freeNodePath(character->path);
        character->path = nearest_character_path;

        characterAttackTarget(game_data, character, attack_animation_state);
    }
}

/**
 * @brief Met à jour la position d'une troupe
 *
 * @param character un pointeur sur une troupe
 */
static void updateCharacterPosition(character_t *character)
{
    if (character->path == NULL)
        return;

    SDL_Point position = getNextPositionInPath(character->path);
    int x_diff = position.x - (int)character->position.x;
    int y_diff = position.y - (int)character->position.y;
    float displacement = (float)character->speed / 30;

    if (x_diff > 0)
    {
        character->position.x += displacement;

        if (character->position.x > position.x)
            character->position.x = position.x;
    }
    else
    {
        character->position.x -= displacement;

        if (character->position.x < position.x)
            character->position.x = position.x;
    }

    if (y_diff > 0)
    {
        character->position.y += displacement;

        if (character->position.y > position.y)
            character->position.y = position.y;
    }
    else
    {
        character->position.y -= displacement;

        if (character->position.y < position.y)
            character->position.y = position.y;
    }

    if (x_diff == 0 && y_diff == 0)
        gotoNextPositionInPath(&character->path);
}

extern void updateCharacter(client_game_data_t *game_data)
{
    for (int i = 0; i < game_data->character_list->count; i++)
    {
        character_t *character = game_data->character_list->list[i];

        updateCharacterTarget(game_data, character);
        updateCharacterPosition(character);

        if (!characterAttackingBuilding(character) && !characterAttackingCharacter(character))
        {
            if (character->path == NULL)
            {
                switch (character->type)
                {
                case GIANT_CHARACTER:
                    changeAnimationState(character->animation, GOBLIN_GIANT_IDLE_ANIM);
                    break;
                case DAEMON_CHARACTER:
                    changeAnimationState(character->animation, DAEMON_IDLE_ANIM);
                    break;
                case RAT_CHARACTER:
                    changeAnimationState(character->animation, RAT_IDLE_ANIM);
                    break;
                }
            }
            else
            {
                switch (character->type)
                {
                case GIANT_CHARACTER:
                    changeAnimationState(character->animation, GOBLIN_GIANT_MOVE_ANIM);
                    break;
                case DAEMON_CHARACTER:
                    changeAnimationState(character->animation, DAEMON_MOVE_ANIM);
                    break;
                case RAT_CHARACTER:
                    changeAnimationState(character->animation, RAT_MOVE_ANIM);
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
