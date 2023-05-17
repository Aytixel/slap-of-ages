/**
 * @file game.c
 * @author Arthur Dureau, Hôa Le Luet, Lucas Dureau
 * @brief Fonction divers pour le jeu
 * @version 1.1
 * @date 19/03/2023
 *
 *
 */

#include "game.h"

#define START 1
#define GOAL 2

node_list_t *createNodeList()
{
    node_list_t *list = (node_list_t *)malloc(sizeof(node_list_t));

    list->size = 0;
    list->capacity = MAP_SIZE;
    list->nodes = (node_t **)malloc(list->capacity * sizeof(node_t *));

    return list;
}

void clearNodeList(node_list_t *list)
{
    while (list->size > 0)
    {
        list->size--;

        if (list->nodes[list->size] != NULL)
            free(list->nodes[list->size]);
    }
}

void freeNodeList(node_list_t *list)
{
    free(list->nodes);
    free(list);
}

void removeParentFromList(node_list_t *list, node_t *node)
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
                removeNode(list, i);
                break;
            }
        }
    }
}

void addNode(node_list_t *list, node_t *node)
{
    if (list->size == list->capacity)
    {
        list->capacity *= 2;
        list->nodes = (node_t **)realloc(list->nodes, list->capacity * sizeof(node_t *));
    }

    list->nodes[list->size++] = node;
}

node_t *removeNode(node_list_t *list, int index)
{
    node_t *node = list->nodes[index];

    list->nodes[index] = list->nodes[--list->size];

    return node;
}

node_t *createNode(SDL_Point position, node_t *parent)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    new_node->position = position;
    new_node->g_cost = 0;
    new_node->h_cost = 0;
    new_node->f_cost = 0;
    new_node->parent = parent;

    return new_node;
}

void freeNodePath(node_t *node)
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

bool isValidNoWall(SDL_Point position)
{
    return position.x >= -CHARACTER_PLACEMENT_MARGIN &&
           position.x < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN &&
           position.y >= -CHARACTER_PLACEMENT_MARGIN &&
           position.y < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN;
}

bool isValid(SDL_Point position, building_t ***map_building)
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
            return 0;
        default:
            return 1;
        }
    }

    return isValidNoWall(position);
}

node_t *aStar(SDL_Point start, SDL_Point goal, building_t ***map_building, int wall)
{

    node_t *start_node = createNode(start, NULL);
    node_t *goal_node = createNode(goal, NULL);

    // Liste des noeuds à explorer
    node_list_t *open_list = createNodeList();
    node_list_t *all_list = createNodeList();

    addNode(open_list, start_node);
    addNode(all_list, start_node);

    // Liste des noeuds déjà explorés
    int **closed_list = malloc((MAP_SIZE + CHARACTER_PLACEMENT_MARGIN * 2) * sizeof(void *));

    closed_list += CHARACTER_PLACEMENT_MARGIN;

    for (int i = -CHARACTER_PLACEMENT_MARGIN; i < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; i++)
    {
        closed_list[i] = malloc((MAP_SIZE + CHARACTER_PLACEMENT_MARGIN * 2) * sizeof(int));
        closed_list[i] += CHARACTER_PLACEMENT_MARGIN;

        for (int j = -CHARACTER_PLACEMENT_MARGIN; j < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; j++)
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
            for (int i = -CHARACTER_PLACEMENT_MARGIN; i < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; i++)
            {
                free(closed_list[i] - CHARACTER_PLACEMENT_MARGIN);
            }

            free(closed_list - CHARACTER_PLACEMENT_MARGIN);
            removeParentFromList(all_list, current_node);
            clearNodeList(all_list);
            freeNodeList(all_list);
            freeNodeList(open_list);
            free(goal_node);
            gotoNextPositionInPath(&current_node);

            return current_node;
        }

        // On retire le noeud de la liste des noeuds à explorer et on l'ajoute à la liste des noeuds déjà explorés
        removeNode(open_list, current_index);
        closed_list[current_node->position.y][current_node->position.x] = 1;

        // On explore les voisins du noeud courant
        for (int i = 0; i < 4; i++)
        {
            int new_x = current_node->position.x + dx[i];
            int new_y = current_node->position.y + dy[i];

            SDL_Point neighbor_position = {new_x, new_y};

            // Si le voisin est invalide ou qu'il a déjà été exploré, on passe au voisin suivant
            int invalid_neighbor = wall ? !isValid(neighbor_position, map_building) : !isValidNoWall(neighbor_position);

            if (invalid_neighbor || closed_list[new_y][new_x])
                continue;

            // Calcule du coût du voisin
            node_t *neighbor = createNode(neighbor_position, current_node);
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
                addNode(open_list, neighbor);
                addNode(all_list, neighbor);
            }
            else
                free(neighbor);
        }
    }

    for (int i = -CHARACTER_PLACEMENT_MARGIN; i < MAP_SIZE + CHARACTER_PLACEMENT_MARGIN; i++)
    {
        free(closed_list[i] - CHARACTER_PLACEMENT_MARGIN);
    }

    free(closed_list - CHARACTER_PLACEMENT_MARGIN);
    clearNodeList(all_list);
    freeNodeList(all_list);
    freeNodeList(open_list);
    free(goal_node);

    return NULL;
}

extern void gotoNextPositionInPath(node_t **node)
{
    while ((*node)->parent != NULL)
    {
        node = &(*node)->parent;
    }

    free(*node);
    *node = NULL;
}

SDL_Point getNextPositionInPath(node_t *node)
{
    while (node->parent != NULL)
    {
        node = node->parent;
    }

    return node->position;
}

// Affichage du chemin en coordonnées
void displayPath(node_t *node)
{
    while (node != NULL)
    {
        printf("(%d, %d) <- ", node->position.x, node->position.y);
        node = node->parent;
    }
}