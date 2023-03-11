#include "building_renderer.h"

/**
 * @file building.h
 * @author Hôa Le Luet
 * @brief
 * @version 0.1
 * @date 2023-03-09
 *
 */

/**
 * @brief Structure contenant les données des bâtiments
 *
 * @param type type du bâtiment
 * @param sprite position et taille du sprite du bâtiment
 * @param rect position et taille du bâtiment en pixel
 * @param position position du bâtiment en cases
 * @param hp points de vie du bâtiment
 * @param max_hp points de vie maximum du bâtiment
 */

typedef struct
{
    building_type_e type;
    building_renderer_t *building_renderer;
    SDL_Rect rect;
    SDL_Point *position;

    int hp;
    int max_hp;

} building_t;

/*Fonctions externes*/

extern building_t *createBuilding(building_type_e type, SDL_Point *position, window_t *window, map_renderer_t *map_renderer);

extern void destroyBuilding(building_t **building);

extern void buildingTakesDamages(building_t *building, int damages);