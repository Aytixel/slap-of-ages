/**
 * @file character_types.h
 * @author Lucas Dureau
 * @brief Gestion des différents types de troupe
 * @version 1.1
 * @date 17/05/2023
 *
 */

#ifndef __CHARACTER_TYPES_H
#define __CHARACTER_TYPES_H

/**
 * @brief Enumération des différents types de troupe
 *
 */
typedef enum character_type_e
{
    GIANT_CHARACTER,  /**< géant goblin*/
    DAEMON_CHARACTER, /**< démon*/
    RAT_CHARACTER     /**< rat guerrier*/
} character_type_e;

#endif