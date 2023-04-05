/**
 * @file character.h
 * @author Arthur Dureau, Lucas Dureau
 * @brief Fonction de gestion de l'affichage des characters
 * @version 1.0
 * @date 19/03/2023
 *
 *
 */

#ifndef __CHARACTER_RENDERER_H
#define __CHARACTER_RENDERER_H

typedef struct character_s character_t;

#include <SDL2/SDL.h>
#include "window/window.h"
#include "map/map_renderer.h"
#include "timer/timer.h"
#include "window/animation.h"

/**
 * @brief Enumération des différents types de troupe
 *
 */
typedef enum character_type_e
{
    GIANT_CHARACTER,  /**< géant goblin*/
    DAEMON_CHARACTER, /**< démon*/
    RAT_CHARACTER     /**< rat guerrier*/
} character_type_e;

/**
 * @brief Structure contenant les paramètres d'une animation
 *
 */
typedef struct
{
    sprite_t *sprite;       /**< sprite de l'animation*/
    int fps;                /**< image par seconde de l'animation*/
    int tile_size;          /**< taille en pixel d'une case sur le sprite*/
    int default_state;      /**< état d'animation par défaut*/
    int *state_frame_count; /**< tableau contenant le nombre d'images par état*/
} animation_data_t;

/**
 * @brief Structure contenant les paramètres des animations des troupes
 *
 */
typedef struct
{
    animation_data_t giant;  /**< paramètres d'animation pour le géant goblin*/
    animation_data_t daemon; /**< paramètres d'animation pour le démon*/
    animation_data_t rat;    /**< paramètres d'animation pour le rat guerrier*/
} character_animations_data_t;

/**
 * @brief Structure contenant les données pour afficher les troupes
 *
 */
typedef struct
{
    map_renderer_t *map_renderer;           /**< un pointeur sur un pointeur sur la carte*/
    character_animations_data_t animations; /**< paramètres d'animation des différentes troupes*/
} character_renderer_t;

/**
 * @brief Créer la structure qui gère l'affichage des troupes
 *
 * @param window un pointeur sur une fenêtre
 * @param map_renderer un pointeur sur la carte
 * @return character_renderer_t*
 */
extern character_renderer_t *createCharacterRenderer(window_t *window, map_renderer_t *map_renderer);

/**
 * @brief Détermine si on peut placer / afficher une troupe
 *
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 * @param position position de la troupe à placer / afficher
 * @return int
 */
extern int canRenderCharacter(character_renderer_t *character_renderer, SDL_Point *position);

/**
 * @brief Fait le rendu de d'une troupe sur la carte
 *
 * @param window un pointeur sur une fenêtre
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 * @param character un pointeur sur une troupe
 * @return int
 */
extern int renderCharacter(window_t *window, character_renderer_t *character_renderer, character_t *character);

/**
 * @brief Détruit la structure qui gère l'affichage des troupes
 *
 * @param character_renderer une référence sur un pointeur sur la structure qui gère l'affichage des troupes
 * @return int
 */
extern int deleteCharacterRenderer(character_renderer_t **character_renderer);

#endif