/**
 * @file clien_packet.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de packet/packet.c
 * @version 0.1
 * @date 01/03/2023
 *
 */

#ifndef __CLIENT_PACKET_H
#define __CLIENT_PACKET_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "client/map_data_serialization.h"
#include "client/game_data.h"
#include "building/building.h"
#include "packet.h"

/**
 * @brief Créer un paquet définissant la carte
 *
 * @param game_data un pointeur sur les données du jeu
 * @return un pointer sur un **paquet**
 */
extern packet_t *createSetMapPacket(client_game_data_t *game_data);

/**
 * @brief Lie un paquet définissant la carte
 *
 * @param packet paquet à lire
 * @param window un pointeur sur une fenêtre
 * @param game_data un pointeur sur les données du jeu
 */
extern void readSetMapPacket(packet_t *packet, window_t *window, client_game_data_t *game_data);

#endif