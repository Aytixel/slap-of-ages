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

extern packet_t *createHandshakePacket(int data)
{
    packet_t *packet = malloc(sizeof(packet_t));

    packet->id = HANDSHAKE_PACKET_ID;
    packet->data = malloc(sizeof(int));

    memcpy(packet->data, &data, sizeof(int));

    packet->data_length = sizeof(int);

    return packet;
}

extern void readHandshakePacket(packet_t *packet, int *data)
{
    memcpy(data, packet->data, sizeof(int));
}

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

extern void readSetPseudoPacket(packet_t *packet, char **pseudo)
{
    int pseudo_length;

    memcpy(&pseudo_length, packet->data, sizeof(int));

    *pseudo = malloc(pseudo_length);

    memcpy(*pseudo, packet->data + sizeof(int), pseudo_length);
}

extern packet_t *createSetMapPacket()
{
    packet_t *packet = malloc(sizeof(packet_t));

    // temporaire
    packet->id = SET_MAP_PACKET_ID;
    packet->data_length = 0;
    packet->data = NULL;

    return packet;
}

extern void readSetMapPacket(packet_t *packet)
{
    // temporaire
}

extern packet_t *createIsPlayerReadyPacket(int is_player_ready)
{
    packet_t *packet = malloc(sizeof(packet_t));

    packet->id = IS_PLAYER_READY_PACKET_ID;
    packet->data_length = sizeof(int);
    packet->data = malloc(packet->data_length);

    memcpy(packet->data, &is_player_ready, sizeof(int));

    return packet;
}

extern void readIsPlayerReadyPacket(packet_t *packet, int *is_player_ready)
{
    memcpy(is_player_ready, packet->data, sizeof(int));
}

extern packet_t *createGameFinishedPacket(float destruction_percentage, long time_left)
{
    packet_t *packet = malloc(sizeof(packet_t));

    packet->id = GAME_FINISHED_PACKET_ID;
    packet->data_length = sizeof(float) + sizeof(long);
    packet->data = malloc(packet->data_length);

    memcpy(packet->data, &destruction_percentage, sizeof(float));
    memcpy(packet->data + sizeof(float), &time_left, sizeof(long));

    return packet;
}

extern void readGameFinishedPacket(packet_t *packet, float *destruction_percentage, long *time_left)
{
    memcpy(destruction_percentage, packet->data, sizeof(float));
    memcpy(time_left, packet->data + sizeof(float), sizeof(long));
}

extern packet_t *createHasPlayerWonPacket(int has_player_won)
{
    packet_t *packet = malloc(sizeof(packet_t));

    packet->id = HAS_PLAYER_WON_PACKET_ID;
    packet->data_length = sizeof(int);
    packet->data = malloc(packet->data_length);

    memcpy(packet->data, &has_player_won, sizeof(int));

    return packet;
}

extern void readHasPlayerWonPacket(packet_t *packet, int *has_player_won)
{
    memcpy(has_player_won, packet->data, sizeof(int));
}