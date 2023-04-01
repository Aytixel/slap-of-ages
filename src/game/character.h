/**
 * @file character.h
 * @author Arthur Dureau
 * @brief Fonction de gestion des characters
 * @version 1.0
 * @date 19/03/2023
 *
 *
 */

#ifndef __CHARACTER_H
#define __CHARACTER_H

#include <stdio.h>
#include <stdlib.h>
#include "character_renderer.h"
#include "map/map_renderer.h"
#include "client/game_data.h"
#include "window/animation.h"
#include "window/animation_states.h"

#include <SDL2/SDL.h>

typedef struct character_s
{
    character_type_e type;
    SDL_Point position;
    int hp;
    int attack;
    int speed;
    int gold_cost;
} character_t;

extern character_t *createCharacter(character_type_e type, SDL_Point *position);
extern character_t ***createCharacterMatrix();
extern void renderCharacterMatrix(window_t *window, character_t ***map_character, character_renderer_t *character_renderer);
extern void destroyCharacter(character_t **character);
extern void clearCharacterMatrix(character_t ***character_matrix);
extern void destroyCharacterMatrix(character_t ****character_matrix);
extern void addCharacterInMatrix(character_t ***character_matrix, character_t *character);
extern void removeCharacterFromMatrix(character_t ***character_matrix, character_t *character);
extern int canPlaceCharacter(character_renderer_t *character_renderer, character_t *character, SDL_Point *position, character_t ***character_matrix);
extern character_t *getCharacter(character_t ***character_matrix, SDL_Point *position);
extern void characterEventHandler(SDL_Event *event, client_game_data_t *game_data, character_t ***map_character, character_renderer_t *character_renderer, window_t *window);

#endif