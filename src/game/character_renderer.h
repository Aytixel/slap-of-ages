/**
 * @file character.h
 * @author Arthur Dureau
 * @brief Fonction de gestion de l'affichage des characters
 * @version 1.0
 * @date 19/03/2023
 *
 *
 */

#ifndef __CHARACTER_RENDERER_H
#define __CHARACTER_RENDERER_H

#include "window/window.h"
#include <SDL2/SDL.h>
#include "map/map_renderer.h"
#include "timer/timer.h"
#include "window/animation.h"

typedef struct character_s character_t;

typedef enum character_type_e
{
    GIANT_CHARACTER,
    DAEMON_CHARACTER
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
    anim_t *animation;
} character_renderer_t;

extern character_renderer_t *createCharacterRenderer(window_t *window, map_renderer_t *map_renderer, character_type_e type);
extern int canRenderCharacter(character_renderer_t *character_renderer, SDL_Point *position, character_type_e character_type);
extern int renderCharacter(window_t *window, character_renderer_t *character_renderer, character_t *character, SDL_Rect *destination_rect);
extern int deleteCharacterRenderer(character_renderer_t **character_renderer);

#endif