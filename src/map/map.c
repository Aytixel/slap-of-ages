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

/**
 * @brief Créer la carte
 *
 * Initialise aussi les données des sprites nécessaires à l'affichage de la carte
 *
 * @param window un pointeur sur une fenêtre
 * @param size taille de la carte en nombre de cases
 * @return un pointer sur un **carte**
 */
extern map_t *createMap(window_t *window, int map_size)
{
    map_t *map = malloc(sizeof(map_t));

    map->size = map_size;
    map->tile_size = MAP_TILE_SIZE;
    map->ground_sprite = loadSprite(window, "asset/sprite/map/Tiles.png");

    if (map->ground_sprite == NULL)
    {
        free(map);

        return NULL;
    }

    map->tree_sprite = loadSprite(window, "asset/sprite/map/Trees.png");

    if (map->tree_sprite == NULL)
    {
        free(map);

        return NULL;
    }

    // position et taille de chaque sprite en nombre de cases
    map_sprite_rects_t sprite_tile_rects = {
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
        {5, 8, 1, 1},
        {0, 0, 3, 6},
        {3, 0, 5, 8},
        {8, 0, 7, 8},
        {0, 8, 3, 6},
        {3, 8, 5, 8},
        {8, 8, 7, 8}};

    map->sprite_tile_rects = sprite_tile_rects;

    // conversion de la position et la taille des sprites en pixel
    for (int i = 0; i < sizeof(map_sprite_rects_t) / sizeof(int); i++)
    {
        ((int *)&map->sprite_rects)[i] = ((int *)&map->sprite_tile_rects)[i] * MAP_TILE_SIZE;
    }

    return map;
}

/**
 * @brief Choisie un type d'arbre de manière pseudo-aléatoire à partir de coordonnées
 *
 * @param map un pointeur sur la carte
 * @param x position x en case
 * @param y position y en case
 * @param tree_source_rect un pointeur sur la position et la taille de l'arbre en pixel
 * @param tree_tile_rect un pointeur sur la position et la taille de l'arbre en case
 */
static void randomizeTreeType(map_t *map, int x, int y, SDL_Rect *tree_source_rect, SDL_Rect *tree_tile_rect)
{
    *tree_source_rect = map->sprite_rects.medium_tree;
    *tree_tile_rect = map->sprite_tile_rects.medium_tree;

    // decide quel type d'arbre afficher
    switch ((y * y + y - x) % 7)
    {
    case -3:
    case 4:
        *tree_source_rect = map->sprite_rects.large_tree;
        *tree_tile_rect = map->sprite_tile_rects.large_tree;
        break;
    case -2:
    case 2:
    case 3:
        *tree_source_rect = map->sprite_rects.small_tree;
        *tree_tile_rect = map->sprite_tile_rects.small_tree;
        break;
    case -5:
    case -7:
        *tree_source_rect = map->sprite_rects.small_dead_tree;
        *tree_tile_rect = map->sprite_tile_rects.small_dead_tree;
        break;
    case -4:
    case -6:
    case 0:
        *tree_source_rect = map->sprite_rects.medium_dead_tree;
        *tree_tile_rect = map->sprite_tile_rects.medium_dead_tree;
        break;
    case 5:
    case 7:
        *tree_source_rect = map->sprite_rects.large_dead_tree;
        *tree_tile_rect = map->sprite_tile_rects.large_dead_tree;
        break;
    default:
        break;
    }
}

/**
 * @brief Choisi si un arbre doit être placer de manière pseudo-aléatoire à partir de coordonnées
 *
 * @param x position x en case
 * @param y position y en case
 * @return **1 ou 0** en fonction de si l'on peut placer l'arbre ou non
 */
static int randomizeTreePlacement(int x, int y)
{
    return (x * y * 3 + x - y) % 9 == 0;
}

/**
 * @brief Fait le rendu de la carte sur la fenêtre
 *
 * Fait le rendu du sol en premier, avant de faire celui des arbres
 *
 * @param window un pointeur sur une fenêtre
 * @param map un pointeur sur la carte
 */
extern void renderMap(window_t *window, map_t *map)
{
    // ajuste la taille des cases pour que la carte remplisse toute la hauteur de la fenêtre
    map->tile_size = window->height / (map->size + 12);

    int offset = map->tile_size * ((float)map->size / 2 - 1);

    // calcul le nombre de cases pour remplir la moitié de la fenêtre
    int halft_width_tile_count = window->width / 2 / map->tile_size - map->size / 2 + 1;
    int halft_height_tile_count = window->height / 2 / map->tile_size - map->size / 2 + 1;

    // affichage du sol de la carte
    for (int y = -halft_height_tile_count; y < map->size + halft_height_tile_count; y++)
    {
        for (int x = -halft_width_tile_count; x < map->size + halft_width_tile_count; x++)
        {
            SDL_Rect ground_destination_rect = positionFromCenter(
                window,
                map->tile_size,
                map->tile_size,
                map->tile_size * x - offset,
                map->tile_size * y - offset,
                TRANSFORM_ORIGIN_BOTTOM_RIGHT);
            SDL_Rect *source_rect = &map->sprite_rects.dirt; // texture de terre par default

            // decide quel type de sol afficher
            if (x >= -1 && x <= map->size && y >= -1 && y <= map->size)
            {
                if (x == -1)
                {
                    if (y == -1)
                        source_rect = &map->sprite_rects.dirt_top_left;
                    else if (y == map->size)
                        source_rect = &map->sprite_rects.dirt_bottom_left;
                    else
                        source_rect = &map->sprite_rects.dirt_left;
                }
                else if (x == map->size)
                {
                    if (y == -1)
                        source_rect = &map->sprite_rects.dirt_top_right;
                    else if (y == map->size)
                        source_rect = &map->sprite_rects.dirt_bottom_right;
                    else
                        source_rect = &map->sprite_rects.dirt_right;
                }
                else if (y == -1)
                    source_rect = &map->sprite_rects.dirt_top;
                else if (y == map->size)
                    source_rect = &map->sprite_rects.dirt_bottom;
                else
                    source_rect = (x + y) % 2 // créer un damier avec la couleur des sprites
                                      ? &map->sprite_rects.dark_grass
                                      : &map->sprite_rects.light_grass;
            }

            SDL_RenderCopy(
                window->renderer,
                map->ground_sprite->texture,
                source_rect,
                &ground_destination_rect);
        }
    }

    // affichage des arbres au tour de la carte
    for (int y = -halft_height_tile_count; y < map->size + halft_height_tile_count + 8; y++)
    {
        for (int x = -halft_width_tile_count; x < map->size + halft_width_tile_count + 7; x++)
        {
            // texture de l'arbre moyen par default
            SDL_Rect tree_source_rect;
            SDL_Rect tree_tile_rect;

            randomizeTreeType(map, x, y, &tree_source_rect, &tree_tile_rect);

            // decide s'il faut afficher un arbre
            if ((
                    x < -1 ||
                    x > map->size + tree_tile_rect.w - 1 ||
                    y < -1 ||
                    y > map->size + tree_tile_rect.h - 1) &&
                randomizeTreePlacement(x, y))
            {
                SDL_Rect tree_destination_rect = positionFromCenter(
                    window,
                    map->tile_size * tree_tile_rect.w,
                    map->tile_size * tree_tile_rect.h,
                    map->tile_size * x - offset,
                    map->tile_size * y - offset,
                    TRANSFORM_ORIGIN_BOTTOM_RIGHT);

                SDL_RenderCopy(
                    window->renderer,
                    map->tree_sprite->texture,
                    &tree_source_rect,
                    &tree_destination_rect);
            }
        }
    }
}

/**
 * @brief Détruit la carte
 *
 * @param map une référence d'un pointeur sur la carte
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteMap(map_t **map)
{
    if (map == NULL || *map == NULL)
        return -1;

    destroySprite(&(*map)->ground_sprite);
    destroySprite(&(*map)->tree_sprite);
    free(*map);
    *map = NULL;

    return 0;
}