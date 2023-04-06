/**
 * @file game_data_serialization.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de client/game_data_serialization.c
 * @version 0.1
 * @date 01/04/2023
 *
 */

#ifndef __GAME_DATA_SERIALIZATION_H
#define __GAME_DATA_SERIALIZATION_H

#include "window/window.h"
#include "game_data.h"
#include "map_data_serialization.h"

/**
 * @brief Converti les données du jeu, en données pouvant être stockées et les stockes
 *
 * @param game_data un pointeur sur les données du jeu
 * @return **0** si tous se passe bien, **-1** sinon
 */
extern int serializeGameData(client_game_data_t *game_data);

/**
 * @brief Reconvertie les données du jeu en structure et en variable
 *
 * @param window un pointeur sur une fenêtre
 * @param game_data un pointeur sur les données du jeu
 * @return **0** si tous se passe bien, **-1** sinon
 */
extern int deserializeGameData(window_t *window, client_game_data_t *game_data);

#endif