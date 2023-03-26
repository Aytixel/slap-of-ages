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

#define MAT_SIZE 10
#define OBSTACLE -1
#define START 1
#define GOAL 2

/**
 * @brief Structure de noeud
 *
 * @param x Coordonnée x du node
 * @param y Coordonnée y du noeud
 * @param g_cost Coût du noeud
 * @param h_cost Coût heuristique du noeud
 * @param f_cost Coût total du noeud
 * @param parent Pointeur vers le noeud parent
 */
typedef struct node_s
{
    int x;
    int y;
    int g_cost;
    int h_cost;
    int f_cost;
    struct node_s *parent;
} node_t;

/**
 * @brief Structure de liste de noeud
 *
 * @param nodes Tableau de pointeur vers les noeuds
 * @param size Taille de la liste
 * @param capacity Capacité de la liste
 */
typedef struct
{
    node_t **nodes;
    int size;
    int capacity;
} node_list_t;

/**
 * @brief Creation d'une liste de noeud
 *
 * @return node_list*
 */
extern node_list_t *create_node_list();

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
 * @brief Heuristique de Manhattan pour l'algorithme A*
 *
 * @param a
 * @param b
 * @return int
 */
extern int heuristic(node_t *a, node_t *b);

/**
 * @brief Vérification de la validité d'un node
 *
 * @param x
 * @param y
 * @param mat
 * @return true
 * @return false
 */
extern bool is_valid(int x, int y, int mat[][MAT_SIZE]);

/**
 * @brief Algorithme de pathfinding A*
 *
 * @param start_x
 * @param start_y
 * @param goal_x
 * @param goal_y
 * @param mat
 * @return node*
 */
extern node_t *a_star(int start_x, int start_y, int goal_x, int goal_y, int mat[][MAT_SIZE]);

/**
 * @brief Vérification de la validité d'un node sans les murs
 *
 * @param x
 * @param y
 * @return true
 * @return false
 */
extern bool is_valid_no_wall(int x, int y);

/**
 * @brief Algorithme de pathfinding A* sans les murs
 *
 * @param start_x
 * @param start_y
 * @param goal_x
 * @param goal_y
 * @param mat
 * @return node*
 */
node_t *a_star_no_wall(int start_x, int start_y, int goal_x, int goal_y, int mat[][MAT_SIZE]);

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
 * @param mat
 */
extern void fill_path_in_mat(node_t *node, int mat[][MAT_SIZE]);

/**
 * @brief Affichage de la matrice
 *
 * @param mat
 */
extern void display_mat(int mat[][MAT_SIZE]);

#endif
