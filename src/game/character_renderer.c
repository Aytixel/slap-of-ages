#include <stdlib.h>
#include "client/common.h"
#include "character_renderer.h"

#define CHARACTER_TILE_SIZE 16

extern character_renderer_t *createCharacterRenderer(window_t *window, map_renderer_t *map_renderer)
{
    character_renderer_t *character_renderer = malloc(sizeof(character_renderer_t));

    character_renderer->map_renderer = map_renderer;
    character_renderer->sprite = loadSprite(window, "asset/sprite/characters/daemon.png");

    if (character_renderer->sprite == NULL)
    {
        free(character_renderer);

        return NULL;
    }

    // position et taille de chaque sprite en nombre de cases
    character_sprite_rects_t sprite_tile_rects = {
        {0, 0, 3, 3},
        {1, 1, 3, 3},
        {2, 2, 3, 3}};

    character_renderer->sprite_tile_rects = sprite_tile_rects;

    // conversion de la position et la taille des sprites en pixel
    for (int i = 0; i < sizeof(character_sprite_rects_t) / sizeof(int); i++)
    {
        ((int *)&character_renderer->sprite_rects)[i] = ((int *)&character_renderer->sprite_tile_rects)[i] * CHARACTER_TILE_SIZE;
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

extern int renderCharacter(window_t *window, character_renderer_t *character_renderer, SDL_Point *position, character_type_e character_type, SDL_Rect *destination_rect)
{
    if (!canRenderCharacter(character_renderer, position, character_type))
        return 0;

    SDL_Rect tile_rect = ((SDL_Rect *)&character_renderer->sprite_tile_rects)[character_type];
    *destination_rect = positionFromCenter(
        window,
        character_renderer->map_renderer->tile_size * tile_rect.w,
        character_renderer->map_renderer->tile_size * tile_rect.h,
        character_renderer->map_renderer->tile_size * position->x - character_renderer->map_renderer->offset_from_center,
        character_renderer->map_renderer->tile_size * position->y - character_renderer->map_renderer->offset_from_center,
        TRANSFORM_ORIGIN_CENTER);

    SDL_RenderCopy(
        window->renderer,
        character_renderer->sprite->texture,
        ((SDL_Rect *)&character_renderer->sprite_rects) + character_type,
        destination_rect);

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
