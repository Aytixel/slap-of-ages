/**
 * @file clien_packet.c
 * @author Lucas Dureau
 * @brief Implémente toute les fonctions pour créer, et lire les différents paquets
 * @version 0.1
 * @date 05/02/2023
 *
 */

#include "client_packet.h"

extern packet_t *createSetMapPacket(client_game_data_t *game_data)
{
    packet_t *packet = malloc(sizeof(packet_t));

    packet->id = SET_MAP_PACKET_ID;
    packet->data_length = serializeMapData(&packet->data, game_data->map_building, game_data->gold_cost);

    return packet;
}

extern void readSetMapPacket(packet_t *packet, window_t *window, client_game_data_t *game_data)
{
    deserializeMapData(packet->data, packet->data_length, window, game_data->opponent_map_building, &game_data->initial_opponent_gold_cost);

    game_data->opponent_gold_cost = game_data->initial_opponent_gold_cost;
}
