/**
 * @file map_data_serialization.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de client/map_data_serialization.c
 * @version 0.1
 * @date 28/03/2023
 *
 */

#ifndef __MAP_DATA_SERIALIZATION_H
#define __MAP_DATA_SERIALIZATION_H

#include "client/game_data.h"
#include "map/building.h"

/**
 * @brief Converti les données de la carte en données pouvant être stocker ou envoyer
 *
 * @param data une référence sur un pointeur sur les données
 * @param map_building matrice contenant la totalité des bâtiments placés sur la carte
 * @param gold_cost le coût en or du village
 * @param map_size taille de la carte
 * @return **taille des données**
 */
extern int serialize_map(void **data, building_t ***map_building, int gold_cost, int map_size);

/**
 * @brief Reconverti les données de la carte en structure et variable
 *
 * @param data un pointeur sur les données
 * @param data_lenth longueur des données
 * @param window un pointeur sur une fenêtre
 * @param map_building matrice contenant la totalité des bâtiments placés sur la carte
 * @param gold_cost un pointeur sur le coût en or du village
 * @param map_size taille de la carte
 * @return **0** si tous se passe bien, **-1** sinon
 */
extern int deserialize_map(void *data, int data_lenth, window_t *window, building_t ***map_building, int *gold_cost, int map_size);

#endif