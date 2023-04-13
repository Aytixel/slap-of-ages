/**
 * @file packet_id.h
 * @author Lucas Dureau
 * @brief Définition des identifiants de paquet résaux
 * @version 1.1
 * @date 05/02/2023
 *
 */

#ifndef __PACKET_ID_H
#define __PACKET_ID_H

/**
 * @brief Enumération définissant tous les identifiants de paquet résaux
 *
 */
typedef enum
{
    HANDSHAKE_PACKET_ID,       /**< poigné de main*/
    SET_PSEUDO_PACKET_ID,      /**< envoie du pseudo*/
    SET_MAP_PACKET_ID,         /**< envoie la carte*/
    IS_PLAYER_READY_PACKET_ID, /**< définie si le joueur est prêt*/
    GAME_FINISHED_PACKET_ID,   /**< définie partie comme terminée*/
    HAS_PLAYER_WON_PACKET_ID,  /**< définie si le joueur à gagné*/
} packet_id_e;

#endif