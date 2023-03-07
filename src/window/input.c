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

void createButton(SDL_Renderer *renderer, TTF_Font *font, const char* buttonText, SDL_Color color, float buttonXRatio, float buttonYRatio, float buttonWidthRatio, float buttonHeightRatio, button_t *button, int windowWidth, int windowHeight) {
    // Création de la surface du bouton
    button->surface = TTF_RenderText_Solid(font, buttonText, color);

    // Création de la texture du bouton à partir de la surface
    button->texture = SDL_CreateTextureFromSurface(renderer, button->surface);

    // Calcul des dimensions du bouton en pixels
    button->rect.w = (int)(windowWidth * buttonWidthRatio);
    button->rect.h = (int)(windowHeight * buttonHeightRatio);

    // Calcul de la position du bouton en pixels
    button->rect.x = (int)(windowWidth * buttonXRatio);
    button->rect.y = (int)(windowHeight * buttonYRatio);

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

void createTextbox(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, int x, int y, int w, int h, Textbox_t* textbox) {
    textbox->surface = TTF_RenderText_Solid(font, " ", color);
    textbox->texture = SDL_CreateTextureFromSurface(renderer, textbox->surface);
    SDL_FreeSurface(textbox->surface);
    textbox->rect.x = x;
    textbox->rect.y = y;
    textbox->rect.w = w;
    textbox->rect.h = h;
    SDL_SetTextInputRect(&(textbox->rect));
}
