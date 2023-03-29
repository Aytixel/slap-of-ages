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

typedef struct client_game_data_s client_game_data_t;

#include "building_renderer.h"
#include "map/map_renderer.h"
#include "client/game_data.h"

/**
 * @brief Structure contenant les données des bâtiments
 *
 * @param type type du bâtiment
 * @param sprite position et taille du sprite du bâtiment
 * @param rect position et taille du bâtiment en pixel
 * @param position position du bâtiment en cases
 * @param hp points de vie du bâtiment
 * @param max_hp points de vie maximum du bâtiment
 * @param gold_cost coût du bâtiment en or
 */

typedef struct building_s
{
    building_type_e type;
    SDL_Rect rect;
    SDL_Point position;

    int hp;
    int max_hp;
    int gold_cost;
} building_t;

/*Fonctions externes*/

/**
 * @brief Créer la structure qui gère l'affichage des bâtiments
 *
 * @param window un pointeur sur une fenêtre
 * @return building_renderer_t*
 */
extern building_t *createBuilding(building_type_e type, SDL_Point *position, window_t *window);

/**
 * @brief Créer la matrice de bâtiment
 *
 * @param map_size taille de la carte
 * @return retourne un pointeur sur la matrice
 */
extern building_t ***createBuildingMatrix(int map_size);

/**
 * @brief Affiche la matrice de bâtiment
 *
 * @param window un pointeur sur une fenêtre
 * @param map_building matrice contenant la totalité des bâtiments placés sur la carte
 * @param building_renderer
 * @param map_size taille de la carte
 */
extern void renderBuildingMatrix(window_t *window, building_t ***map_building, building_renderer_t *building_renderer, int map_size);

/**
 * @brief Détruit la structure de bâtiment
 *
 * @param building un pointeur sur un pointeur sur un buiding_t
 */
extern void destroyBuilding(building_t **building);

/**
 * @brief Permet de gérer les dégâts subis par un bâtiment
 *
 * @param matrix matrice contenant la totalité des bâtiments placés sur la carte
 * @param building un pointeur sur un bâtiment
 * @param damages les dégâts subis par le bâtiment
 */
extern void buildingTakesDamages(building_t ***matrix, building_t *building, int damages);

/**
 * @brief Permet de détruire tout les bâtiments sur la carte
 *
 * @param building_matrix Matrice contenant la totalité des bâtiments placés sur la carte
 * @param map_size Taille de la carte en nombre de cases
 */
extern void clearMatrix(building_t ***building_matrix, int map_size);

/**
 * @brief Détruit la matrice de bâtiments
 *
 * @param building_matrix matrice contenant les bâtiments
 * @param map_size taille de la carte
 */
extern void destroyBuildingMatrix(building_t ****building_matrix, int map_size);

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

/**
 * @brief Permet de vérifier si un bâtiment peut être placé à une position donnée
 *
 * @param building Le bâtiment qui doit être placé
 * @param position La position à vérifier
 * @param building_matrix Matrice contenant la totalité des bâtiments placés sur la carte
 * @return retourne 1 si le bâtiment peut être placé, 0 sinon
 */
extern int canPlaceBuilding(building_renderer_t *building_renderer, building_t *building, SDL_Point *position, building_t ***building_matrix);

/**
 * @brief Récupère le bâtiment à une position donnée
 *
 * @param building_matrix matrice contenant les bâtiments
 * @param position position du bätiment à retourner
 * @return building_t* si le bâtiment existe, NULL sinon
 */
extern building_t *getBuilding(building_t ***building_matrix, SDL_Point *position);

/**
 * @brief Fonction d'écoute des événements du système de placement de bâtiment
 *
 * @param event un pointeur sur les événements
 * @param game_data un pointeur sur les données du jeux
 * @param building_renderer un pointeur sur la structure qui gère l'affichage des bâtiments
 * @param window un pointeur sur une fenêtre
 */
extern void buildingEventHandler(SDL_Event *event, client_game_data_t *game_data, building_renderer_t *building_renderer, window_t *window);

#endif