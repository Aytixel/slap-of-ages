/**
 * @file building_renderer.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de map/building_renderer.c
 * @version 0.1
 * @date 03/03/2023
 *
 */

#ifndef __BUILDING_RENDERER_H
#define __BUILDING_RENDERER_H

#include "window/window.h"
#include "map_renderer.h"

/**
 * @brief Enumération des différents types de bâtiments
 *
 */
typedef enum
{
    HOUSE_1_BUILDING,
    HOUSE_2_BUILDING,
    HOUSE_3_BUILDING,
    MILL_BUILDING,
    MINE_BUILDING,
    FIELD_BUILDING,
    WELL_BUILDING,
    CORNER_WALL_BUILDING,
    VERTICAL_WALL_BUILDING,
    HORIZONTAL_WALL_BUILDING,
    VERTICAL_SPACE_FILLER_WALL_BUILDING,
    HORIZONTAL_SPACE_FILLER_WALL_BUILDING,
} building_type_e;

/**
 * @brief Structure contenant les positions et tailles des sprites des bâtiments
 *
 */
typedef struct
{
    SDL_Rect house_1;                      /**< maison 1*/
    SDL_Rect house_2;                      /**< maison 2*/
    SDL_Rect house_3;                      /**< maison 3*/
    SDL_Rect mill;                         /**< moulin*/
    SDL_Rect mine;                         /**< mine*/
    SDL_Rect field;                        /**< champs*/
    SDL_Rect well;                         /**< puit*/
    SDL_Rect corner_wall;                  /**< coin de mur*/
    SDL_Rect vertical_wall;                /**< mur verticale*/
    SDL_Rect horizontal_wall;              /**< mur horizontale*/
    SDL_Rect vertical_space_filler_wall;   /**< mur de remplissage entre deux murs l'un au-dessus de l'autre*/
    SDL_Rect horizontal_space_filler_wall; /**< mur de remplissage entre deux murs l'un à côté de l'autre*/
} building_sprite_rects_t;

/**
 * @brief Structure contenant les données pour afficher les bâtiments
 *
 */
typedef struct
{
    map_renderer_t *map_renderer;              /**< pointeur sur la carte*/
    sprite_t *sprite;                          /**< grille de sprite contenant les sprites */
    building_sprite_rects_t sprite_tile_rects; /**< positions et tailles des sprites des bâtiments en nombre de cases*/
    building_sprite_rects_t sprite_rects;      /**< positions et tailles des sprites des bâtiments en nombre de pixels*/
} building_renderer_t;

/**
 * @brief Créer la structure qui gère l'affichage des bâtiments
 *
 * @param window un pointeur sur une fenêtre
 * @param map_renderer un pointeur sur la carte
 * @return building_renderer_t*
 */
extern building_renderer_t *createBuildingRenderer(window_t *window, map_renderer_t *map_renderer);

/**
 * @brief Détermine si on peut placer / afficher un certain type de bâtiment à tel endroit
 *
 * @param building_renderer un pointeur sur la structure qui gère l'affichage des bâtiments
 * @param position position du bâtiment en nombre de case
 * @param building_type type de batiment à placer / afficher
 * @return **1** si on peut le placer, **0** sinon
 */
extern int canRenderBuilding(building_renderer_t *building_renderer, SDL_Point *position, building_type_e building_type);

/**
 * @brief Fait le rendu de d'un batiment sur la carte
 *
 * @param window un pointeur sur une fenêtre
 * @param building_renderer un pointeur sur la structure qui gère l'affichage des bâtiments
 * @param position position du bâtiment en nombre de case
 * @param building_type type de batiment à afficher
 * @return **1** si on peut le placer, **0** sinon
 */
extern int renderBuilding(window_t *window, building_renderer_t *building_renderer, SDL_Point *position, building_type_e building_type);

/**
 * @brief Détruit la structure qui gère l'affichage des bâtiments
 *
 * @param building_renderer une référence d'un pointeur sur la structure qui gère l'affichage des bâtiments
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteBuildingRenderer(building_renderer_t **building_renderer);

#endif