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
 * @brief Enumération représentant le point d'origin d'un transformation 2D
 *
 */
typedef enum
{
    TRANSFORM_ORIGIN_CENTER,       /**< centre*/
    TRANSFORM_ORIGIN_TOP_LEFT,     /**< coin haut gauche*/
    TRANSFORM_ORIGIN_TOP_RIGHT,    /**< coin haut droit*/
    TRANSFORM_ORIGIN_BOTTOM_LEFT,  /**< coin bas gauche*/
    TRANSFORM_ORIGIN_BOTTOM_RIGHT, /**< coin bas gauche*/
    TRANSFORM_ORIGIN_TOP,          /**< côté haut*/
    TRANSFORM_ORIGIN_RIGHT,        /**< côté droite*/
    TRANSFORM_ORIGIN_BOTTOM,       /**< côté bas*/
    TRANSFORM_ORIGIN_LEFT,         /**< côté gauche*/
} transform_origin_e;

/**
 * @brief Structure contenant les données d'un sprite
 *
 */
typedef struct
{
    SDL_Texture *texture; /**< texture du sprite*/
    int width;            /**< largeur de la texture*/
    int height;           /**< longueur de la texture*/
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

extern SDL_Rect positionFromCenter(window_t *window, int width, int height, int x, int y, transform_origin_e origin);

extern SDL_Rect positionToCenter(window_t *window, int width, int height);

#endif