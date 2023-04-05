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
    SDL_Point position;    /**< Coordonnées du noeud*/
    float g_cost;          /**< Coût du noeud*/
    float h_cost;          /**< Coût heuristique du noeud*/
    float f_cost;          /**< Coût total du noeud*/
    struct node_s *parent; /**< Pointeur vers le noeud parent*/
} node_t;

/**
 * @brief Structure de liste de noeud
 *
 */
typedef struct
{
    node_t **nodes; /**< Tableau de pointeur vers les noeuds*/
    int size;       /**< Taille de la liste*/
    int capacity;   /**< Capacité de la liste*/
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
 * @param list
 */
extern void clearNodeList(node_list_t *list);

/**
 * @brief Libération de la mémoire d'une liste de noeud
 *
 * @param list
 */
extern void freeNodeList(node_list_t *list);

/**
 * @brief Suppression de la liste d'un noeud et de ses parents
 *
 * @param list
 * @param node
 */
void removeParentFromList(node_list_t *list, node_t *node);

/**
 * @brief Ajout d'un noeud à la liste
 *
 * @param list
 * @param node
 */
extern void addNode(node_list_t *list, node_t *node);

/**
 * @brief Suppression d'un noeud
 *
 * @param list
 * @param index
 * @return node*
 */
extern node_t *removeNode(node_list_t *list, int index);

/**
 * @brief Creation d'une noeud
 *
 * @param position
 * @param parent
 * @return node*
 */
extern node_t *createNode(SDL_Point position, node_t *parent);

/**
 * @brief Libération de la mémoire d'une node et de ses parent
 *
 * @param node
 */
extern void freeNodePath(node_t *node);

/**
 * @brief Heuristique pour l'algorithme A*
 *
 * @param a
 * @param b
 * @return float
 */
extern float heuristic(node_t *a, node_t *b);

/**
 * @brief Vérification de la validité d'un node sans les murs
 *
 * @param x
 * @param y
 * @return true
 * @return false
 */
extern bool isValidNoWall(SDL_Point position);

/**
 * @brief Vérification de la validité d'un node
 *
 * @param x
 * @param y
 * @param mat
 * @return true
 * @return false
 */
extern bool isValid(SDL_Point position, building_t ***map_building);

/**
 * @brief Algorithme de pathfinding A*
 *
 * @param start
 * @param goal
 * @param map_building
 * @param wall
 * @return node*
 */
extern node_t *aStar(SDL_Point start, SDL_Point goal, building_t ***map_building, int wall);

/**
 * @brief Va à la prochaine position dans le chemin en enlevant le noeud courant
 *
 * @param node
 */
extern void gotoNextPositionInPath(node_t **node);

/**
 * @brief Récupère la position vers la quelle avancer
 *
 * @param node
 * @return SDL_Point
 */
extern SDL_Point getNextPositionInPath(node_t *node);

/**
 * @brief Affichage du chemin
 *
 * @param node
 */
extern void displayPath(node_t *node);

#endif