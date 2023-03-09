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

/**
 * @brief Structure contenant les données d'un bouton
 * 
 */

typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
    SDL_Surface* surface;
    char* text;
} button_t;

/**
 * @brief Structure contenant les données d'une textbox
 * 
 */

typedef struct {
    SDL_Surface* surface; 
    SDL_Texture* texture;
    SDL_Rect rect;
    char* text;
} Textbox_t;

extern void createButton(SDL_Renderer *renderer, TTF_Font *font, const char* buttonText, SDL_Color color, float buttonXRatio, float buttonYRatio, float buttonWidthRatio, float buttonHeightRatio, button_t *button, int windowWidth, int windowHeight);

extern void createTextbox(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, int x, int y, int w, int h, Textbox_t* textbox);

extern int isMouseClickInRect(SDL_Event event, SDL_Rect rect) ;

extern SDL_Point getMousePosition();

#endif