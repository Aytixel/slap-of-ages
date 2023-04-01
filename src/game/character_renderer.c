#include <stdlib.h>
#include "client/common.h"
#include "character_renderer.h"
#include "character.h"

#define CHARACTER_TILE_SIZE 8

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
    return position->x >= 0 &&
           position->y >= 0 &&
           position->x + ((SDL_Rect *)&character_renderer->sprite_tile_rects)[character_type].w <= MAP_SIZE &&
           position->y + ((SDL_Rect *)&character_renderer->sprite_tile_rects)[character_type].h <= MAP_SIZE;
}

extern int renderCharacter(window_t *window, character_renderer_t *character_renderer, character_t *character, SDL_Rect *destination_rect)
{
    SDL_Point *position = &(character->position);
    character_type_e character_type = character->type;

    if (!canRenderCharacter(character_renderer, position, character_type))
        return 0;


    updateAnim(character_renderer->animation, character_renderer->animation->current_state, CHARACTER_TILE_SIZE, &(character->position), window);

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
