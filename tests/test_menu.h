/**
 * @file test_menu.h
 * @author Arthur Dureau
 * @brief Implémentation des prototypes de tests/test_menu.c
 * @version 1.0
 * @date 03/03/2023
 * 
 * 
 */

#ifndef __TEST_MENU_H
#define __TEST_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "window/window.h"

extern void handleEvent(SDL_Event *event, window_t *window);

extern int initialisationSDL();

extern void renderDrawColor(SDL_Renderer *renderer);

extern SDL_Point getMousePosition();

extern int menu();

#endif