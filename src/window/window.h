/**
 * @file window.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de window/window.c
 * @version 0.1
 * @date 08/02/2023
 *
 */

#ifndef __WINDOW_H
#define __WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief Structure contenant les données d'un sprite
 *
 */
typedef struct
{
    SDL_Texture *texture; /**< texture du sprite*/
    SDL_Surface *surface; /**< surface du sprite*/
} sprite_t;

/**
 * @brief Structure contenant toutes les données de la fenêtre
 *
 */
typedef struct
{
    SDL_Window *window;     /**< fenêtre SDL2*/
    SDL_Renderer *renderer; /**< moteur de rendue de la fenêtre*/
    int width;              /**< largeur de la fenêtre*/
    int height;             /**< longueur de la fenêtre*/
} window_t;

extern window_t *createWindow(char *title, int width, int height);

extern void updateWindowSize(window_t *window, SDL_Event *event);

extern int destroyWindow(window_t **window);

extern sprite_t *loadSprite(window_t *window, char *path);

extern sprite_t *createTextSprite(window_t *window, TTF_Font *font, char *text, SDL_Color color);

extern int destroySprite(sprite_t **sprite);

#endif