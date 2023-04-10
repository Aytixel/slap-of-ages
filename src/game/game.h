/**
 * @file game.h
 * @author Arthur Dureau, Hôa Le Luet, Lucas Dureau
 * @brief Fonction divers pour le jeu
 * @version 1.0
 * @date 19/03/2023
 *
 *
 */

#ifndef __GAME_H
#define __GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "building/building.h"

#define OBSTACLE -1
#define START 1
#define GOAL 2

/**
 * @brief Structure de noeud
 *
 */
typedef struct node_s
{
    SDL_Point position;    /**< coordonnées du noeud*/
    float g_cost;          /**< coût du noeud*/
    float h_cost;          /**< coût heuristique du noeud*/
    float f_cost;          /**< coût total du noeud*/
    struct node_s *parent; /**< pointeur vers le noeud parent*/
} node_t;

/**
 * @brief Structure de liste de noeud
 *
 */
typedef struct
{
    node_t **nodes; /**< tableau de pointeur vers les noeuds*/
    int size;       /**< taille de la liste*/
    int capacity;   /**< capacité de la liste*/
} node_list_t;

/**
 * @brief Creation d'une liste de noeud
 *
 * @return node_list*
 */
extern node_list_t *createNodeList();

/**
 * @brief Vide une liste de ses noeuds
 *
 * @param list pointeur sur une liste de noeud
 */
extern void clearNodeList(node_list_t *list);

/**
 * @brief Libération de la mémoire d'une liste de noeud
 *
 * @param list pointeur sur une liste de noeud
 */
extern void freeNodeList(node_list_t *list);

/**
 * @brief Suppression de la liste d'un noeud et de ses parents
 *
 * @param list pointeur sur une liste de noeud
 * @param node pointeur sur un noeud
 */
void removeParentFromList(node_list_t *list, node_t *node);

/**
 * @brief Ajout d'un noeud à la liste
 *
 * @param list pointeur sur une liste de noeud
 * @param node pointeur sur un noeud
 */
extern void addNode(node_list_t *list, node_t *node);

/**
 * @brief Suppression d'un noeud
 *
 * @param list pointeur sur une liste de noeud
 * @param index position du noeud dans la liste
 * @return node*
 */
extern node_t *removeNode(node_list_t *list, int index);

/**
 * @brief Creation d'une noeud
 *
 * @param position position du noeud
 * @param parent pointeur sur le noeud parent
 * @return node*
 */
extern node_t *createNode(SDL_Point position, node_t *parent);

/**
 * @brief Libération de la mémoire d'une node et de ses parent
 *
 * @param node pointeur sur un noeud
 */
extern void freeNodePath(node_t *node);

/**
 * @brief Heuristique pour l'algorithme A*
 *
 * @param a pointeur sur un noeud
 * @param b pointeur sur un noeud
 * @return float
 */
extern float heuristic(node_t *a, node_t *b);

/**
 * @brief Vérification de la validité d'un node sans les murs
 *
 * @param position position du noeud
 * @return true ou false
 */
extern bool isValidNoWall(SDL_Point position);

/**
 * @brief Vérification de la validité d'un node
 *
 * @param position position du noeud
 * @param map_building matrice contenant les bâtiments
 * @return true ou false
 */
extern bool isValid(SDL_Point position, building_t ***map_building);

/**
 * @brief Algorithme de pathfinding A*
 *
 * @param start position du noeud de départ
 * @param goal position du noeud d'arriver
 * @param map_building matrice contenant les bâtiments
 * @param wall indique si l'on doit prendre en compte les murs ou non
 * @return node*
 */
extern node_t *aStar(SDL_Point start, SDL_Point goal, building_t ***map_building, int wall);

/**
 * @brief Va à la prochaine position dans le chemin en enlevant le noeud courant
 *
 * @param node pointeur sur un noeud
 */
extern void gotoNextPositionInPath(node_t **node);

/**
 * @brief Récupère la position vers la quelle avancer
 *
 * @param node pointeur sur un noeud
 * @return SDL_Point
 */
extern SDL_Point getNextPositionInPath(node_t *node);

/**
 * @brief Affichage du chemin
 *
 * @param node pointeur sur un noeud
 */
extern void displayPath(node_t *node);

#endif