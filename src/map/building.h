/**
 * @file building.h
 * @author Hôa Le Luet, Lucas Dureau
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
 */
typedef struct building_s
{
    building_type_e type; /**< type du bâtiment*/
    SDL_Point position;   /**< position du bâtiment en cases*/

    int hp;     /**< points de vie du bâtiment*/
    int max_hp; /**< points de vie maximum du bâtiment*/
} building_t;

/**
 * @brief Créer la structure qui gère les données des bâtiments
 *
 * @param window un pointeur sur une fenêtre
 * @return building_renderer_t*
 */
extern building_t *createBuilding(building_type_e type, SDL_Point *position, window_t *window);

/**
 * @brief Récupère le prix d'un bâtiment
 *
 * @param type le type de bâtiment
 * @return coût du bâtiment en or
 */
extern int getBuildingGoldCost(building_type_e type);

/**
 * @brief Créer la matrice de bâtiment
 *
 * @return retourne un pointeur sur la matrice
 */
extern building_t ***createBuildingMatrix();

/**
 * @brief Affiche la matrice de bâtiment
 *
 * @param window un pointeur sur une fenêtre
 * @param map_building matrice contenant la totalité des bâtiments placés sur la carte
 * @param building_renderer un pointeur sur la structure qui gère l'affichage des bâtiments
 */
extern void renderBuildingMatrix(window_t *window, building_t ***map_building, building_renderer_t *building_renderer);

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
 * @param building_matrix matrice contenant la totalité des bâtiments placés sur la carte
 */
extern void clearBuildingMatrix(building_t ***building_matrix);

/**
 * @brief Détruit la matrice de bâtiments
 *
 * @param building_matrix matrice contenant les bâtiments
 */
extern void destroyBuildingMatrix(building_t ****building_matrix);

/**
 * @brief Permet d'ajouter un bâtiment sur la carte
 *
 *
 * @param building_matrix matrice contenant la totalité des bâtiments placés sur la carte
 * @param building le bâtiment à ajouter à la carte
 */
extern void addBuildingInMatrix(building_t ***building_matrix, building_t *building);

/**
 * @brief Permet de supprimer un bâtiment de la carte
 *
 * @param building_matrix matrice contenant la totalité des bâtiments placés sur la carte
 * @param building le bâtiment à supprimer de la carte
 */
extern void removeBuildingFromMatrix(building_t ***building_matrix, building_t *building);

/**
 * @brief Permet de vérifier si un bâtiment peut être placé à une position donnée
 *
 * @param building_renderer un pointeur sur la structure qui gère l'affichage des bâtiments
 * @param type le type de bâtiment qui doit être placé
 * @param position la position à vérifier
 * @param building_matrix matrice contenant la totalité des bâtiments placés sur la carte
 * @return retourne 1 si le bâtiment peut être placé, 0 sinon
 */
extern int canPlaceBuilding(building_renderer_t *building_renderer, building_type_e type, SDL_Point *position, building_t ***building_matrix);

/**
 * @brief Récupère le bâtiment à une position donnée
 *
 * @param building_matrix matrice contenant les bâtiments
 * @param x position du bâtiment à retourner en x
 * @param y position du bâtiment à retourner en y
 * @return building_t* si le bâtiment existe, NULL sinon
 */
extern building_t *getBuilding(building_t ***building_matrix, int x, int y);

/**
 * @brief Récupère le bâtiment à une position donnée
 *
 * @param building_matrix matrice contenant les bâtiments
 * @param position position du bâtiment à retourner
 * @return building_t* si le bâtiment existe, NULL sinon
 */
extern building_t *getBuildingWithPoint(building_t ***building_matrix, SDL_Point *position);

/**
 * @brief Fonction d'écoute des événements du système de placement de bâtiment
 *
 * @param event un pointeur sur les données du jeu
 * @param game_data un pointeur sur les données du jeu
 * @param building_renderer un pointeur sur la structure qui gère l'affichage des bâtiments
 * @param window un pointeur sur une fenêtre
 */
extern void buildingEventHandler(SDL_Event *event, client_game_data_t *game_data, building_renderer_t *building_renderer, window_t *window);

#endif