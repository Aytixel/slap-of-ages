/**
 * @file building.c
 * @author Lucas Dureau
 * @brief Permet de gérer l'affichage des bâtiments
 * @version 0.1
 * @date 03/03/2023
 *
 */

#include <stdlib.h>
#include "building.h"

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

    return building;
}

/**
 * @brief Fait le rendu de d'un batiment sur la carte
 *
 * @param window un pointeur sur une fenêtre
 * @param building un pointeur sur la structure qui gère l'affichage des bâtiments
 * @param x position en x
 * @param y position en y
 * @param building_type type de batiment à afficher
 */
extern void renderBuilding(window_t *window, building_t *building, int x, int y, building_type_e building_type)
{
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

    free(*building);
    *building = NULL;

    return 0;
}