/**
 * @file window.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de window.c
 * @version 0.1
 * @date 08/02/2023
 *
 */

#ifndef __WINDOW_H
#define __WINDOW_H

#include <SDL2/SDL.h>

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

extern int destroyWindow(window_t **window);

#endif