/**
 * @file packet_id.h
 * @author Lucas Dureau
 * @brief Définition des identifiants de paquet résaux
 * @version 0.1
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
    HANDSHAKE_PACKET_ID,  /**< poigné de main*/
    SET_PSEUDO_PACKET_ID, /**< envoie du pseudo*/
} packet_id_e;

#endif