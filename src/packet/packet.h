/**
 * @file packet.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de packet/packet.c
 * @version 1.1
 * @date 01/03/2023
 *
 */

#ifndef __PACKET_H
#define __PACKET_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "packet_id.h"

/**
 * @brief Paquet réseaux pour les sockets
 *
 */
typedef struct
{
    uint8_t id;         /**< id du packet (de 0 a 255)*/
    void *data;         /**< buffer de données*/
    size_t data_length; /**< taille du buffer de données*/
} packet_t;

/**
 * @brief Créer un paquet de poignée de main
 *
 * @param handshake_data données de la poignée de main
 * @return un pointer sur un **paquet**
 */
extern packet_t *createHandshakePacket(int data);

/**
 * @brief Lie un paquet de poignée de main
 *
 * @param packet paquet à lire
 * @param data un pointeur sur les données de la poignée de main
 */
extern void readHandshakePacket(packet_t *packet, int *data);

/**
 * @brief Créer un paquet définissant le pseudo
 *
 * @param pseudo
 * @return un pointer sur un **paquet**
 */
extern packet_t *createSetPseudoPacket(char *pseudo);

/**
 * @brief Lie un paquet définissant le pseudo
 *
 * @param packet paquet à lire
 * @param pseudo un pointeur sur le pseudo
 */
extern void readSetPseudoPacket(packet_t *packet, char **pseudo);

/**
 * @brief Créer un paquet définissant si le joueur est prêt
 *
 * @param is_player_ready le joueur est-il prêt ?
 * @return un pointer sur un **paquet**
 */
extern packet_t *createIsPlayerReadyPacket(int is_player_ready);

/**
 * @brief Lie un paquet définissant si le joueur est prêt
 *
 * @param packet paquet à lire
 * @param is_player_ready le joueur est-il prêt ?
 */
extern void readIsPlayerReadyPacket(packet_t *packet, int *is_player_ready);

/**
 * @brief Créer un paquet définissant le fait qu'un joueur à terminé
 *
 * @param destruction_percentage pourcentage de destruction
 * @param time_left temps restant avant la fin de partie
 * @return un pointer sur un **paquet**
 */
extern packet_t *createGameFinishedPacket(int destruction_percentage, long time_left);

/**
 * @brief Lie un paquet définissant le fait qu'un joueur à terminé
 *
 * @param packet paquet à lire
 * @param destruction_percentage pourcentage de destruction
 * @param time_left temps restant avant la fin de partie
 */
extern void readGameFinishedPacket(packet_t *packet, int *destruction_percentage, long *time_left);

/**
 * @brief Créer un paquet définissant si le joueur à gagné
 *
 * @param has_player_won le joueur est-il gagant ?
 * @return un pointer sur un **paquet**
 */
extern packet_t *createHasPlayerWonPacket(int has_player_won);

/**
 * @brief Lie un paquet définissant si le joueur est prêt
 *
 * @param packet paquet à lire
 * @param has_player_won le joueur est-il gagant ?
 */
extern void readHasPlayerWonPacket(packet_t *packet, int *has_player_won);

#endif