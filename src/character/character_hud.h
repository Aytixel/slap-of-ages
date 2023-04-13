/**
 * @file character_hud.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de character/character_hud.c
 * @version 1.1
 * @date 05/04/2023
 *
 */

#ifndef __CHARACTER_HUD_H
#define __CHARACTER_HUD_H

#include "client/game_data.h"
#include "character_renderer.h"
#include "window/input.h"
#include "window/animation.h"

/**
 * @brief Structure de données contenant les données de l'interface de sélection des troupes
 *
 */
typedef struct
{
    TTF_Font *text_font;                      /**< police d'écriture pour le texte*/
    animation_t *rat_animation;               /**< animation du rat*/
    animation_t *daemon_animation;            /**< animation du démon*/
    animation_t *giant_animation;             /**< animation du géant*/
    button_t *rat_button;                     /**< bouton de sélection de lu rat*/
    button_t *daemon_button;                  /**< bouton de sélection du démon*/
    button_t *giant_button;                   /**< bouton de sélection du géant goblin*/
    character_renderer_t *character_renderer; /**< un pointeur sur la structure qui gère l'affichage des troupes*/
} character_hud_t;

/**
 * @brief Créer les données de linterface de sélection de troupe
 *
 * @param window un pointeur sur une fenêtre
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 *
 * @return un pointer sur les **données de linterface de sélection de troupe**
 */
extern character_hud_t *createCharacterHud(window_t *window, character_renderer_t *character_renderer);

/**
 * @brief Fonction d'écoute des événements de linterface de sélection de troupe
 *
 * @param event un pointeur sur les événements
 * @param character_hud un pointeur sur linterface de sélection de troupe
 * @param game_data un pointeur sur les données du jeu
 */
extern void characterHudEventHandler(SDL_Event *event, character_hud_t *character_hud, client_game_data_t *game_data);

/**
 * @brief Fonction d'affichage de linterface de sélection de troupe
 *
 * @param window un pointeur sur une fenêtre
 * @param character_hud un pointeur sur linterface de sélection de troupe
 */
extern void renderCharacterHud(window_t *window, character_hud_t *character_hud);

/**
 * @brief Détruit les données de linterface de sélection de troupe
 *
 * @param character_hud une référence d'un pointeur sur les données de linterface de sélection de troupe
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteCharacterHud(character_hud_t **character_hud);

#endif