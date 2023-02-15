/**
 * @file window.c
 * @author Lucas Dureau
 * @brief Permets de gérer la fenêtre
 * @version 0.1
 * @date 08/02/2023
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
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
        free(window);

        return NULL;
    }

    if ((window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED)) == NULL)
    {
        fprintf(stderr, "(Erreur): Création du renderer impossible : %s\n", SDL_GetError());

        SDL_DestroyWindow(window->window);
        TTF_Quit();
        SDL_Quit();
        free(window);

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
extern int destroyWindow(window_t **window)
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

/**
 * @brief Charge un sprite depuis une image
 *
 * @param window un pointeur sur une fenêtre
 * @param path chemin vers le fichier du sprite
 * @return un pointer sur un **sprite**
 */
extern sprite_t *loadSprite(window_t *window, char *path)
{
    sprite_t *sprite = malloc(sizeof(sprite_t));

    if ((sprite->surface = IMG_Load(path)) == NULL)
    {
        fprintf(stderr, "(Erreur): Impossible de charger le sprite \"%s\" : %s\n", path, SDL_GetError());

        free(sprite);

        return NULL;
    }

    if ((sprite->texture = SDL_CreateTextureFromSurface(window->renderer, sprite->surface)) == NULL)
    {
        fprintf(stderr, "(Erreur): Création de la texture pour le sprite \"%s\" impossible : %s\n", path, SDL_GetError());

        SDL_FreeSurface(sprite->surface);
        free(sprite);

        return NULL;
    }

    return sprite;
}

/**
 * @brief Génère un sprite à partir d'un texte
 *
 * @param window un pointeur sur une fenêtre
 * @param font un pointeur sur une police d'écriture
 * @param text le texte à génèrer
 * @param color couleur du texte à génèrer
 * @return un pointer sur un **sprite**
 */
extern sprite_t *createTextSprite(window_t *window, TTF_Font *font, char *text, SDL_Color color)
{
    sprite_t *sprite = malloc(sizeof(sprite_t));

    if ((sprite->surface = TTF_RenderText_Solid(font, text, color)) == NULL)
    {
        fprintf(stderr, "(Erreur): Impossible de créer le sprite du texte \"%s\" : %s\n", text, TTF_GetError());

        free(sprite);

        return NULL;
    }

    if ((sprite->texture = SDL_CreateTextureFromSurface(window->renderer, sprite->surface)) == NULL)
    {
        fprintf(stderr, "(Erreur): Création de la texture pour le sprite du texte \"%s\" impossible : %s\n", text, SDL_GetError());

        SDL_FreeSurface(sprite->surface);
        free(sprite);

        return NULL;
    }

    return sprite;
}

/**
 * @brief Détruit un sprite
 *
 * @param sprite une référence d'un pointeur sur un sprite
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int destroySprite(sprite_t **sprite)
{
    if (sprite == NULL || *sprite == NULL)
        return -1;

    SDL_DestroyTexture((*sprite)->texture);
    SDL_FreeSurface((*sprite)->surface);
    free(*sprite);
    *sprite = NULL;

    return 0;
}

/**
 * @brief Calcule une position par rapport au centre de l'écran
 *
 * @param window un pointeur sur une fenêtre
 * @param width largeur de l'élément à afficher
 * @param height hauteur de l'élément à afficher
 * @param x décalage en x depuis le centre
 * @param y décalage en y depuis le centre
 * @param origin point d'origine sur lequel est centrée l'élément
 * @return une **position** sur la fenêtre
 */
extern SDL_Rect positionFromCenter(window_t *window, int width, int height, int x, int y, transform_origin_e origin)
{
    int offset_x = 0, offset_y = 0;

    switch (origin)
    {
    case TRANSFORM_ORIGIN_TOP_LEFT:
        break;
    case TRANSFORM_ORIGIN_TOP_RIGHT:
        offset_x = -width;
        break;
    case TRANSFORM_ORIGIN_BOTTOM_RIGHT:
        offset_x = -width;
    case TRANSFORM_ORIGIN_BOTTOM_LEFT:
        offset_y = -height;
        break;
    case TRANSFORM_ORIGIN_CENTER:
    default:
        offset_x = -width / 2;
        offset_y = -height / 2;
        break;
    }

    SDL_Rect position = {window->width / 2 + offset_x + x, window->height / 2 + offset_y + y, width, height};

    return position;
}

/**
 * @brief Calcule une position par rapport au centre de l'écran à partir d'une **surface**
 *
 * @param window un pointeur sur une fenêtre
 * @param surface un pointeur sur une surface
 * @param x décalage en x depuis le centre
 * @param y décalage en y depuis le centre
 * @param origin point d'origine sur lequel est centrée l'élément
 * @return une **position** sur la fenêtre
 */
extern SDL_Rect surfaceFromCenter(window_t *window, SDL_Surface *surface, int x, int y, transform_origin_e origin)
{
    return positionFromCenter(window, surface->w, surface->h, x, y, origin);
}

/**
 * @brief Calcule la position au centre de l'écran
 *
 * @param window un pointeur sur une fenêtre
 * @param width largeur de l'élément à afficher
 * @param height hauteur de l'élément à afficher
 * @return une **position** sur la fenêtre
 */
extern SDL_Rect positionToCenter(window_t *window, int width, int height)
{
    return positionFromCenter(window, width, height, 0, 0, TRANSFORM_ORIGIN_CENTER);
}

/**
 * @brief Calcule la position au centre de l'écran à partir d'une **surface**
 *
 * @param window un pointeur sur une fenêtre
 * @param surface un pointeur sur une surface
 * @return une **position** sur la fenêtre
 */
extern SDL_Rect surfaceToCenter(window_t *window, SDL_Surface *surface)
{
    return positionToCenter(window, surface->w, surface->h);
}