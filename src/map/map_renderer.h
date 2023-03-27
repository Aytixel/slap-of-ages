/**
 * @file map_renderer.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de map/map_renderer.c
 * @version 0.1
 * @date 16/02/2023
 *
 */

#ifndef __MAP_RENDERER_H
#define __MAP_RENDERER_H

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
    int offset_from_center;               /**< décalage en pixel depuis le centre de la carte jusqu'au bord*/
} map_renderer_t;

/**
 * @brief Créer la carte
 *
 * Initialise aussi les données des sprites nécessaires à l'affichage de la carte
 *
 * @param window un pointeur sur une fenêtre
 * @param size taille de la carte en nombre de cases
 * @return un pointer sur un **carte**
 */
extern map_renderer_t *createMapRenderer(window_t *window, int map_size);

/**
 * @brief Fait le rendu de la carte sur la fenêtre
 *
 * Fait le rendu du sol en premier, avant de faire celui des arbres
 *
 * @param window un pointeur sur une fenêtre
 * @param map_renderer un pointeur sur la carte
 */
extern void renderMap(window_t *window, map_renderer_t *map_renderer);

/**
 * @brief Détruit la carte
 *
 * @param map_renderer une référence d'un pointeur sur la carte
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteMapRenderer(map_renderer_t **map_renderer);

/**
 * @brief Permet de gérer la position du batiment selon le placement de la souris
 *
 * @param mouse_position la position de la souris
 * @param window un pointeur sur une fenêtre
 * @param map_renderer un pointeur sur la structure de rendu de la carte
 * @return SDL_Point contenant les coordonnées du bâtiment en cases, si le clique de la souris est en dehors de la carte, la valeur retournée est {-1, -1}.
 */
extern SDL_Point getTileCoord(SDL_Point *mouse_position, window_t *window, map_renderer_t *map_renderer);

#endif