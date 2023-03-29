/**
 * @file animation_states.h
 * @brief fichier contenant les différents états des animations
 * @author Hôa Le Luet
 * @version 1.0
 * @date 07/03/2023
 *
 */

#ifndef __ANIMATION_STATES_H
#define __ANIMATION_STATES_H

/**
 * @brief   Enumération des états d'une animation de portail
 *
 */
typedef enum
{
    PORTAL_IDLE_ANIM,    /**< le portail est ouvert*/
    PORTAL_SPAWN_ANIM,   /**< le portail apparaît*/
    PORTAL_DESPAWN_ANIM, /**< le portail disparait*/
} portal_anim_state_e;

/**
 * @brief   Enumération des états d'une animation de livre
 *
 */
typedef enum
{
    BOOK_OPEN_ANIM,   /**< le livre s'ouvre*/
    BOOK_CLOSE_ANIM,  /**< le livre se ferme*/
    BOOK_OPENED_ANIM, /**< le livre est ouvert*/
} book_anim_state_e;

/**
 * @brief   Enumération des états d'une animation de démon
 *
 */
typedef enum
{
    DAEMON_IDLE_ANIM,   /**< le démon est immobile*/
    DAEMON_MOVE_ANIM,   /**< le démon se déplace*/
    DAEMON_ATTACK_ANIM, /**< le démon attaque*/
    DAEMON_DEATH_ANIM,  /**< le démon meurt*/
} daemon_anim_state_e;

/**
 * @brief   Enumération des états d'une animation de Géant goblin
 *
 */
typedef enum
{
    GOBLIN_GIANT_IDLE_ANIM,   /**< le géant goblin est immobile*/
    GOBLIN_GIANT_MOVE_ANIM,   /**< le géant goblin se déplace*/
    GOBLIN_GIANT_ATTACK_ANIM, /**< le géant goblin attaque*/
    GOBLIN_GIANT_IDLE2_ANIM,  /**< le géant goblin est immobile variante*/
    GOBLIN_GIANT_DEATH_ANIM,  /**< le géant goblin meurt*/
} goblin_giant_anim_state_e;

/**
 * @brief   Enumération des états d'une animation de Rat avec une hache
 *
 */
typedef enum
{
    RAT_IDLE_ANIM,   /**< le rat est immobile*/
    RAT_MOVE_ANIM,   /**< le rat se déplace*/
    RAT_ATTACK_ANIM, /**< le rat attaque*/
    RAT_IDLE2_ANIM,  /**< le rat est immobile variante*/
    RAT_DEATH_ANIM,  /**< le rat meurt*/
} rat_anim_state_e;

#endif