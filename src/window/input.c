/**
 * @file input.c
 * @author Arthur Dureau
 * @brief Fichier de création des boutons et des textes
 * @version 1.0
 * @date 03/03/2023
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "input.h"
#include "window.h"

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

void createButton(TTF_Font *font, const char* buttonText, SDL_Color color, float buttonXRatio, float buttonYRatio, float buttonWidthRatio, float buttonHeightRatio, button_t *button, window_t *window) {
    // Création de la surface du bouton
    button->surface = TTF_RenderText_Solid(font, buttonText, color);

    // Création de la texture du bouton à partir de la surface
    button->texture = SDL_CreateTextureFromSurface(window->renderer, button->surface);

    // Calcul des dimensions du bouton en pixels
    button->rect.w = (int)(window->width * buttonWidthRatio);
    button->rect.h = (int)(window->height * buttonHeightRatio);

    // Calcul de la position du bouton en pixels
    button->rect.x = (int)(window->width * buttonXRatio);
    button->rect.y = (int)(window->height * buttonYRatio);

    // Initialisation du texte du bouton
    button->text = strdup(buttonText);
}




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

void createTextbox(TTF_Font* font, SDL_Color color, SDL_Rect rect, Textbox_t* textbox, window_t *window) {
    textbox->surface = TTF_RenderText_Solid(font, " ", color);
    textbox->texture = SDL_CreateTextureFromSurface(window->renderer, textbox->surface);
    SDL_FreeSurface(textbox->surface);
    memcpy(&(textbox->rect), &rect, sizeof(SDL_Rect));
    SDL_SetTextInputRect(&(textbox->rect));
}

/**
 * @brief Fonction de détection du clic de la souris dans un rectangle
 * 
 * @param event 
 * @param rect 
 * @return int 
 */

int isMouseClickInRect(SDL_Event event, SDL_Rect rect) {
  if (event.type == SDL_MOUSEBUTTONDOWN &&
      event.button.x >= rect.x && event.button.x <= rect.x + rect.w &&
      event.button.y >= rect.y && event.button.y <= rect.y + rect.h) {
    return 1;
  }
  return 0;
}

/**
 * @brief Fonction de récupération de la position de la souris
 * 
 * @return SDL_Point 
 */

SDL_Point getMousePosition()
{
  int mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  // printf("Mouse position : (%d, %d)\n", mouseX, mouseY);
  return (SDL_Point){mouseX, mouseY};
}