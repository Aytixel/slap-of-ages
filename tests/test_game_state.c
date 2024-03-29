#include <stdio.h>
#include <assert.h>
#include "server/game_state.h"

//@import math
//@main
int main(int argc, char *argv[])
{
    printf("Test de server/game_state.h\n\n");

    server_game_state_t *game_state = createGameState();

    assert(game_state != NULL);
    assert(isGameEmpty(game_state));
    assert(!isGameStarted(game_state));
    assert(!isGameFinished(game_state));
    printf("Création des données de partie OK\n");

    assert(addPlayerToGame(game_state, 0, NULL, NULL) == 1);
    assert(!isGameEmpty(game_state));
    assert(!isGameStarted(game_state));
    assert(!isGameFinished(game_state));
    assert(game_state->player[0]->socket_fd == 0);
    printf("\n\tAjout d'un 1er joueur à la partie OK\n");

    assert(addPlayerToGame(game_state, 1, NULL, NULL) == 1);
    assert(isGameStarted(game_state) == 1);
    assert(!isGameFinished(game_state));
    assert(game_state->player[1]->socket_fd == 1);
    printf("\tAjout d'un 2ème joueur à la partie OK\n");

    assert(removePlayerFromGame(game_state, 0) == 1);
    assert(!isGameStarted(game_state));
    assert(!isGameFinished(game_state));
    assert(game_state->player[0] == NULL);
    printf("\nSuppression du 1er joueur à la partie OK\n");

    assert(addPlayerToGame(game_state, 2, NULL, NULL) == 1);
    assert(isGameStarted(game_state) == 1);
    assert(!isGameFinished(game_state));
    assert(game_state->player[0]->socket_fd == 2);
    printf("\n\tAjout d'un 1er joueur à la partie OK\n");

    assert(!addPlayerToGame(game_state, 3, NULL, NULL));
    assert(isGameStarted(game_state) == 1);
    assert(!isGameFinished(game_state));
    printf("\tAjout d'un 3ème joueur à la partie impossible OK\n");

    assert(gameHasPlayer(game_state, 0) == -1);
    assert(gameHasPlayer(game_state, 1) == 1);
    assert(!gameHasPlayer(game_state, 2));
    assert(gameHasPlayer(game_state, 3) == -1);
    printf("\n\tLes bons joueur dont dans la partie OK\n");

    char data[12] = {0};
    packet_t packet = {0, &data, 0};

    assert(!setPlayerFinished(game_state, 1, &packet));
    assert(game_state->player[1]->has_finished == 1);
    assert(!game_state->player[1]->destruction_percentage);
    assert(!game_state->player[1]->time_left);
    assert(!isGameFinished(game_state));
    printf("\n\tLe 1er joueur a fini OK\n");

    assert(setPlayerFinished(game_state, 3, &packet) == -1);
    printf("\tLe 3ème joueur ne peut finir OK\n");

    assert(!setPlayerFinished(game_state, 2, &packet));
    assert(game_state->player[0]->has_finished == 1);
    assert(!game_state->player[0]->destruction_percentage);
    assert(!game_state->player[0]->time_left);
    assert(isGameFinished(game_state) == 1);
    printf("\tLe 2ème joueur a fini OK\n");

    assert(!deleteGameState(&game_state));
    assert(game_state == NULL);
    printf("\nDestruction des données de partie OK\n");

    server_game_state_array_t *game_data_array = createGameStateArray();

    assert(game_data_array != NULL);
    printf("\nCréation d'un tableau de partie OK\n");

    assert(!removeGameStateFromArray(game_data_array, 234));
    assert(!removeGameStateFromArray(game_data_array, -1));
    printf("\n\tImpossible de supprimer une partie quand le tableau est vide OK\n");

    addGameStateToArray(game_data_array);

    assert(removeGameStateFromArray(game_data_array, 0) == 1);
    printf("\n\tAjout et suppression d'une partie OK\n");

    assert(findGame(game_data_array) == -1);
    printf("\n\tAucune partie trouvé OK\n");

    addGameStateToArray(game_data_array);
    addGameStateToArray(game_data_array);
    addGameStateToArray(game_data_array);

    assert(!findGame(game_data_array));
    assert(addPlayerToGame(game_data_array->game_state[0], 2, NULL, NULL) == 1);
    assert(!findGame(game_data_array));
    printf("\n\t1er partie trouvé OK\n");

    assert(addPlayerToGame(game_data_array->game_state[0], 1, NULL, NULL) == 1);
    assert(findGame(game_data_array) == 1);
    printf("\t2ème partie trouvé OK\n");

    assert(removeGameStateFromArray(game_data_array, 1) == 1);
    assert(removeGameStateFromArray(game_data_array, 1) == 1);
    assert(removeGameStateFromArray(game_data_array, 0) == 1);
    printf("\n\tAjout et suppression de 3 parties OK\n");

    addGameStateToArray(game_data_array);

    assert(!deleteGameStateArray(&game_data_array));
    assert(game_data_array == NULL);
    printf("\nDestruction d'un tableau de partie OK\n");

    printf("\nTest de server/game_state.h succès\n");

    return 0;
}