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
 * @param x Coordonnée x du noeud
 * @param y Coordonnée y du noeud
 * @param g_cost Coût du noeud
 * @param h_cost Coût heuristique du noeud
 * @param f_cost Coût total du noeud
 * @param parent Pointeur vers le noeud parent
 * 
 */

typedef struct Noeud_s{
    int x;
    int y;
    int g_cost;
    int h_cost;
    int f_cost;
    struct Noeud_s *parent;
} Noeud_t;

/**
 * @brief Structure de liste de noeud
 * 
 * @param noeuds Tableau de pointeur vers les noeuds
 * @param size Taille de la liste
 * @param capacity Capacité de la liste
 * 
 */

typedef struct {
    Noeud_t **noeuds;
    int size;
    int capacity;
} NoeudList_t;

/**
 * @brief Creation d'une liste de noeud
 * 
 * @return NoeudList* 
 */
extern NoeudList_t *create_noeudlist() ;

/**
 * @brief Libération de la mémoire d'une liste de noeud
 * 
 * @param list 
 */
extern void free_noeudlist(NoeudList_t *list);

/**
 * @brief Ajout d'un noeud à la liste
 * 
 * @param list 
 * @param noeud 
 */
extern void add_noeud(NoeudList_t *list, Noeud_t *noeud);

/**
 * @brief Suppression d'un noeud
 * 
 * @param list 
 * @param index 
 * @return Noeud* 
 */
extern Noeud_t *remove_noeud(NoeudList_t *list, int index);

/**
 * @brief Creation d'un noeud
 * 
 * @param x 
 * @param y 
 * @param parent 
 * @return Noeud* 
 */
extern Noeud_t *create_noeud(int x, int y, Noeud_t *parent) ;

/**
 * @brief Heuristique de Manhattan pour l'algorithme A*
 * 
 * @param a 
 * @param b 
 * @return int 
 */
extern int heuristic(Noeud_t *a, Noeud_t *b) ;

/**
 * @brief Vérification de la validité d'un noeud
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
 * @return Noeud* 
 */
extern Noeud_t *a_star(int start_x, int start_y, int goal_x, int goal_y, int mat[][MAT_SIZE]);

/**
 * @brief Vérification de la validité d'un noeud sans les murs
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
 * @return Noeud* 
 */
Noeud_t *a_star_no_wall(int start_x, int start_y, int goal_x, int goal_y, int mat[][MAT_SIZE]);

/**
 * @brief Affichage du chemin
 * 
 * @param noeud 
 */
extern void affichage_chemin(Noeud_t *noeud);

/**
 * @brief Remplissage de la matrice avec le chemin
 * 
 * @param noeud 
 * @param mat 
 */
extern void fill_path_in_mat(Noeud_t *noeud, int mat[][MAT_SIZE]);

/**
 * @brief Affichage de la matrice
 * 
 * @param mat 
 */
extern void affichage_matrice(int mat[][MAT_SIZE]);

#endif

