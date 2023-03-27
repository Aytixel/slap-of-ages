/**
 * @file map_renderer.c
 * @author Lucas Dureau
 * @brief Permet de gérer l'affichage de la carte
 * @version 0.1
 * @date 16/02/2023
 *
 */

#include <stdlib.h>
#include "map_renderer.h"

#define MAP_TILE_SIZE 16

extern map_renderer_t *createMapRenderer(window_t *window, int map_size)
{
    map_renderer_t *map_renderer = malloc(sizeof(map_renderer_t));

    map_renderer->size = map_size;
    map_renderer->tile_size = MAP_TILE_SIZE;
    map_renderer->ground_sprite = loadSprite(window, "asset/sprite/map/Tiles.png");

    if (map_renderer->ground_sprite == NULL)
    {
        free(map_renderer);

        return NULL;
    }

    map_renderer->tree_sprite = loadSprite(window, "asset/sprite/map/Trees.png");

    if (map_renderer->tree_sprite == NULL)
    {
        free(map_renderer);

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

    map_renderer->sprite_tile_rects = sprite_tile_rects;

    // conversion de la position et la taille des sprites en pixel
    for (int i = 0; i < sizeof(map_sprite_rects_t) / sizeof(int); i++)
    {
        ((int *)&map_renderer->sprite_rects)[i] = ((int *)&map_renderer->sprite_tile_rects)[i] * MAP_TILE_SIZE;
    }

    map_renderer->offset_from_center = map_renderer->tile_size * map_renderer->size / 2;

    return map_renderer;
}

/**
 * @brief Choisie un type d'arbre de manière pseudo-aléatoire à partir de coordonnées
 *
 * @param map_renderer un pointeur sur la carte
 * @param x position x en case
 * @param y position y en case
 * @param tree_source_rect un pointeur sur la position et la taille de l'arbre en pixel
 * @param tree_tile_rect un pointeur sur la position et la taille de l'arbre en case
 */
static void randomizeTreeType(map_renderer_t *map_renderer, int x, int y, SDL_Rect *tree_source_rect, SDL_Rect *tree_tile_rect)
{
    *tree_source_rect = map_renderer->sprite_rects.medium_tree;
    *tree_tile_rect = map_renderer->sprite_tile_rects.medium_tree;

    // decide quel type d'arbre afficher
    switch ((y * y + y - x) % 7)
    {
    case -3:
    case 4:
        *tree_source_rect = map_renderer->sprite_rects.large_tree;
        *tree_tile_rect = map_renderer->sprite_tile_rects.large_tree;
        break;
    case -2:
    case 2:
    case 3:
        *tree_source_rect = map_renderer->sprite_rects.small_tree;
        *tree_tile_rect = map_renderer->sprite_tile_rects.small_tree;
        break;
    case -5:
    case -7:
        *tree_source_rect = map_renderer->sprite_rects.small_dead_tree;
        *tree_tile_rect = map_renderer->sprite_tile_rects.small_dead_tree;
        break;
    case -4:
    case -6:
    case 0:
        *tree_source_rect = map_renderer->sprite_rects.medium_dead_tree;
        *tree_tile_rect = map_renderer->sprite_tile_rects.medium_dead_tree;
        break;
    case 5:
    case 7:
        *tree_source_rect = map_renderer->sprite_rects.large_dead_tree;
        *tree_tile_rect = map_renderer->sprite_tile_rects.large_dead_tree;
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

extern void renderMap(window_t *window, map_renderer_t *map_renderer)
{
    map_renderer->offset_from_center = map_renderer->tile_size * map_renderer->size / 2;

    // ajuste la taille des cases pour que la carte remplisse toute la hauteur de la fenêtre
    map_renderer->tile_size = window->height / (map_renderer->size + 12);

    int offset = map_renderer->offset_from_center - map_renderer->tile_size;

    // calcul le nombre de cases pour remplir la moitié de la fenêtre
    int halft_width_tile_count = window->width / 2 / map_renderer->tile_size - map_renderer->size / 2 + 1;
    int halft_height_tile_count = window->height / 2 / map_renderer->tile_size - map_renderer->size / 2 + 1;

    // affichage du sol de la carte
    for (int y = -halft_height_tile_count; y < map_renderer->size + halft_height_tile_count; y++)
    {
        for (int x = -halft_width_tile_count; x < map_renderer->size + halft_width_tile_count; x++)
        {
            SDL_Rect ground_destination_rect = positionFromCenter(
                window,
                map_renderer->tile_size,
                map_renderer->tile_size,
                map_renderer->tile_size * x - offset,
                map_renderer->tile_size * y - offset,
                TRANSFORM_ORIGIN_BOTTOM_RIGHT);
            SDL_Rect *source_rect = &map_renderer->sprite_rects.dirt; // texture de terre par default

            // decide quel type de sol afficher
            if (x >= -1 && x <= map_renderer->size && y >= -1 && y <= map_renderer->size)
            {
                if (x == -1)
                {
                    if (y == -1)
                        source_rect = &map_renderer->sprite_rects.dirt_top_left;
                    else if (y == map_renderer->size)
                        source_rect = &map_renderer->sprite_rects.dirt_bottom_left;
                    else
                        source_rect = &map_renderer->sprite_rects.dirt_left;
                }
                else if (x == map_renderer->size)
                {
                    if (y == -1)
                        source_rect = &map_renderer->sprite_rects.dirt_top_right;
                    else if (y == map_renderer->size)
                        source_rect = &map_renderer->sprite_rects.dirt_bottom_right;
                    else
                        source_rect = &map_renderer->sprite_rects.dirt_right;
                }
                else if (y == -1)
                    source_rect = &map_renderer->sprite_rects.dirt_top;
                else if (y == map_renderer->size)
                    source_rect = &map_renderer->sprite_rects.dirt_bottom;
                else
                    source_rect = (x + y) % 2 // créer un damier avec la couleur des sprites
                                      ? &map_renderer->sprite_rects.dark_grass
                                      : &map_renderer->sprite_rects.light_grass;
            }

            SDL_RenderCopy(
                window->renderer,
                map_renderer->ground_sprite->texture,
                source_rect,
                &ground_destination_rect);
        }
    }

    // affichage des arbres au tour de la carte
    for (int y = -halft_height_tile_count; y < map_renderer->size + halft_height_tile_count + 8; y++)
    {
        for (int x = -halft_width_tile_count; x < map_renderer->size + halft_width_tile_count + 7; x++)
        {
            // texture de l'arbre moyen par default
            SDL_Rect tree_source_rect;
            SDL_Rect tree_tile_rect;

            randomizeTreeType(map_renderer, x, y, &tree_source_rect, &tree_tile_rect);

            // decide s'il faut afficher un arbre
            if ((
                    x < -1 ||
                    x > map_renderer->size + tree_tile_rect.w - 1 ||
                    y < -1 ||
                    y > map_renderer->size + tree_tile_rect.h - 1) &&
                randomizeTreePlacement(x, y))
            {
                SDL_Rect tree_destination_rect = positionFromCenter(
                    window,
                    map_renderer->tile_size * tree_tile_rect.w,
                    map_renderer->tile_size * tree_tile_rect.h,
                    map_renderer->tile_size * x - offset,
                    map_renderer->tile_size * y - offset,
                    TRANSFORM_ORIGIN_BOTTOM_RIGHT);

                SDL_RenderCopy(
                    window->renderer,
                    map_renderer->tree_sprite->texture,
                    &tree_source_rect,
                    &tree_destination_rect);
            }
        }
    }
}

extern int deleteMapRenderer(map_renderer_t **map_renderer)
{
    if (map_renderer == NULL || *map_renderer == NULL)
        return -1;

    destroySprite(&(*map_renderer)->ground_sprite);
    destroySprite(&(*map_renderer)->tree_sprite);
    free(*map_renderer);
    *map_renderer = NULL;

    return 0;
}

extern SDL_Point getTileCoord(SDL_Point *mouse_position, window_t *window, map_renderer_t *map_renderer)
{

    SDL_Rect center_coord;
    SDL_Point tile_coord;

    int maxPixels = map_renderer->tile_size * map_renderer->size;

    center_coord = positionToCenter(window, 0, 0);

    tile_coord.x = (mouse_position->x - center_coord.x + maxPixels / 2) / map_renderer->tile_size;
    tile_coord.y = (mouse_position->y - center_coord.y + maxPixels / 2) / map_renderer->tile_size;

    if (tile_coord.x < 0 || tile_coord.y < 0 || tile_coord.x >= map_renderer->size || tile_coord.y >= map_renderer->size)
    {
        tile_coord.x = -1;
        tile_coord.y = -1;
        return tile_coord;
    }

    return tile_coord;
}