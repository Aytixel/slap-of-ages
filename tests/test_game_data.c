#include <stdio.h>
#include <assert.h>
#include "server/game_data.h"

int main()
{
    printf("Test de server/game_data.h\n\n");

    game_data_t *game_data = createGameData();

    assert(game_data != NULL);
    assert(isGameStarted(game_data) == 0);
    assert(isGameFinished(game_data) == 0);
    printf("Création des données de partie OK\n");

    assert(addPlayerToGame(game_data, 0) == 1);
    assert(isGameStarted(game_data) == 0);
    assert(isGameFinished(game_data) == 0);
    assert(game_data->player[0]->socket_fd == 0);
    printf("\n\tAjout d'un 1er joueur à la partie OK\n");

    assert(addPlayerToGame(game_data, 1) == 1);
    assert(isGameStarted(game_data) == 1);
    assert(isGameFinished(game_data) == 0);
    assert(game_data->player[1]->socket_fd == 1);
    printf("\tAjout d'un 2ème joueur à la partie OK\n");

    assert(removePlayerFromGame(game_data, 0) == 1);
    assert(isGameStarted(game_data) == 0);
    assert(isGameFinished(game_data) == 0);
    assert(game_data->player[0] == NULL);
    printf("\nSuppression du 1er joueur à la partie OK\n");

    assert(addPlayerToGame(game_data, 2) == 1);
    assert(isGameStarted(game_data) == 1);
    assert(isGameFinished(game_data) == 0);
    assert(game_data->player[0]->socket_fd == 2);
    printf("\n\tAjout d'un 1er joueur à la partie OK\n");

    assert(addPlayerToGame(game_data, 3) == 0);
    assert(isGameStarted(game_data) == 1);
    assert(isGameFinished(game_data) == 0);
    printf("\tAjout d'un 3ème joueur à la partie impossible OK\n");

    assert(gameHasPlayer(game_data, 0) == -1);
    assert(gameHasPlayer(game_data, 1) == 1);
    assert(gameHasPlayer(game_data, 2) == 0);
    assert(gameHasPlayer(game_data, 3) == -1);
    printf("\n\tLes bons joueur dont dans la partie OK\n");

    char data[12] = {0};
    packet_t packet = {0, &data, 0};

    assert(setPlayerFinished(game_data, 1, &packet) == 0);
    assert(game_data->player[1]->has_finished == 1);
    assert(game_data->player[1]->destruction_percentage == 0);
    assert(game_data->player[1]->time_left == 0);
    assert(isGameFinished(game_data) == 0);
    printf("\n\tLe 1er joueur a fini OK\n");

    assert(setPlayerFinished(game_data, 3, &packet) == -1);
    printf("\tLe 3ème joueur ne peut finir OK\n");

    assert(setPlayerFinished(game_data, 2, &packet) == 0);
    assert(game_data->player[0]->has_finished == 1);
    assert(game_data->player[0]->destruction_percentage == 0);
    assert(game_data->player[0]->time_left == 0);
    assert(isGameFinished(game_data) == 1);
    printf("\tLe 2ème joueur a fini OK\n");

    assert(!deleteGameData(&game_data));
    assert(game_data == NULL);
    printf("\nDestruction des données de partie OK\n");

    printf("\nTest de server/game_data.h succès\n");

    return 0;
}