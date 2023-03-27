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
#include "client/game_data.h"

// Structure pour stocker information du menu_test
typedef struct
{
    TTF_Font *text_font;
    TTF_Font *textbox_font;
    sprite_t *background_sprite;
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
 * @param window
 * @param game_data
 * @return menu_t*
 */
extern menu_t *createMenu(window_t *window, client_game_data_t *game_data);

/**
 * @brief Fonction d'écoute des événements du menu
 *
 * @param game_data
 * @param event
 * @param menu
 * @return int
 */
extern int menuEventHandler(client_game_data_t *game_data, SDL_Event *event, menu_t *menu);

/**
 * @brief Fonction d'affichage du menu
 *
 * @param window
 * @param menu
 * @return int
 */
extern int menuRenderer(window_t *window, menu_t *menu);

/**
 * @brief Fonction de destruction du menu
 *
 * @param menu
 * @return int
 */
extern int deleteMenu(menu_t **menu);

#endif