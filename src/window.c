/**
 * @file window.c
 * @author Lucas Dureau
 * @brief Permets de gérer la fenêtre
 * @version 0.1
 * @date 08/02/2023
 *
 */

#include "stdio.h"
#include "stdlib.h"
#include "window.h"

/**
 * @brief Créer une fenêtre
 *
 * Définis :
 *  - la taille minimale de la fenêtre, comme étant la taille passé en paramètre.
 *  - la couleur de nettoyage au noir.
 *  - la fenêtre comme redimensionnable.
 *
 * Puis nettoie la fenêtre.
 *
 * @param title nom de la fenêtre
 * @param width largeur de la fenêtre
 * @param height longueur de la fenêtre
 * @return un pointer sur une **fenêtre**
 */
extern window_t *createWindow(char *title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "(Erreur): Initialisation d'SDL2 impossible : %s\n", SDL_GetError());

        return NULL;
    }

    if (TTF_Init() < 0)
    {
        fprintf(stderr, "(Erreur): Initialisation d'SDL2_ttf impossible : %s\n", TTF_GetError());

        SDL_Quit();

        return NULL;
    }

    window_t *window = malloc(sizeof(window_t));

    if ((window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) == NULL)
    {
        fprintf(stderr, "(Erreur): Création de la fenêtre impossible : %s\n", SDL_GetError());

        TTF_Quit();
        SDL_Quit();

        return NULL;
    }

    if ((window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED)) == NULL)
    {
        fprintf(stderr, "(Erreur): Création du renderer impossible : %s\n", SDL_GetError());

        SDL_DestroyWindow(window->window);
        TTF_Quit();
        SDL_Quit();

        return NULL;
    }

    window->width = width;
    window->height = height;

    SDL_SetWindowMinimumSize(window->window, width, height);
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
    SDL_RenderClear(window->renderer);
    SDL_RenderPresent(window->renderer);

    return window;
}

/**
 * @brief Détruit une fenêtre
 *
 * @param window une référence d'un pointeur sur une fenêtre
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
int destroyWindow(window_t **window)
{
    if (window == NULL || *window == NULL)
        return -1;

    SDL_DestroyRenderer((*window)->renderer);
    SDL_DestroyWindow((*window)->window);
    TTF_Quit();
    SDL_Quit();

    free(*window);
    *window = NULL;

    return 0;
}