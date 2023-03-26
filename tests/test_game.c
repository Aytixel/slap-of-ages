#include <stdio.h>
#include <stdlib.h>
#include "game/game.h"

#define MAT_SIZE 10

int main()
{
    int mat[MAT_SIZE][MAT_SIZE] = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {-1, 0, 0, -1, -1, -1, -1, -1, 0, -1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, -1, 0, 0, 0, 0, 0, 0, 0, -1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, -1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, -1, 0, -1, 2, 0}};
    int start_x = 0;
    int start_y = 0;
    int goal_x = MAT_SIZE - 1;
    int goal_y = MAT_SIZE - 1;

    // Test Algortihme A*
    printf("Test Algorithme A*:\n");
    node_t *path = a_star(start_x, start_y, goal_x, goal_y, mat);

    if (path)
    {
        printf("Chemin trouver: ");
        display_path(path);
        printf("\n");

        fill_path_in_mat(path, mat);
        mat[start_y][start_x] = START;
        mat[goal_y][goal_x] = GOAL;

        printf("Affichage du chemin trouvé dans la matrice( 5 = chemin utilisé ):\n");
        display_mat(mat);
    }
    else
    {
        printf("Pas de chemin trouver.\n");
    }

    // Reset de la matrice pour le test A* sans murs
    mat[start_y][start_x] = START;
    mat[goal_y][goal_x] = GOAL;
    for (int y = 0; y < MAT_SIZE; y++)
    {
        for (int x = 0; x < MAT_SIZE; x++)
        {
            if (mat[y][x] == 5)
            {
                mat[y][x] = 0;
            }
        }
    }

    // Test Algortihme A* sans murs (on peut passer à travers les murs)
    printf("\nTest Algorithme A* sans prise en compte des murs:\n");
    node_t *path_no_wall = a_star_no_wall(start_x, start_y, goal_x, goal_y, mat);

    if (path_no_wall)
    {
        printf("Chemin trouver: ");
        display_path(path_no_wall);
        printf("\n");

        fill_path_in_mat(path_no_wall, mat);
        mat[start_y][start_x] = START;
        mat[goal_y][goal_x] = GOAL;

        printf("Affichage du chemin trouvé dans la matrice( 5 = chemin utilisé ):\n");
        display_mat(mat);
    }
    else
    {
        printf("Pas de chemin trouver.\n");
    }
}