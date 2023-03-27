/**
 * @file test_menu.h
 * @author Arthur Dureau
 * @brief Implémentation des prototypes de tests/test_menu.c
 * @version 1.0
 * @date 03/03/2023
 *
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

// Structure pour stocker information du menu_test
typedef struct
{
    TTF_Font *text_font;
    TTF_Font *textbox_font;
    anim_t *book_animation;
    book_anim_state_e book_animation_state;
    button_t *join_button;
    button_t *quit_button;
    sprite_t *hostname_label;
    sprite_t *port_label;
    sprite_t *pseudo_label;
    textbox_t *hostname_textbox;
    textbox_t *port_textbox;
    textbox_t *pseudo_textbox;
    SDL_Rect hostname_rect;
    SDL_Rect port_rect;
    SDL_Rect pseudo_rect;
} menu_t;

/**
 * @brief Fonction de création du menu
 *
 * @param window un pointeur sur une fenêtre
 * @param game_data un pointeur sur les données du jeux
 * @return un pointeur sur un menu
 */
extern menu_t *createMenu(window_t *window, client_game_data_t *game_data);

/**
 * @brief Fonction d'écoute des événements du menu
 *
 * @param game_data un pointeur sur les données du jeux
 * @param event un pointeur sur les événements
 * @param menu un pointeur sur un menu
 * @return 1 si le bouton join est cliquer, 0 sinon
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
 * @return int
 */
extern int deleteMenu(menu_t **menu);

#endif