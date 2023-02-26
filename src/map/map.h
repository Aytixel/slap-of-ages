/**
 * @file map.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de map/map.c
 * @version 0.1
 * @date 16/02/2023
 *
 */

#ifndef __MAP_H
#define __MAP_H

#include "window/window.h"

/**
 * @brief Structure contenant les positions et tailles des sprites de la carte
 *
 */
typedef struct
{
    SDL_Rect light_grass;       /**< herbe clair*/
    SDL_Rect dark_grass;        /**< herbe sombre*/
    SDL_Rect dirt;              /**< terre*/
    SDL_Rect dirt_top_left;     /**< transition herbe terre haut gauche*/
    SDL_Rect dirt_top;          /**< transition herbe terre haut*/
    SDL_Rect dirt_top_right;    /**< transition herbe terre haut droit*/
    SDL_Rect dirt_right;        /**< transition herbe terre droite*/
    SDL_Rect dirt_bottom_right; /**< transition herbe terre bas droit*/
    SDL_Rect dirt_bottom;       /**< transition herbe terre bas*/
    SDL_Rect dirt_bottom_left;  /**< transition herbe terre bas gauche*/
    SDL_Rect dirt_left;         /**< transition herbe terre gauche*/
    SDL_Rect small_tree;        /**< petit arbre*/
    SDL_Rect medium_tree;       /**< moyen arbre*/
    SDL_Rect large_tree;        /**< grand arbre*/
    SDL_Rect small_dead_tree;   /**< petit arbre mort*/
    SDL_Rect medium_dead_tree;  /**< moyen arbre mort*/
    SDL_Rect large_dead_tree;   /**< grand arbre mort*/
} map_sprite_rects_t;

/**
 * @brief Structure contenant les données de la carte
 *
 */
typedef struct
{
    int size;                             /**< taille de la carte en nombre de cases*/
    int tile_size;                        /**< taille d'une case en pixel*/
    sprite_t *ground_sprite;              /**< grille de sprite contenant les sprites du sol */
    sprite_t *tree_sprite;                /**< grille de sprite contenant les sprites des arbres */
    map_sprite_rects_t sprite_tile_rects; /**< positions et tailles des sprites de la carte en nombre de cases*/
    map_sprite_rects_t sprite_rects;      /**< positions et tailles des sprites de la carte en nombre de pixels*/
} map_t;

extern map_t *createMap(window_t *window, int map_size);

extern void renderMap(window_t *window, map_t *map);

extern int deleteMap(map_t **map);

#endif