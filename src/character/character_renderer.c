/**
 * @file character_renderer.c
 * @author Dureau Arthur, Lucas Dureau
 * @brief Permet de gérer l'affichage des personnages
 * @version 1.1
 * @date 29/03/2023
 *
 *
 */

#include <stdlib.h>
#include "client/common.h"
#include "character_renderer.h"
#include "character.h"

extern character_renderer_t *createCharacterRenderer(window_t *window, map_renderer_t *map_renderer)
{
    character_renderer_t *character_renderer = malloc(sizeof(character_renderer_t));

    character_renderer->map_renderer = map_renderer;

    // giant
    character_renderer->animations.giant.sprite = loadSprite(window, "asset/sprite/characters/giant_goblin.png");

    if (character_renderer->animations.giant.sprite == NULL)
    {
        free(character_renderer);

        return NULL;
    }

    character_renderer->animations.giant.fps = 4;
    character_renderer->animations.giant.tile_size = 32;
    character_renderer->animations.giant.default_state = GOBLIN_GIANT_IDLE_ANIM;
    character_renderer->animations.giant.state_frame_count = malloc(sizeof(int) * 6);
    int giant_states[] = {2, 8, 7, 4, 6, -1};

    for (int i = 0; i < 6; i++)
        character_renderer->animations.giant.state_frame_count[i] = giant_states[i];

    // daemon
    character_renderer->animations.daemon.sprite = loadSprite(window, "asset/sprite/characters/daemon.png");

    if (character_renderer->animations.daemon.sprite == NULL)
    {
        destroySprite(&character_renderer->animations.giant.sprite);
        free(character_renderer->animations.giant.state_frame_count);
        free(character_renderer);

        return NULL;
    }

    character_renderer->animations.daemon.fps = 8;
    character_renderer->animations.daemon.tile_size = 32;
    character_renderer->animations.daemon.default_state = DAEMON_IDLE_ANIM;
    character_renderer->animations.daemon.state_frame_count = malloc(sizeof(int) * 5);
    int daemon_states[] = {6, 6, 4, 8, -1};

    for (int i = 0; i < 5; i++)
        character_renderer->animations.daemon.state_frame_count[i] = daemon_states[i];

    // rat
    character_renderer->animations.rat.sprite = loadSprite(window, "asset/sprite/characters/ratfolk_axe.png");

    if (character_renderer->animations.rat.sprite == NULL)
    {
        destroySprite(&character_renderer->animations.giant.sprite);
        destroySprite(&character_renderer->animations.daemon.sprite);
        free(character_renderer->animations.giant.state_frame_count);
        free(character_renderer->animations.daemon.state_frame_count);
        free(character_renderer);

        return NULL;
    }

    character_renderer->animations.rat.fps = 8;
    character_renderer->animations.rat.tile_size = 32;
    character_renderer->animations.rat.default_state = RAT_IDLE_ANIM;
    character_renderer->animations.rat.state_frame_count = malloc(sizeof(int) * 6);
    int rat_states[] = {4, 8, 12, 4, 5, -1};

    for (int i = 0; i < 6; i++)
        character_renderer->animations.rat.state_frame_count[i] = rat_states[i];

    return character_renderer;
}

extern int canRenderCharacter(character_renderer_t *character_renderer, SDL_Point *position)
{
    return position->x >= -CHARACTER_PLACEMENT_MARGIN &&
           position->y >= -CHARACTER_PLACEMENT_MARGIN &&
           position->x < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN &&
           position->y < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN;
}

extern int renderCharacter(window_t *window, character_renderer_t *character_renderer, character_t *character)
{
    SDL_Point character_position = {character->position.x, character->position.y};

    if (!canRenderCharacter(character_renderer, &character_position))
        return 0;

    SDL_Point position = {
        character->position.x * character_renderer->map_renderer->tile_size - character_renderer->map_renderer->offset_from_center + character_renderer->map_renderer->tile_size / 2,
        character->position.y * character_renderer->map_renderer->tile_size - character_renderer->map_renderer->offset_from_center + character_renderer->map_renderer->tile_size};

    updateAnimation(character->animation, character_renderer->map_renderer->tile_size, &position, window, TRANSFORM_ORIGIN_BOTTOM);

    return 1;
}

extern int deleteCharacterRenderer(character_renderer_t **character_renderer)
{
    if (character_renderer == NULL || *character_renderer == NULL)
        return -1;

    destroySprite(&(*character_renderer)->animations.giant.sprite);
    destroySprite(&(*character_renderer)->animations.daemon.sprite);
    destroySprite(&(*character_renderer)->animations.rat.sprite);
    free((*character_renderer)->animations.giant.state_frame_count);
    free((*character_renderer)->animations.daemon.state_frame_count);
    free((*character_renderer)->animations.rat.state_frame_count);
    free(*character_renderer);
    *character_renderer = NULL;

    return 0;
}
