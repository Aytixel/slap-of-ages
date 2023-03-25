#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"

#define MAT_SIZE 10
#define OBSTACLE -1
#define START 1
#define GOAL 2

NoeudList_t *create_noeudlist() {
    NoeudList_t *list = (NoeudList_t *)malloc(sizeof(NoeudList_t));
    list->noeuds = (Noeud_t **)malloc(MAT_SIZE * sizeof(Noeud_t *));
    list->size = 0;
    list->capacity = MAT_SIZE;
    return list;
}

void free_noeudlist(NoeudList_t *list) {
    free(list->noeuds);
    free(list);
}

void add_noeud(NoeudList_t *list, Noeud_t *noeud) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->noeuds = (Noeud_t **)realloc(list->noeuds, list->capacity * sizeof(Noeud_t *));
    }
    list->noeuds[list->size++] = noeud;
}

Noeud_t *remove_noeud(NoeudList_t *list, int index) {
    Noeud_t *noeud = list->noeuds[index];
    list->noeuds[index] = list->noeuds[--list->size];
    return noeud;
}

Noeud_t *create_noeud(int x, int y, Noeud_t *parent) {
    Noeud_t *new_noeud = (Noeud_t *)malloc(sizeof(Noeud_t));
    new_noeud->x = x;
    new_noeud->y = y;
    new_noeud->g_cost = 0;
    new_noeud->h_cost = 0;
    new_noeud->f_cost = 0;
    new_noeud->parent = parent;
    return new_noeud;
}

int heuristic(Noeud_t *a, Noeud_t *b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

bool is_valid(int x, int y, int mat[][MAT_SIZE]) {
    return x >= 0 && x < MAT_SIZE && y >= 0 && y < MAT_SIZE && mat[y][x] != OBSTACLE;
}

Noeud_t *a_star(int start_x, int start_y, int goal_x, int goal_y, int mat[][MAT_SIZE]) {
    Noeud_t *start_noeud = create_noeud(start_x, start_y, NULL);
    Noeud_t *goal_noeud = create_noeud(goal_x, goal_y, NULL);

    // Liste des noeuds à explorer
    NoeudList_t *open_list = create_noeudlist();
    add_noeud(open_list, start_noeud);

    // Liste des noeuds déjà explorés
    int closed_list[MAT_SIZE][MAT_SIZE] = {0};

    // Tableau des déplacements possibles
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    // Tant qu'il reste des noeuds à explorer
    while (open_list->size > 0) {
        Noeud_t *current_noeud = open_list->noeuds[0];
        int current_index = 0;

        // On cherche le noeud avec le coût le plus faible
        for (int i = 1; i < open_list->size; i++) {
            if (open_list->noeuds[i]->f_cost < current_noeud->f_cost) {
                current_noeud = open_list->noeuds[i];
                current_index = i;
            }
        }

        // Si on a atteint le noeud final, on retourne le noeud courant
         if (current_noeud->x == goal_noeud->x && current_noeud->y == goal_noeud->y) {
            free_noeudlist(open_list);
            free(goal_noeud);
            return current_noeud;
        }

        // On retire le noeud de la liste des noeuds à explorer et on l'ajoute à la liste des noeuds déjà explorés
        remove_noeud(open_list, current_index);
        closed_list[current_noeud->y][current_noeud->x] = 1;

        // On explore les voisins du noeud courant
        for (int i = 0; i < 4; i++) {
            int new_x = current_noeud->x + dx[i];
            int new_y = current_noeud->y + dy[i];

            // Si le voisin est invalide ou qu'il a déjà été exploré, on passe au voisin suivant
            if (!is_valid(new_x, new_y, mat) || closed_list[new_y][new_x]) {
                continue;
            }

            // Calcule du coût du voisin
            Noeud_t *neighbor = create_noeud(new_x, new_y, current_noeud); 
            neighbor->g_cost = current_noeud->g_cost + 1;
            neighbor->h_cost = heuristic(neighbor, goal_noeud);
            neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;

            bool in_open_list = false;

            // Parcours de la liste des noeuds, vérifie si les coordonnées du voisin sont déjà présentes dans la liste et vérifie si le coût du voisin est inférieur ou égal au coût du noeud déjà présent dans la liste
            for (int j = 0; j < open_list->size; j++) {
                if (open_list->noeuds[j]->x == new_x && open_list->noeuds[j]->y == new_y && open_list->noeuds[j]->g_cost <= neighbor->g_cost) {
                    in_open_list = true;
                    free(neighbor);
                    break;
                }
            }

            // Sinon, on l'ajoute à la liste des noeuds à explorer
            if (!in_open_list) {
                add_noeud(open_list, neighbor);
            }
        }
    }

    free_noeudlist(open_list);
    free(goal_noeud);
    return NULL;
}

bool is_valid_no_wall(int x, int y) {
    return x >= 0 && x < MAT_SIZE && y >= 0 && y < MAT_SIZE;
}

Noeud_t *a_star_no_wall(int start_x, int start_y, int goal_x, int goal_y, int mat[][MAT_SIZE]) {
    Noeud_t *start_noeud = create_noeud(start_x, start_y, NULL);
    Noeud_t *goal_noeud = create_noeud(goal_x, goal_y, NULL);

    NoeudList_t *open_list = create_noeudlist();
    add_noeud(open_list, start_noeud);

    int closed_list[MAT_SIZE][MAT_SIZE] = {0};

    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    while (open_list->size > 0) {
        Noeud_t *current_noeud = open_list->noeuds[0];
        int current_index = 0;

        for (int i = 1; i < open_list->size; i++) {
            if (open_list->noeuds[i]->f_cost < current_noeud->f_cost) {
                current_noeud = open_list->noeuds[i];
                current_index = i;
            }
        }

        if (current_noeud->x == goal_noeud->x && current_noeud->y == goal_noeud->y) {
            free_noeudlist(open_list);
            free(goal_noeud);
            return current_noeud;
        }

        remove_noeud(open_list, current_index);
        closed_list[current_noeud->y][current_noeud->x] = 1;

        for (int i = 0; i < 4; i++) {
            int new_x = current_noeud->x + dx[i];
            int new_y = current_noeud->y + dy[i];

            if (!is_valid_no_wall(new_x, new_y) || closed_list[new_y][new_x]) {
                continue;
            }

            Noeud_t *neighbor = create_noeud(new_x, new_y, current_noeud);
            neighbor->g_cost = current_noeud->g_cost + 1;
            neighbor->h_cost = heuristic(neighbor, goal_noeud);
            neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;

            bool in_open_list = false;
            for (int j = 0; j < open_list->size; j++) {
                if (open_list->noeuds[j]->x == new_x && open_list->noeuds[j]->y == new_y && open_list->noeuds[j]->g_cost <= neighbor->g_cost) {
                    in_open_list = true;
                    free(neighbor);
                    break;
                }
            }

            if (!in_open_list) {
                add_noeud(open_list, neighbor);
            }
        }
    }

    free_noeudlist(open_list);
    free(goal_noeud);
    return NULL;
}


// Affichage du chemin en coordonnées
void affichage_chemin(Noeud_t *noeud) {
    if (noeud->parent != NULL) {
        affichage_chemin(noeud->parent);
        printf("-> (%d, %d) ", noeud->x, noeud->y);
    } else {
        printf("(%d, %d) ", noeud->x, noeud->y);
    }
}

// Affichage du chemin avec des 5 dans la matrice
void fill_path_in_mat(Noeud_t *noeud, int mat[][MAT_SIZE]) {
    if (noeud->parent != NULL) {
        fill_path_in_mat(noeud->parent, mat);
    }
    mat[noeud->y][noeud->x] = 5;
}

// Affichage de la matrice
void affichage_matrice(int mat[][MAT_SIZE]) {
    for (int y = 0; y < MAT_SIZE; y++) {
        for (int x = 0; x < MAT_SIZE; x++) {
            printf("%2d ", mat[y][x]);
        }
        printf("\n");
    }
}