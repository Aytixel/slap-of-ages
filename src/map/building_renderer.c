/**
 * @file building_renderer.c
 * @author Lucas Dureau
 * @brief Permet de gérer l'affichage des bâtiments
 * @version 0.1
 * @date 03/03/2023
 *
 */

#include <stdlib.h>
#include "building_renderer.h"

#define BUILDING_TILE_SIZE 16

/**
 * @brief Créer la structure qui gère l'affichage des bâtiments
 *
 * @param window un pointeur sur une fenêtre
 * @param map_renderer un pointeur sur la carte
 * @return building_renderer_t*
 */
extern building_renderer_t *createBuildingRenderer(window_t *window, map_renderer_t *map_renderer)
{
    building_renderer_t *building_renderer = malloc(sizeof(building_renderer_t));

    building_renderer->map_renderer = map_renderer;
    building_renderer->sprite = loadSprite(window, "asset/sprite/building_renderer/MasterSimple.png");

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

/**
 * @brief Détermine si on peut placer / afficher un certain type de bâtiment à tel endroit
 *
 * @param building_renderer un pointeur sur la structure qui gère l'affichage des bâtiments
 * @param x position en x en nombre de case
 * @param y position en y en nombre de case
 * @param building_type type de batiment à placer / afficher
 * @return **1** si on peut le placer, **0** sinon
 */
extern int canRenderBuilding(building_renderer_t *building_renderer, int x, int y, building_type_e building_type)
{
    return x >= 0 &&
           y >= 0 &&
           x + ((SDL_Rect *)&building_renderer->sprite_tile_rects)[building_type].w <= building_renderer->map_renderer->size &&
           y + ((SDL_Rect *)&building_renderer->sprite_tile_rects)[building_type].h <= building_renderer->map_renderer->size;
}

/**
 * @brief Fait le rendu de d'un batiment sur la carte
 *
 * @param window un pointeur sur une fenêtre
 * @param building_renderer un pointeur sur la structure qui gère l'affichage des bâtiments
 * @param x position en x en nombre de case
 * @param y position en y en nombre de case
 * @param building_type type de batiment à afficher
 * @return **1** si on peut le placer, **0** sinon
 */
extern int renderBuilding(window_t *window, building_renderer_t *building_renderer, int x, int y, building_type_e building_type)
{
    if (!canRenderBuilding(building_renderer, x, y, building_type))
        return 0;

    int offset = building_renderer->map_renderer->tile_size * building_renderer->map_renderer->size / 2;
    SDL_Rect tile_rect = ((SDL_Rect *)&building_renderer->sprite_tile_rects)[building_type];
    SDL_Rect destination_rect = positionFromCenter(
        window,
        building_renderer->map_renderer->tile_size * tile_rect.w,
        building_renderer->map_renderer->tile_size * tile_rect.h,
        building_renderer->map_renderer->tile_size * x - offset,
        building_renderer->map_renderer->tile_size * y - offset,
        TRANSFORM_ORIGIN_TOP_LEFT);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        ((SDL_Rect *)&building_renderer->sprite_rects) + building_type,
        &destination_rect);

    return 1;
}

/**
 * @brief Détruit la structure qui gère l'affichage des bâtiments
 *
 * @param building_renderer une référence d'un pointeur sur la structure qui gère l'affichage des bâtiments
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteBuildingRenderer(building_renderer_t **building_renderer)
{
    if (building_renderer == NULL || *building_renderer == NULL)
        return -1;

    destroySprite(&(*building_renderer)->sprite);
    free(*building_renderer);
    *building_renderer = NULL;

    return 0;
}