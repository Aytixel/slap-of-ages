/**
 * @file packet.c
 * @author Lucas Dureau
 * @brief Implémente toute les fonctions pour créer, et lire les différents paquets
 * @version 0.1
 * @date 05/02/2023
 *
 */

#include <stdlib.h>
#include <string.h>
#include "packet.h"

/**
 * @brief Créer un paquet de poignée de main
 *
 * @param handshake_data données de la poignée de main
 * @return un pointer sur un **paquet**
 */
extern packet_t *createHandshakePacket(int data)
{
    packet_t *packet = malloc(sizeof(packet_t));

    packet->id = HANDSHAKE_PACKET_ID;
    packet->data = malloc(sizeof(int));

    memcpy(packet->data, &data, sizeof(int));

    packet->data_length = sizeof(int);

    return packet;
}

/**
 * @brief Lie un paquet de poignée de main
 *
 * @param packet paquet à lire
 * @param data un pointeur sur les données de la poignée de main
 */
extern void readHandshakePacket(packet_t *packet, int *data)
{
    memcpy(data, packet->data, sizeof(int));
}

/**
 * @brief Créer un paquet définissant le pseudo
 *
 * @param pseudo
 * @return un pointer sur un **paquet**
 */
extern packet_t *createSetPseudoPacket(char *pseudo)
{
    packet_t *packet = malloc(sizeof(packet_t));
    int pseudo_length = strlen(pseudo) + 1;

    packet->id = SET_PSEUDO_PACKET_ID;
    packet->data_length = sizeof(int) + pseudo_length;
    packet->data = malloc(packet->data_length);

    memcpy(packet->data, &pseudo_length, sizeof(int));
    memcpy(packet->data + sizeof(int), pseudo, pseudo_length);

    return packet;
}

/**
 * @brief Lie un paquet définissant le pseudo
 *
 * @param packet paquet à lire
 * @param pseudo un pointeur sur le pseudo
 */
extern void readSetPseudoPacket(packet_t *packet, char **pseudo)
{
    int pseudo_length;

    memcpy(&pseudo_length, packet->data, sizeof(int));

    *pseudo = malloc(pseudo_length);

    memcpy(*pseudo, packet->data + sizeof(int), pseudo_length);
}