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
    PACKET_ID_HANDSHAKE,        /**< poigné de main*/
    PACKET_ID_CONNECTION_CLOSE, /**< connexion fermé*/
} packet_id_e;

#endif