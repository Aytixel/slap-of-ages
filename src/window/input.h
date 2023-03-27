/**
 * @file input.h
 * @author Arthur Dureau
 * @brief Implémentation des prototypes de window/input.c
 * @version 1.0
 * @date 03/03/2023
 *
 *
 */

#ifndef __INPUT_H
#define __INPUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "window.h"

/**
 * @brief Structure contenant les données d'un bouton
 *
 */

typedef struct
{
    sprite_t *sprite;
    SDL_Rect rect;
    int is_selected;
    TTF_Font *font;
    SDL_Color color;
    SDL_Color selected_color;
    char *text;
} button_t;

/**
 * @brief Structure contenant les données d'une textbox
 *
 */

typedef struct
{
    sprite_t *sprite;
    SDL_Rect rect;
    SDL_Rect text_rect;
    int max_width;
    int character_limit;
    int character_diplay_count;
    TTF_Font *font;
    SDL_Color color;
    char *text;
} textbox_t;

/**
 * @brief Fonction de création des boutons
 *
 * @param window
 * @param font
 * @param text
 * @param color
 * @param selected_color
 * @return button_t*
 */
extern button_t *createButton(window_t *window, TTF_Font *font, char *text, SDL_Color color, SDL_Color selected_color);

/**
 * @brief Fonction d'affichage d'un bouton
 *
 * @param window
 * @param button
 */
extern void renderButton(window_t *window, button_t *button);

/**
 * @brief Fonction de destruction des boutons
 *
 * @param button
 *
 * @return void
 */
extern void destroyButton(button_t **button);

/**
 * @brief Fonction de création des textes box (texte de saisie)
 *
 * @param window
 * @param font
 * @param text
 * @param color
 * @param character_limit
 * @return textbox_t*
 */
textbox_t *createTextbox(window_t *window, TTF_Font *font, char *text, SDL_Color color, int character_limit);

/**
 * @brief Fonction d'affichage d'une boite de texte
 *
 * @param window
 * @param textbox
 */
extern void renderTextbox(window_t *window, textbox_t *textbox);

/**
 * @brief Fonction de mise à jour des textes box (texte de saisie)
 *
 * @param event
 * @param textbox
 */
extern void updateTextbox(SDL_Event *event, textbox_t *textbox);

/**
 * @brief Fonction de destruction des textes box (texte de saisie)
 *
 * @param textbox
 *
 * @return void
 */
extern void destroyTextbox(textbox_t **textbox);

/**
 * @brief Fonction d'affichage d'un rectangle
 *
 * @param renderer
 * @param rect
 * @param r
 * @param g
 * @param b
 * @param a
 */
extern void drawRect(SDL_Renderer *renderer, SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/**
 * @brief Fonction de détection de clic dans une zone
 *
 * @example isMouseClickInRect(event, button.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN)
 * @example isMouseClickInRect(event, button.rect, SDL_BUTTON_RIGHT, SDL_MOUSEBUTTONUP)
 *
 * @param event
 * @param rect
 * @param button
 * @param type
 * @return int
 */
extern int isMouseClickInRect(SDL_Event event, SDL_Rect rect, int button, int type);

/**
 * @brief Fonction de récupération de la position de la souris
 *
 * @return SDL_Point
 */
extern SDL_Point getMousePosition();

#endif