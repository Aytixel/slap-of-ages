#ifndef __TEST_MENU_H
#define __TEST_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
    SDL_Surface* surface;  // nouvelle variable
    char* text;
} Button;

typedef struct {
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect rect;
} Textbox;

extern void createButton(SDL_Renderer *renderer, TTF_Font *font, const char* buttonText, SDL_Color color, float buttonXRatio, float buttonYRatio, float buttonWidthRatio, float buttonHeightRatio, Button *button);

extern int menu();

#endif