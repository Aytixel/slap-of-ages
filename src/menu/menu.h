/**
 * @file menu.h
 * @author Arthur Dureau, Lucas Dureau
 * @brief Implémentation des prototypes de menu/menu.c
 * @version 1.0
 * @date 03/03/2023
 *
 */

#ifndef __MENU_H
#define __MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "window/window.h"
#include "window/input.h"
#include "window/animation.h"
#include "window/animation_states.h"
#include "client/game_data.h"

/**
 * @brief Structure pour stocker information du menu_test
 *
 */
typedef struct
{
    TTF_Font *text_font;                    /**< police d'écriture du texte*/
    TTF_Font *textbox_font;                 /**< police d'écriture pour les boites de texte*/
    animation_t *book_animation;                 /**< animation du livre*/
    book_anim_state_e book_animation_state; /**< état de l'animation du livre*/
    button_t *join_button;                  /**< bouton rejoindre*/
    button_t *quit_button;                  /**< bouton quitter*/
    sprite_t *hostname_label;               /**< sprite de l'étiquette "ip"*/
    sprite_t *port_label;                   /**< sprite de l'étiquette "port"*/
    sprite_t *pseudo_label;                 /**< sprite de l'étiquette "pseudo"*/
    textbox_t *hostname_textbox;            /**< boite de text "ip"*/
    textbox_t *port_textbox;                /**< boite de text "port"*/
    textbox_t *pseudo_textbox;              /**< boite de text "pseudo"*/
    SDL_Rect hostname_rect;                 /**< rectangle d'affiche de la boite de text "ip"*/
    SDL_Rect port_rect;                     /**< rectangle d'affiche de la boite de text "port"*/
    SDL_Rect pseudo_rect;                   /**< rectangle d'affiche de la boite de text "pseudo"*/
} menu_t;

/**
 * @brief Fonction de création du menu
 *
 * @param window un pointeur sur une fenêtre
 * @param game_data un pointeur sur les données du jeu
 * @return un pointeur sur un menu
 */
extern menu_t *createMenu(window_t *window, client_game_data_t *game_data);

/**
 * @brief Fonction d'écoute des événements du menu
 *
 * @param game_data un pointeur sur les données du jeu
 * @param event un pointeur sur les événements
 * @param menu un pointeur sur un menu
 * @return 1 si le bouton rejoindre est cliquer, 0 sinon
 */
extern int menuEventHandler(client_game_data_t *game_data, SDL_Event *event, menu_t *menu);

/**
 * @brief Fonction d'affichage du menu
 *
 * @param window un pointeur sur une fenêtre
 * @param menu un pointeur sur un menu
 * @return 1 si le programme peut continuer, 0 sinon
 */
extern int menuRenderer(window_t *window, menu_t *menu);

/**
 * @brief Fonction de destruction du menu
 *
 * @param menu une référence sur un pointeur sur un menu
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteMenu(menu_t **menu);

#endif