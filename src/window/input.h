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
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
    char *text;
} textbox_t;

/**
 * @brief Fonction de création des boutons
 *
 * @param renderer
 * @param font
 * @param buttonText
 * @param color
 * @param buttonXRatio
 * @param buttonYRatio
 * @param buttonWidthRatio
 * @param buttonHeightRatio
 * @param button
 * @param width
 * @param height
 *
 * @return void
 */
extern button_t *createButton(window_t *window, TTF_Font *font, char *text, SDL_Color color, SDL_Color selected_color);

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
 * @param renderer
 * @param font
 * @param color
 * @param x
 * @param y
 * @param w
 * @param h
 * @param rect
 *
 * @return void
 */
extern textbox_t *createTextbox(TTF_Font *font, SDL_Color color, SDL_Rect rect, window_t *window);

/**
 * @brief Fonction de mise à jour des textes box (texte de saisie)
 *
 * @param event
 * @param font
 * @param inputText
 * @param width
 * @param height
 *
 * @return void
 */
extern void updateTextboxText(SDL_Event event, TTF_Font *font, char *inputText, int *width, int *height);

/**
 * @brief Fonction de destruction des textes box (texte de saisie)
 *
 * @param textbox
 *
 * @return void
 */
extern void destroyTextbox(textbox_t **textbox);

extern void drawRect(SDL_Renderer *renderer, SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

extern void drawClear(SDL_Renderer *renderer);

extern void drawText(SDL_Renderer *renderer, TTF_Font *font, char *text, SDL_Rect rect, SDL_Color color);

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