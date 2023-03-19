/**
 * @file building.h
 * @author Hôa Le Luet
 * @brief
 * @version 0.1
 * @date 2023-03-09
 *
 */

#ifndef __BUILDING_H
#define __BUILDING_H

#include "building_renderer.h"
#include "map/map_renderer.h"

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
    SDL_Point position;

    int hp;
    int max_hp;

} building_t;

/*Fonctions externes*/

/**
 * @brief Créer la structure qui gère l'affichage des bâtiments
 *
 * @param window un pointeur sur une fenêtre
 * @param map_renderer un pointeur sur la carte
 * @return building_renderer_t*
 */
extern building_t *createBuilding(building_type_e type, SDL_Point *position, window_t *window, map_renderer_t *map_renderer);

/**
 * @brief Détruit la structure de bâtiment
 *
 * @param building un pointeur sur un pointeur sur un buiding_t
 */
extern void destroyBuilding(building_t **building);

/**
 * @brief Permet de gérer les dégâts subis par un bâtiment
 *
 * @param building un pointeur sur un bâtiment
 * @param damages les dégâts subis par le bâtiment
 */
extern void buildingTakesDamages(building_t *building, int damages);

/**
 * @brief Permet de gérer la position du batiment selon le placement de la souris
 *
 * @param mouse_position la position de la souris
 * @param window un pointeur sur une fenêtre
 * @param map un pointeur sur la structure de rendu de la carte
 * @return SDL_Point contenant les coordonnées du bâtiment en cases, si le clique de la souris est en dehors de la carte, la valeur retournée est {-1, -1}.
 */

extern SDL_Point getTileCoord(SDL_Point *mouse_position, window_t *window, map_renderer_t *map);

/**
 * @brief Permet de détruire tout les bâtiments sur la carte
 *
 * @param building_matrix Matrice contenant la totalité des bâtiments placés sur la carte
 * @param map_size Taille de la carte en nombre de cases
 */

extern void clearMatrix(building_t ***building_matrix, int map_size);

/**
 * @brief Permet d'ajouter un bâtiment sur la carte
 *
 *
 * @param building_matrix Matrice contenant la totalité des bâtiments placés sur la carte
 * @param building Le bâtiment à ajouter à la carte
 */

extern void addBuildingInMatrix(building_t ***building_matrix, building_t *building);

/**
 * @brief Permet de supprimer un bâtiment de la carte
 *
 * @param building_matrix Matrice contenant la totalité des bâtiments placés sur la carte
 * @param building Le bâtiment à supprimer de la carte
 */

extern void removeBuildingFromMatrix(building_t ***building_matrix, building_t *building);

/**
 * @brief Permet de mettre à jour la position d'un bâtiment
 *
 * @param building Le bâtiment à mettre à jour
 * @param position La nouvelle position du bâtiment
 */

extern void updateBuildingCoord(building_t *building, SDL_Point *position);

#endif