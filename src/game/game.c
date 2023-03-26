#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"

#define MAT_SIZE 10
#define OBSTACLE -1
#define START 1
#define GOAL 2

node_list_t *create_node_list()
{
    node_list_t *list = (node_list_t *)malloc(sizeof(node_list_t));
    list->nodes = (node_t **)malloc(MAT_SIZE * sizeof(node_t *));
    list->size = 0;
    list->capacity = MAT_SIZE;
    return list;
}

void free_node_list(node_list_t *list)
{
    free(list->nodes);
    free(list);
}

void add_node(node_list_t *list, node_t *node)
{
    if (list->size == list->capacity)
    {
        list->capacity *= 2;
        list->nodes = (node_t **)realloc(list->nodes, list->capacity * sizeof(node_t *));
    }
    list->nodes[list->size++] = node;
}

node_t *remove_node(node_list_t *list, int index)
{
    node_t *node = list->nodes[index];
    list->nodes[index] = list->nodes[--list->size];
    return node;
}

node_t *create_node(int x, int y, node_t *parent)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->x = x;
    new_node->y = y;
    new_node->g_cost = 0;
    new_node->h_cost = 0;
    new_node->f_cost = 0;
    new_node->parent = parent;
    return new_node;
}

int heuristic(node_t *a, node_t *b)
{
    return abs(a->x - b->x) + abs(a->y - b->y);
}

bool is_valid(int x, int y, int mat[][MAT_SIZE])
{
    return x >= 0 && x < MAT_SIZE && y >= 0 && y < MAT_SIZE && mat[y][x] != OBSTACLE;
}

node_t *a_star(int start_x, int start_y, int goal_x, int goal_y, int mat[][MAT_SIZE])
{
    node_t *start_node = create_node(start_x, start_y, NULL);
    node_t *goal_node = create_node(goal_x, goal_y, NULL);

    // Liste des nodes à explorer
    node_list_t *open_list = create_node_list();
    add_node(open_list, start_node);

    // Liste des nodes déjà explorés
    int closed_list[MAT_SIZE][MAT_SIZE] = {0};

    // Tableau des déplacements possibles
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    // Tant qu'il reste des nodes à explorer
    while (open_list->size > 0)
    {
        node_t *current_node = open_list->nodes[0];
        int current_index = 0;

        // On cherche le node avec le coût le plus faible
        for (int i = 1; i < open_list->size; i++)
        {
            if (open_list->nodes[i]->f_cost < current_node->f_cost)
            {
                current_node = open_list->nodes[i];
                current_index = i;
            }
        }

        // Si on a atteint le node final, on retourne le node courant
        if (current_node->x == goal_node->x && current_node->y == goal_node->y)
        {
            free_node_list(open_list);
            free(goal_node);
            return current_node;
        }

        // On retire le node de la liste des nodes à explorer et on l'ajoute à la liste des nodes déjà explorés
        remove_node(open_list, current_index);
        closed_list[current_node->y][current_node->x] = 1;

        // On explore les voisins du node courant
        for (int i = 0; i < 4; i++)
        {
            int new_x = current_node->x + dx[i];
            int new_y = current_node->y + dy[i];

            // Si le voisin est invalide ou qu'il a déjà été exploré, on passe au voisin suivant
            if (!is_valid(new_x, new_y, mat) || closed_list[new_y][new_x])
            {
                continue;
            }

            // Calcule du coût du voisin
            node_t *neighbor = create_node(new_x, new_y, current_node);
            neighbor->g_cost = current_node->g_cost + 1;
            neighbor->h_cost = heuristic(neighbor, goal_node);
            neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;

            bool in_open_list = false;

            // Parcours de la liste des nodes, vérifie si les coordonnées du voisin sont déjà présentes dans la liste et vérifie si le coût du voisin est inférieur ou égal au coût du node déjà présent dans la liste
            for (int j = 0; j < open_list->size; j++)
            {
                if (open_list->nodes[j]->x == new_x && open_list->nodes[j]->y == new_y && open_list->nodes[j]->g_cost <= neighbor->g_cost)
                {
                    in_open_list = true;
                    free(neighbor);
                    break;
                }
            }

            // Sinon, on l'ajoute à la liste des nodes à explorer
            if (!in_open_list)
            {
                add_node(open_list, neighbor);
            }
        }
    }

    free_node_list(open_list);
    free(goal_node);
    return NULL;
}

bool is_valid_no_wall(int x, int y)
{
    return x >= 0 && x < MAT_SIZE && y >= 0 && y < MAT_SIZE;
}

node_t *a_star_no_wall(int start_x, int start_y, int goal_x, int goal_y, int mat[][MAT_SIZE])
{
    node_t *start_node = create_node(start_x, start_y, NULL);
    node_t *goal_node = create_node(goal_x, goal_y, NULL);

    node_list_t *open_list = create_node_list();
    add_node(open_list, start_node);

    int closed_list[MAT_SIZE][MAT_SIZE] = {0};

    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    while (open_list->size > 0)
    {
        node_t *current_node = open_list->nodes[0];
        int current_index = 0;

        for (int i = 1; i < open_list->size; i++)
        {
            if (open_list->nodes[i]->f_cost < current_node->f_cost)
            {
                current_node = open_list->nodes[i];
                current_index = i;
            }
        }

        if (current_node->x == goal_node->x && current_node->y == goal_node->y)
        {
            free_node_list(open_list);
            free(goal_node);
            return current_node;
        }

        remove_node(open_list, current_index);
        closed_list[current_node->y][current_node->x] = 1;

        for (int i = 0; i < 4; i++)
        {
            int new_x = current_node->x + dx[i];
            int new_y = current_node->y + dy[i];

            if (!is_valid_no_wall(new_x, new_y) || closed_list[new_y][new_x])
            {
                continue;
            }

            node_t *neighbor = create_node(new_x, new_y, current_node);
            neighbor->g_cost = current_node->g_cost + 1;
            neighbor->h_cost = heuristic(neighbor, goal_node);
            neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;

            bool in_open_list = false;
            for (int j = 0; j < open_list->size; j++)
            {
                if (open_list->nodes[j]->x == new_x && open_list->nodes[j]->y == new_y && open_list->nodes[j]->g_cost <= neighbor->g_cost)
                {
                    in_open_list = true;
                    free(neighbor);
                    break;
                }
            }

            if (!in_open_list)
            {
                add_node(open_list, neighbor);
            }
        }
    }

    free_node_list(open_list);
    free(goal_node);
    return NULL;
}

// Affichage du chemin en coordonnées
void display_path(node_t *node)
{
    if (node->parent != NULL)
    {
        display_path(node->parent);
        printf("-> (%d, %d) ", node->x, node->y);
    }
    else
    {
        printf("(%d, %d) ", node->x, node->y);
    }
}

// Affichage du chemin avec des 5 dans la matrice
void fill_path_in_mat(node_t *node, int mat[][MAT_SIZE])
{
    if (node->parent != NULL)
    {
        fill_path_in_mat(node->parent, mat);
    }
    mat[node->y][node->x] = 5;
}

// Affichage de la matrice
void display_mat(int mat[][MAT_SIZE])
{
    for (int y = 0; y < MAT_SIZE; y++)
    {
        for (int x = 0; x < MAT_SIZE; x++)
        {
            printf("%2d ", mat[y][x]);
        }
        printf("\n");
    }
}