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

typedef struct
{
    SDL_Rect light_grass;
    SDL_Rect dark_grass;
    SDL_Rect dirt;
    SDL_Rect dirt_top_left;
    SDL_Rect dirt_top;
    SDL_Rect dirt_top_right;
    SDL_Rect dirt_right;
    SDL_Rect dirt_bottom_right;
    SDL_Rect dirt_bottom;
    SDL_Rect dirt_bottom_left;
    SDL_Rect dirt_left;
    SDL_Rect small_tree;
    SDL_Rect medium_tree;
    SDL_Rect large_tree;
    SDL_Rect small_dead_tree;
    SDL_Rect medium_dead_tree;
    SDL_Rect large_dead_tree;
} map_sprite_rects_t;

typedef struct
{
    int map_size;
    int tile_size;
    sprite_t *map_tile_sprite;
    sprite_t *map_tree_sprite;
    map_sprite_rects_t map_sprite_tile_rects;
    map_sprite_rects_t map_sprite_rects;
} map_t;

extern map_t *createMap(window_t *window, int map_size);

extern void renderMap(window_t *window, map_t *map);

extern int deleteMap(map_t **map);

#endif