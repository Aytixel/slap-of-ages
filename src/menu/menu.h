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
    SDL_Color light_text_color;
    SDL_Color dark_text_color;
    SDL_Color selected_button_color;
    SDL_Color join_button_color;
    SDL_Color quit_button_color;
    sprite_t *background_sprite;
    button_t *join_button;
    button_t *quit_button;
    button_t *pseudo_button;
    button_t *port_button;
    button_t *hostname_button;
    textbox_t *hostname_textbox;
    textbox_t *port_textbox;
    textbox_t *pseudo_textbox;
    SDL_Rect RectIp;
    SDL_Rect RectPort;
    SDL_Rect RectPseudo;
    char hostname_input_text[1024];
    char port_input_text[6];
    char pseudo_input_text[64];
    int widthIp;
    int widthPort;
    int widthPseudo;
    int heightIp;
    int heightPort;
    int heightPseudo;
} menu_t;

extern menu_t *createMenu(window_t *window, client_game_data_t *game_data);

/**
 * @brief Fonction de création du menu multijoueur
 *
 * @return int
 */
extern int menuEventHandler(client_game_data_t *game_data, SDL_Event *event, menu_t *menu);

extern int menuRenderer(window_t *window, menu_t *menu);

extern int deleteMenu(menu_t **menu);

#endif