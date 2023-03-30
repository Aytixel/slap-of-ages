#ifndef __CHARACTER_RENDERER_H
#define __CHARACTER_RENDERER_H

#include "window/window.h"
#include "map/map_renderer.h"
//#include "character.h"

typedef enum character_type_e
{
    GIANT_CHARACTER,
    ARCHER_CHARACTER
} character_type_e;

/**
 * @brief Structure contenant les positions et tailles des sprites des personnages
 *
 */
typedef struct
{
    SDL_Rect character_type_1;
    SDL_Rect character_type_2;
    SDL_Rect character_type_3;
} character_sprite_rects_t;

/**
 * @brief Structure contenant les données pour afficher les personnages
 *
 */
typedef struct
{
    map_renderer_t *map_renderer;
    sprite_t *sprite;
    character_sprite_rects_t sprite_tile_rects;
    character_sprite_rects_t sprite_rects;
} character_renderer_t;

extern character_renderer_t *createCharacterRenderer(window_t *window, map_renderer_t *map_renderer);
extern int canRenderCharacter(character_renderer_t *character_renderer, SDL_Point *position, character_type_e character_type);
extern int renderCharacter(window_t *window, character_renderer_t *character_renderer, SDL_Point *position, character_type_e character_type, SDL_Rect *destination_rect);
extern int deleteCharacterRenderer(character_renderer_t **character_renderer);

#endif