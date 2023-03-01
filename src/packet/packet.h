/**
 * @file packet.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de packet/packet.c
 * @version 0.1
 * @date 01/03/2023
 *
 */

#ifndef __PACKET_H
#define __PACKET_H

#include <stdint.h>
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

extern packet_t *createHandshakePacket(int data);

extern void readHandshakePacket(packet_t *packet, int *data);

extern packet_t *createSetPseudoPacket(char *pseudo);

extern void readSetPseudoPacket(packet_t *packet, char **pseudo);

#endif