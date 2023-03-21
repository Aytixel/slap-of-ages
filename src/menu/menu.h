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

// Structure pour stocker l'ip et le port et le pseudo
typedef struct
{
    char *ip;
    int port;
    char *pseudo;
} connection_t;

/*
// Structure pour stocker les informations du menu_test (font, color, bouton, textbox, rect)    
typedef struct
{
    TTF_Font *font;
    SDL_Color color;
    button_t *button;
    Textbox_t *textbox;
    SDL_Rect rect;
} menu_test_t; 
*/

// Structure pour stocker information du menu_test
typedef struct {
    SDL_Texture* texturep;
    button_t buttonHost;
    button_t buttonJoin;
    button_t buttonQuitter;
    button_t buttonPseudo;
    button_t buttonPort;
    button_t buttonIp;
    Textbox_t textboxIp;
    Textbox_t textboxPort;
    Textbox_t textboxPseudo;
    SDL_Rect RectIp;
    SDL_Rect RectPort;
    SDL_Rect RectPseudo;
    window_t *window;
    TTF_Font *font;
    SDL_Color colorH;
    SDL_Color colorJ;
    SDL_Color colorQ;
    char * inputTextIp;
    char * inputTextPort;
    char * inputTextPseudo;
    int widthIp;
    int widthPort;
    int widthPseudo;
    int heightIp;
    int heightPort;
    int heightPseudo;
} menu_t;


extern void handleEvent(SDL_Event *event, window_t *window);

extern void renderDrawColor(SDL_Renderer *renderer);

extern SDL_Point getMousePosition();

extern int menu_event(connection_t *connection, SDL_Event *event, menu_t *menu);

extern int menu_renderer(window_t * window, menu_t *menu);

#endif