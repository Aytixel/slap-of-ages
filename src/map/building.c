/**
 * @file building.c
 * @author Lucas Dureau
 * @brief Permet de gérer l'affichage des bâtiments
 * @version 0.1
 * @date 03/03/2023
 *
 */

#include <stdlib.h>
#include "map.h"
#include "building.h"

#define BUILDING_TILE_SIZE 16

/**
 * @brief Créer la structure qui gère l'affichage des bâtiments
 *
 * @param window un pointeur sur une fenêtre
 * @param map un pointeur sur la carte
 * @return building_t*
 */
extern building_t *createBuilding(window_t *window, map_t *map)
{
    building_t *building = malloc(sizeof(building_t));

    building->map = map;
    building->sprite = loadSprite(window, "asset/sprite/building/MasterSimple.png");

    if (building->sprite == NULL)
    {
        free(building);

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

    building->sprite_tile_rects = sprite_tile_rects;

    // conversion de la position et la taille des sprites en pixel
    for (int i = 0; i < sizeof(building_sprite_rects_t) / sizeof(int); i++)
    {
        ((int *)&building->sprite_rects)[i] = ((int *)&building->sprite_tile_rects)[i] * BUILDING_TILE_SIZE;
    }

    return building;
}

/**
 * @brief Détermine si on peut placer / afficher un certain type de bâtiment à tel endroit
 *
 * @param building un pointeur sur la structure qui gère l'affichage des bâtiments
 * @param x position en x en nombre de case
 * @param y position en y en nombre de case
 * @param building_type type de batiment à placer / afficher
 * @return **1** si on peut le placer, **0** sinon
 */
extern int canPlaceBuilding(building_t *building, int x, int y, building_type_e building_type)
{
    return x >= 0 &&
           y >= 0 &&
           x + ((SDL_Rect *)&building->sprite_tile_rects)[building_type].w <= building->map->size &&
           y + ((SDL_Rect *)&building->sprite_tile_rects)[building_type].h <= building->map->size;
}

/**
 * @brief Fait le rendu de d'un batiment sur la carte
 *
 * @param window un pointeur sur une fenêtre
 * @param building un pointeur sur la structure qui gère l'affichage des bâtiments
 * @param x position en x en nombre de case
 * @param y position en y en nombre de case
 * @param building_type type de batiment à afficher
 * @return **1** si on peut le placer, **0** sinon
 */
extern int renderBuilding(window_t *window, building_t *building, int x, int y, building_type_e building_type)
{
    if (!canPlaceBuilding(building, x, y, building_type))
        return 0;

    int offset = building->map->tile_size * building->map->size / 2;
    SDL_Rect tile_rect = ((SDL_Rect *)&building->sprite_tile_rects)[building_type];
    SDL_Rect destination_rect = positionFromCenter(
        window,
        building->map->tile_size * tile_rect.w,
        building->map->tile_size * tile_rect.h,
        building->map->tile_size * x - offset,
        building->map->tile_size * y - offset,
        TRANSFORM_ORIGIN_TOP_LEFT);

    SDL_RenderCopy(
        window->renderer,
        building->sprite->texture,
        ((SDL_Rect *)&building->sprite_rects) + building_type,
        &destination_rect);

    return 1;
}

/**
 * @brief Détruit la structure qui gère l'affichage des bâtiments
 *
 * @param building une référence d'un pointeur sur la structure qui gère l'affichage des bâtiments
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteBuilding(building_t **building)
{
    if (building == NULL || *building == NULL)
        return -1;

    destroySprite(&(*building)->sprite);
    free(*building);
    *building = NULL;

    return 0;
}