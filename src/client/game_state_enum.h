/**
 * @file game_state_enum.h
 * @author Lucas Dureau
 * @brief Définition des différents états du jeu
 * @version 1.1
 * @date 17/03/2023
 *
 */

#ifndef __CLIENT_GAME_STATE_ENUM_H
#define __CLIENT_GAME_STATE_ENUM_H

/**
 * @brief Enumération définissant les différents états du jeu
 *
 */
typedef enum
{
    CONNECTION_GAME_STATE,     /**< phase de connexion au serveur*/
    PREPARATION_GAME_STATE,    /**< phase de préparation du village*/
    MATCHMAKING_GAME_STATE,    /**< phase de d'attente d'une partie*/
    COMBAT_GAME_STATE,         /**< phase de combat*/
    WAITING_RESULT_GAME_STATE, /**< phase d'attente des résultats*/
} client_game_state_e;

#endif
