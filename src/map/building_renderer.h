/**
 * @file building_renderer.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de map/building_renderer.c
 * @version 0.1
 * @date 03/03/2023
 *
 */

#ifndef __BUILDING_H
#define __BUILDING_H

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
    SPACE_FILLER_WALL_BUILDING,
} building_type_e;

/**
 * @brief Structure contenant les positions et tailles des sprites des bâtiments
 *
 */
typedef struct
{
    SDL_Rect house_1;           /**< maison 1*/
    SDL_Rect house_2;           /**< maison 2*/
    SDL_Rect house_3;           /**< maison 3*/
    SDL_Rect mill;              /**< moulin*/
    SDL_Rect mine;              /**< mine*/
    SDL_Rect field;             /**< champs*/
    SDL_Rect well;              /**< puit*/
    SDL_Rect corner_wall;       /**< coin de mur*/
    SDL_Rect vertical_wall;     /**< mur verticale*/
    SDL_Rect horizontal_wall;   /**< mur horizontale*/
    SDL_Rect space_filler_wall; /**< mur de remplissage entre deux murs l'un au-dessus de l'autre*/
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

extern building_renderer_t *createBuildingRenderer(window_t *window, map_renderer_t *map_renderer);

extern int canRenderBuilding(building_renderer_t *building, SDL_Point *position, building_type_e building_type);

extern int renderBuilding(window_t *window, building_renderer_t *building, SDL_Point *position, building_type_e building_type, SDL_Rect *destination_rect);

extern int deleteBuildingRenderer(building_renderer_t **building);

#endif