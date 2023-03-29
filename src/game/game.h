/**
 * @file game.h
 * @author Arthur Dureau
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

#define OBSTACLE -1
#define START 1
#define GOAL 2

/**
 * @brief Structure de noeud
 *
 */
typedef struct node_s
{
    int x;                 /**< Coordonnée x du node*/
    int y;                 /**< Coordonnée y du noeud*/
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
extern node_list_t *create_node_list(int map_size);

/**
 * @brief Vide une liste de ses noeuds
 *
 * @param list
 */
extern void clear_node_list(node_list_t *list);

/**
 * @brief Libération de la mémoire d'une liste de noeud
 *
 * @param list
 */
extern void free_node_list(node_list_t *list);

/**
 * @brief Suppression de la liste d'un noeud et de ses parents
 *
 * @param list
 * @param node
 */
void remove_parent_from_list(node_list_t *list, node_t *node);

/**
 * @brief Ajout d'un noeud à la liste
 *
 * @param list
 * @param node
 */
extern void add_node(node_list_t *list, node_t *node);

/**
 * @brief Suppression d'un noeud
 *
 * @param list
 * @param index
 * @return node*
 */
extern node_t *remove_node(node_list_t *list, int index);

/**
 * @brief Creation d'une noeud
 *
 * @param x
 * @param y
 * @param parent
 * @return node*
 */
extern node_t *create_node(int x, int y, node_t *parent);

/**
 * @brief Libération de la mémoire d'une node et de ses parent
 *
 * @param node
 */
extern void free_node_path(node_t *node);

/**
 * @brief Heuristique pour l'algorithme A*
 *
 * @param a
 * @param b
 * @return float
 */
extern float heuristic(node_t *a, node_t *b);

/**
 * @brief Vérification de la validité d'un node
 *
 * @param x
 * @param y
 * @param map_size
 * @param mat
 * @return true
 * @return false
 */
extern bool is_valid(int x, int y, int map_size, int mat[][map_size]);

/**
 * @brief Vérification de la validité d'un node sans les murs
 *
 * @param x
 * @param y
 * @param map_size
 * @return true
 * @return false
 */
extern bool is_valid_no_wall(int x, int y, int map_size);

/**
 * @brief Algorithme de pathfinding A*
 *
 * @param start_x
 * @param start_y
 * @param goal_x
 * @param goal_y
 * @param mat
 * @param wall
 * @return node*
 */
extern node_t *a_star(int start_x, int start_y, int goal_x, int goal_y, int map_size, int mat[][map_size], int wall);

/**
 * @brief Affichage du chemin
 *
 * @param node
 */
extern void display_path(node_t *node);

/**
 * @brief Remplissage de la matrice avec le chemin
 *
 * @param node
 * @param map_size
 * @param mat
 */
extern void fill_path_in_mat(node_t *node, int map_size, int mat[][map_size]);

/**
 * @brief Affichage de la matrice
 *
 * @param map_size
 * @param mat
 */
extern void display_mat(int map_size, int mat[][map_size]);

#endif