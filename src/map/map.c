/**
 * @file map.c
 * @author Lucas Dureau
 * @brief Permet de gérer l'affichage de la carte
 * @version 0.1
 * @date 16/02/2023
 *
 */

#include <stdio.h>
#include "map.h"

#define MAP_TILE_SIZE 16

extern map_t *createMap(window_t *window, int map_size)
{
    map_t *map = malloc(sizeof(map_t));

    map->map_size = map_size;
    map->tile_size = 16;
    map->map_tile_sprite = loadSprite(window, "asset/sprite/map/Tiles.png");

    if (map->map_tile_sprite == NULL)
    {
        free(map);

        return NULL;
    }

    // position et taille de chaque sprite en nombre de case
    map_sprite_rects_t map_sprite_tile_rects = {
        {9, 6, 1, 1},
        {10, 6, 1, 1},
        {9, 8, 1, 1},
        {5, 7, 1, 1},
        {6, 7, 1, 1},
        {7, 7, 1, 1},
        {7, 8, 1, 1},
        {7, 9, 1, 1},
        {6, 9, 1, 1},
        {5, 9, 1, 1},
        {5, 8, 1, 1}};

    map->map_sprite_tile_rects = map_sprite_tile_rects;

    // conversion de la position et la taille des sprites en pixel
    for (int i = 0; i < sizeof(map_sprite_rects_t) / sizeof(int); i++)
    {
        ((int *)&map->map_sprite_rects)[i] = ((int *)&map->map_sprite_tile_rects)[i] * MAP_TILE_SIZE;
    }

    return map;
}

extern void renderMap(window_t *window, map_t *map)
{
    map->tile_size = window->height / (map->map_size + 2);

    int offset = map->tile_size * ((float)map->map_size / 2 - 1);
    int width_filling_tile_count = window->width / 2 / map->tile_size - map->map_size / 2 + 1;
    int height_filling_tile_count = window->width / 2 / map->tile_size - map->map_size / 2 + 1;

    for (int x = -width_filling_tile_count; x < map->map_size + width_filling_tile_count; x++)
    {
        for (int y = -height_filling_tile_count; y < map->map_size + height_filling_tile_count; y++)
        {
            SDL_Rect destination_rect = positionFromCenter(
                window,
                map->tile_size, map->tile_size,
                map->tile_size * x - offset, map->tile_size * y - offset,
                TRANSFORM_ORIGIN_BOTTOM_RIGHT);
            SDL_Rect *source_rect = &map->map_sprite_rects.dirt; // texture de terre par default

            if (x >= -1 && x <= map->map_size && y >= -1 && y <= map->map_size)
            {
                if (x == -1)
                {
                    if (y == -1)
                        source_rect = &map->map_sprite_rects.dirt_top_left;
                    else if (y == map->map_size)
                        source_rect = &map->map_sprite_rects.dirt_bottom_left;
                    else
                        source_rect = &map->map_sprite_rects.dirt_left;
                }
                else if (x == map->map_size)
                {
                    if (y == -1)
                        source_rect = &map->map_sprite_rects.dirt_top_right;
                    else if (y == map->map_size)
                        source_rect = &map->map_sprite_rects.dirt_bottom_right;
                    else
                        source_rect = &map->map_sprite_rects.dirt_right;
                }
                else if (y == -1)
                    source_rect = &map->map_sprite_rects.dirt_top;
                else if (y == map->map_size)
                    source_rect = &map->map_sprite_rects.dirt_bottom;
                else
                    source_rect = (x + y) % 2 // créer un damier avec la couleur des sprites
                                      ? &map->map_sprite_rects.dark_grass
                                      : &map->map_sprite_rects.light_grass;
            }

            SDL_RenderCopy(
                window->renderer,
                map->map_tile_sprite->texture,
                source_rect,
                &destination_rect);
        }
    }
}

extern int deleteMap(map_t **map)
{
    if (map == NULL || *map == NULL)
        return -1;

    destroySprite(&(*map)->map_tile_sprite);
    free(*map);
    *map = NULL;

    return 0;
}