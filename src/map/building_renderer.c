/**
 * @file building_renderer.c
 * @author Lucas Dureau
 * @brief Permet de gérer l'affichage des bâtiments
 * @version 0.1
 * @date 03/03/2023
 *
 */

#include <stdlib.h>
#include "client/common.h"
#include "building_renderer.h"

#define BUILDING_TILE_SIZE 16

extern building_renderer_t *createBuildingRenderer(window_t *window, map_renderer_t *map_renderer)
{
    building_renderer_t *building_renderer = malloc(sizeof(building_renderer_t));

    building_renderer->map_renderer = map_renderer;
    building_renderer->sprite = loadSprite(window, "asset/sprite/building/MasterSimple.png");

    if (building_renderer->sprite == NULL)
    {
        free(building_renderer);

        return NULL;
    }

    // position et taille de chaque sprite en nombre de cases
    building_sprite_rects_t sprite_tile_rects = {
        {10, 0, 1, 1},
        {10, 1, 1, 1},
        {11, 1, 1, 1},
        {10, 2, 1, 2},
        {9, 0, 1, 1},
        {10, 4, 1, 1},
        {9, 1, 1, 1},
        {12, 6, 1, 1},
        {12, 4, 1, 2},
        {10, 5, 2, 1},
        {11, 6, 1, 2}};

    building_renderer->sprite_tile_rects = sprite_tile_rects;

    // conversion de la position et la taille des sprites en pixel
    for (int i = 0; i < sizeof(building_sprite_rects_t) / sizeof(int); i++)
    {
        ((int *)&building_renderer->sprite_rects)[i] = ((int *)&building_renderer->sprite_tile_rects)[i] * BUILDING_TILE_SIZE;
    }

    return building_renderer;
}

extern int canRenderBuilding(building_renderer_t *building_renderer, SDL_Point *position, building_type_e building_type)
{
    return position->x >= 0 &&
           position->y >= 0 &&
           position->x + ((SDL_Rect *)&building_renderer->sprite_tile_rects)[building_type].w <= MAP_SIZE &&
           position->y + ((SDL_Rect *)&building_renderer->sprite_tile_rects)[building_type].h <= MAP_SIZE;
}

extern int renderBuilding(window_t *window, building_renderer_t *building_renderer, SDL_Point *position, building_type_e building_type, SDL_Rect *destination_rect)
{
    if (!canRenderBuilding(building_renderer, position, building_type))
        return 0;

    SDL_Rect tile_rect = ((SDL_Rect *)&building_renderer->sprite_tile_rects)[building_type];
    *destination_rect = positionFromCenter(
        window,
        building_renderer->map_renderer->tile_size * tile_rect.w,
        building_renderer->map_renderer->tile_size * tile_rect.h,
        building_renderer->map_renderer->tile_size * position->x - building_renderer->map_renderer->offset_from_center,
        building_renderer->map_renderer->tile_size * position->y - building_renderer->map_renderer->offset_from_center,
        TRANSFORM_ORIGIN_TOP_LEFT);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        ((SDL_Rect *)&building_renderer->sprite_rects) + building_type,
        destination_rect);

    return 1;
}

extern int deleteBuildingRenderer(building_renderer_t **building_renderer)
{
    if (building_renderer == NULL || *building_renderer == NULL)
        return -1;

    destroySprite(&(*building_renderer)->sprite);
    free(*building_renderer);
    *building_renderer = NULL;

    return 0;
}