/**
 * @file character_renderer.c
 * @brief Permet de gérer l'affichage des personnages
 * @author Dureau Arthur
 * @version 1.0
 * @date 29/03/2023
 *
 *
 */

#include <stdlib.h>
#include "client/common.h"
#include "character_renderer.h"
#include "character.h"

#define CHARACTER_TILE_SIZE 32

extern character_renderer_t *createCharacterRenderer(window_t *window, map_renderer_t *map_renderer, character_type_e type)
{
    character_renderer_t *character_renderer = malloc(sizeof(character_renderer_t));

    character_renderer->map_renderer = map_renderer;

    switch (type)
    {
    case DAEMON_CHARACTER:
        character_renderer->sprite = loadSprite(window, "asset/sprite/characters/daemon.png");
        int daemon_frame_rate = 10;
        int daemon_tile_size = CHARACTER_TILE_SIZE;
        int daemon_states[] = {6, 6, 4, 8, -1};
        anim_t *daemon_animation = createAnim(daemon_tile_size,
                                              daemon_states,
                                              character_renderer->sprite,
                                              daemon_frame_rate);

        daemon_animation->current_state = DAEMON_IDLE_ANIM;
        character_renderer->animation = daemon_animation;
        break;
    default:
        character_renderer->sprite = NULL;
        break;
    }

    if (character_renderer->sprite == NULL)
    {
        free(character_renderer);
        return NULL;
    }

    return character_renderer;
}

extern int canRenderCharacter(character_renderer_t *character_renderer, SDL_Point *position, character_type_e character_type)
{
    return position->x >= -2 &&
           position->y >= -2 &&
           position->x < MAP_SIZE + 2 &&
           position->y < MAP_SIZE + 2;
}

extern int renderCharacter(window_t *window, character_renderer_t *character_renderer, character_t *character, SDL_Rect *destination_rect)
{
    SDL_Point position = character->position; // map_renderer->tile_size
    // SDL_FPoint
    position.x *= character_renderer->map_renderer->tile_size;
    position.y *= character_renderer->map_renderer->tile_size;
    position.x -= character_renderer->map_renderer->offset_from_center;
    position.y -= character_renderer->map_renderer->offset_from_center;
    position.x += character_renderer->map_renderer->tile_size / 2;
    position.y += character_renderer->map_renderer->tile_size;
    character_type_e character_type = character->type;

    if (!canRenderCharacter(character_renderer, &character->position, character_type))
    {
        return 0;
    }

    /*
    SDL_Rect current_frame_rect = character_renderer->animation->frame_rect;
    *destination_rect = positionFromCenter(
        window,
        character_renderer->map_renderer->tile_size * current_frame_rect.w,
        character_renderer->map_renderer->tile_size * current_frame_rect.h,
        character_renderer->map_renderer->tile_size * position->x - character_renderer->map_renderer->offset_from_center,
        character_renderer->map_renderer->tile_size * position->y - character_renderer->map_renderer->offset_from_center,
        TRANSFORM_ORIGIN_CENTER);

    SDL_Point destination_point = {destination_rect->x, destination_rect->y};
    */

    updateAnim(character_renderer->animation, character_renderer->animation->current_state, character_renderer->map_renderer->tile_size, &position, window, TRANSFORM_ORIGIN_BOTTOM);

    return 1;
}

extern int deleteCharacterRenderer(character_renderer_t **character_renderer)
{
    if (character_renderer == NULL || *character_renderer == NULL)
        return -1;

    destroySprite(&(*character_renderer)->sprite);
    free(*character_renderer);
    *character_renderer = NULL;

    return 0;
}
