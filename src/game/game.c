/**
 * @file game.c
 * @author Arthur Dureau, Hôa Le Luet, Lucas Dureau
 * @brief Fonction divers pour le jeu
 * @version 1.0
 * @date 19/03/2023
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "client/common.h"
#include "game.h"

#define START 1
#define GOAL 2

node_list_t *create_node_list()
{
    node_list_t *list = (node_list_t *)malloc(sizeof(node_list_t));
    list->nodes = (node_t **)malloc(MAP_SIZE * sizeof(node_t *));
    list->size = 0;
    list->capacity = MAP_SIZE;
    return list;
}

void clear_node_list(node_list_t *list)
{
    do
    {
        if (list->nodes[--list->size] != NULL)
            free(list->nodes[list->size]);
    } while (list->size);
}

void free_node_list(node_list_t *list)
{
    free(list->nodes);
    free(list);
}

void remove_parent_from_list(node_list_t *list, node_t *node)
{
    node_t *last_node = NULL;

    while (node != NULL)
    {
        last_node = node;
        node = node->parent;

        for (int i = 0; i < list->size; i++)
        {
            if (list->nodes[i] == last_node)
            {
                remove_node(list, i);
                break;
            }
        }
    }
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

node_t *create_node(SDL_Point position, node_t *parent)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->position = position;
    new_node->g_cost = 0;
    new_node->h_cost = 0;
    new_node->f_cost = 0;
    new_node->parent = parent;
    return new_node;
}

void free_node_path(node_t *node)
{
    node_t *last_node = NULL;

    while (node != NULL)
    {
        last_node = node;
        node = node->parent;
        free(last_node);
    }
}

float heuristic(node_t *a, node_t *b)
{
    return sqrtf(powf(a->position.x - b->position.x, 2) + powf(a->position.y - b->position.y, 2));
}

bool is_valid(SDL_Point position, building_t ***map_building)
{
    if (position.x >= 0 && position.x < MAP_SIZE && position.y >= 0 && position.y < MAP_SIZE)
    {
        if (map_building[position.x][position.y] == NULL)
            return 1;
        switch (map_building[position.x][position.y]->type)
        {
        case CORNER_WALL_BUILDING:
        case VERTICAL_WALL_BUILDING:
        case HORIZONTAL_WALL_BUILDING:
            return 1;
            break;

        default:
            return 0;
            break;
        }
    }

    return 0;
}

bool is_valid_no_wall(SDL_Point position)
{
    return position.x >= 0 && position.x < MAP_SIZE && position.y >= 0 && position.y < MAP_SIZE;
}

node_t *a_star(SDL_Point start, SDL_Point goal, building_t ***map_building, int wall)
{

    node_t *start_node = create_node(start, NULL);
    node_t *goal_node = create_node(goal, NULL);

    // Liste des noeuds à explorer
    node_list_t *open_list = create_node_list(MAP_SIZE);
    node_list_t *all_list = create_node_list(MAP_SIZE);
    add_node(open_list, start_node);
    add_node(all_list, start_node);

    // Liste des noeuds déjà explorés
    int **closed_list = malloc(MAP_SIZE * sizeof(void *));
    for (int i = 0; i < MAP_SIZE; i++)
    {
        closed_list[i] = malloc(MAP_SIZE * sizeof(int));

        for (int j = 0; j < MAP_SIZE; j++)
        {
            closed_list[i][j] = 0;
        }
    }

    // Tableau des déplacements possibles
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    // Tant qu'il reste des noeuds à explorer
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

        // Si on a atteint le node final, on retourne le noeud courant
        if (current_node->position.x == goal_node->position.x && current_node->position.y == goal_node->position.y)
        {
            for (int i = 0; i < MAP_SIZE; i++)
            {
                free(closed_list[i]);
            }
            free(closed_list);
            remove_parent_from_list(all_list, current_node);
            clear_node_list(all_list);
            free_node_list(all_list);
            free_node_list(open_list);
            free(goal_node);
            return current_node;
        }

        // On retire le noeud de la liste des noeuds à explorer et on l'ajoute à la liste des noeuds déjà explorés
        remove_node(open_list, current_index);
        closed_list[current_node->position.y][current_node->position.x] = 1;

        // On explore les voisins du noeud courant
        for (int i = 0; i < 4; i++)
        {
            int new_x = current_node->position.x + dx[i];
            int new_y = current_node->position.y + dy[i];

            SDL_Point neighbor_position = {new_x, new_y};

            // Si le voisin est invalide ou qu'il a déjà été exploré, on passe au voisin suivant
            int invalid_neighbor = wall == 0 ? !is_valid(neighbor_position, map_building) : !is_valid_no_wall(neighbor_position);

            if (invalid_neighbor || closed_list[new_y][new_x])
                continue;

            // Calcule du coût du voisin
            node_t *neighbor = create_node(neighbor_position, current_node);
            neighbor->g_cost = current_node->g_cost + 1;
            neighbor->h_cost = heuristic(neighbor, goal_node);
            neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;

            bool in_open_list = false;

            // Parcours de la liste des noeuds, vérifie si les coordonnées du voisin sont déjà présentes dans la liste et vérifie si le coût du voisin est inférieur ou égal au coût du noeud déjà présent dans la liste
            for (int j = 0; j < open_list->size; j++)
            {
                if (open_list->nodes[j]->position.x == new_x && open_list->nodes[j]->position.y == new_y && open_list->nodes[j]->g_cost <= neighbor->g_cost)
                {
                    in_open_list = true;
                    break;
                }
            }

            // Sinon, on l'ajoute à la liste des noeuds à explorer
            if (!in_open_list)
            {
                add_node(open_list, neighbor);
                add_node(all_list, neighbor);
            }
            else
                free(neighbor);
        }
    }

    for (int i = 0; i < MAP_SIZE; i++)
    {
        free(closed_list[i]);
    }
    free(closed_list);
    clear_node_list(all_list);
    free_node_list(all_list);
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
        printf("-> (%d, %d) ", node->position.x, node->position.y);
    }
    else
    {
        printf("(%d, %d) ", node->position.x, node->position.y);
    }
}