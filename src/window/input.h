/**
 * @file input.h
 * @author Arthur Dureau, Lucas Dureau
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
    sprite_t *sprite;         /**< sprite du texte*/
    SDL_Rect rect;            /**< rectangle d'affichage*/
    int is_selected;          /**< le bouton est-il sélectionner*/
    TTF_Font *font;           /**< police d'écriture*/
    SDL_Color color;          /**< couleur du texte*/
    SDL_Color selected_color; /**< couleur du texte sélectionner*/
    char *text;
} button_t;

/**
 * @brief Structure contenant les données d'une textbox
 *
 */
typedef struct
{
    sprite_t *sprite;           /**< sprite du texte*/
    SDL_Rect rect;              /**< rectangle de la boite de texte*/
    SDL_Rect text_rect;         /**< rectangle de texte*/
    int max_width;              /**< largueur maximum*/
    int character_limit;        /**< limite de caractère à afficher*/
    int character_diplay_count; /**< nombre de caractère à afficher*/
    TTF_Font *font;             /**< police d'écriture*/
    SDL_Color color;            /**< couleur du texte*/
    char *text;
} textbox_t;

/**
 * @brief Fonction de création des boutons
 *
 * @param window un pointeur sur une fenêtre
 * @param font police d'écriture
 * @param text
 * @param color couleur du texte
 * @param selected_color couleur du text séléctionner
 * @return un pointeur sur un bouton
 */
extern button_t *createButton(window_t *window, TTF_Font *font, char *text, SDL_Color color, SDL_Color selected_color);

/**
 * @brief Fonction d'affichage d'un bouton
 *
 * @param window un pointeur sur une fenêtre
 * @param button un pointeur sur un bouton
 */
extern void renderButton(window_t *window, button_t *button);

/**
 * @brief Fonction de destruction des boutons
 *
 * @param button une référence sur un pointeur sur un bouton
 */
extern void destroyButton(button_t **button);

/**
 * @brief Fonction de création des textes box (texte de saisie)
 *
 * @param window un pointeur sur une fenêtre
 * @param font police d'écriture
 * @param text
 * @param color couleur du texte
 * @param character_limit nombre maximum de caractère
 * @return un pointeur sur une boite de texte
 */
textbox_t *createTextbox(window_t *window, TTF_Font *font, char *text, SDL_Color color, int character_limit);

/**
 * @brief Fonction d'affichage d'une boite de texte
 *
 * @param window un pointeur sur une fenêtre
 * @param textbox un pointeur sur une boite de texte
 */
extern void renderTextbox(window_t *window, textbox_t *textbox);

/**
 * @brief Fonction de mise à jour des textes box (texte de saisie)
 *
 * @param event un pointeur sur les événements
 * @param textbox un pointeur sur une boite de texte
 */
extern void updateTextbox(SDL_Event *event, textbox_t *textbox);

/**
 * @brief Fonction de destruction des textes box (texte de saisie)
 *
 * @param textbox une référence sur un pointeur sur une boite de texte
 */
extern void destroyTextbox(textbox_t **textbox);

/**
 * @brief Fonction d'affichage d'un rectangle
 *
 * @param renderer moteur de rendue de la fenêtre
 * @param rect rectangle de dessin
 * @param r rouge
 * @param g vert
 * @param b bleu
 * @param a transparence
 */
extern void drawRect(SDL_Renderer *renderer, SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/**
 * @brief Fonction de détection de clic dans une zone
 *
 * @param event événements
 * @param rect rectangle de détection
 * @param button quel bouton est détecter
 * @param type quel état du bouton est détecter
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