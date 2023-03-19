/**
 * @file animation_states.h
 * @brief fichier contenant les différents états des animations
 * @author Hôa Le Luet
 * @version 1.0
 * @date 07/03/2023
 *
 *
 */

#ifndef __ANIMATION_STATES_H
#define __ANIMATION_STATES_H

/**
 * @brief   Enumération des états d'une animation de portail
 *
 * @param PORTAL_IDLE : le portail est ouvert
 * @param PORTAL_SPAWN : le portail apparaît
 * @param PORTAL_DESPAWN : le portail disparait
 * @return un type énuméré de type portal_anim_state_e
 */

typedef enum
{
    PORTAL_IDLE_ANIM,
    PORTAL_SPAWN_ANIM,
    PORTAL_DESPAWN_ANIM,

} portal_anim_state_e;

/**
 * @brief   Enumération des états d'une animation de livre
 *
 * @param BOOK_OPEN_ANIM : le livre s'ouvre
 * @param BOOK_CLOSE_ANIM : le livre se ferme
 * @param BOOK_OPENED_ANIM : le livre est ouvert
 * @return un type énuméré de type book_anim_state_e
 */

typedef enum
{
    BOOK_OPEN_ANIM,
    BOOK_CLOSE_ANIM,
    BOOK_OPENED_ANIM,

} book_anim_state_e;

/**
 * @brief   Enumération des états d'une animation de démon
 *
 * @param DAEMON_IDLE_ANIM : le démon est immobile
 * @param DAEMON_MOVE_ANIM : le démon se déplace
 * @param DAEMON_ATTACK_ANIM : le démon attaque
 * @param DAEMON_DEATH_ANIM : le démon meurt
 * @return un type énuméré de type daemon_anim_state_e
 */

typedef enum
{
    DAEMON_IDLE_ANIM,
    DAEMON_MOVE_ANIM,
    DAEMON_ATTACK_ANIM,
    DAEMON_DEATH_ANIM,

} daemon_anim_state_e;

/**
 * @brief   Enumération des états d'une animation de Géant goblin
 *
 * @param GIANT_GOBLIN_IDLE_ANIM : le géant goblin est immobile
 * @param GIANT_GOBLIN_MOVE_ANIM : le géant goblin se déplace
 * @param GIANT_GOBLIN_ATTACK_ANIM : le géant goblin attaque
 * @param GIANT_GOBLIN_IDLE2_ANIM : le géant goblin est immobile (animation différente)
 * @param GIANT_GOBLIN_DEATH_ANIM : le géant goblin meurt
 * @return un type énuméré de type goblin_giant_anim_state_e
 */

typedef enum
{
    GOBLIN_GIANT_IDLE_ANIM,
    GOBLIN_GIANT_MOVE_ANIM,
    GOBLIN_GIANT_ATTACK_ANIM,
    GOBLIN_GIANT_IDLE2_ANIM,
    GOBLIN_GIANT_DEATH_ANIM,

} goblin_giant_anim_state_e;

/**
 * @brief   Enumération des états d'une animation de Rat avec une hache
 *
 * @param RAT_IDLE_ANIM : le rat est immobile
 * @param RAT_MOVE_ANIM : le rat se déplace
 * @param RAT_ATTACK_ANIM : le rat attaque
 * @param RAT_IDLE2_ANIM : le rat est immobile (animation différente)
 * @param RAT_DEATH_ANIM : le rat meurt
 * @return un type énuméré de type rat_anim_state_e
 */

typedef enum
{
    RAT_IDLE_ANIM,
    RAT_MOVE_ANIM,
    RAT_ATTACK_ANIM,
    RAT_IDLE2_ANIM,
    RAT_DEATH_ANIM,

} rat_anim_state_e;

#endif