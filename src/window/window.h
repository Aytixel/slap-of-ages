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
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

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
    SDL_Window *window;        /**< fenêtre SDL2*/
    SDL_Renderer *renderer;    /**< moteur de rendue de la fenêtre*/
    int original_width;        /**< largeur originale de la fenêtre*/
    int original_height;       /**< longueur originale de la fenêtre*/
    int width;                 /**< largeur de la fenêtre*/
    int height;                /**< longueur de la fenêtre*/
    float width_scale_factor;  /**< coefficient de largeur*/
    float height_scale_factor; /**< coefficient de longueur*/
} window_t;

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

extern window_t *createWindow(char *title, int width, int height);

/**
 * @brief Met à jours la taille de la fenêtre
 *
 * @param window un pointeur sur une fenêtre
 * @param event un pointeur sur event
 */
extern void updateWindowSize(window_t *window, SDL_Event *event);

/**
 * @brief Détruit une fenêtre
 *
 * @param window une référence d'un pointeur sur une fenêtre
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int destroyWindow(window_t **window);

/**
 * @brief Charge une police depuis un fichier
 *
 * @param path chemin vers le fichier de la police
 * @param pt_size taille de la police
 * @return un pointer sur une **TTF_Font**
 */
extern TTF_Font *loadFont(char *path, int pt_size);

/**
 * @brief Charge un sprite depuis une image
 *
 * @param window un pointeur sur une fenêtre
 * @param path chemin vers le fichier du sprite
 * @return un pointer sur un **sprite**
 */
extern sprite_t *loadSprite(window_t *window, char *path);

/**
 * @brief Génère un sprite à partir d'un texte
 *
 * @param window un pointeur sur une fenêtre
 * @param font un pointeur sur une police d'écriture
 * @param text le texte à génèrer
 * @param color couleur du texte à génèrer
 * @return un pointer sur un **sprite**
 */
extern sprite_t *createTextSprite(window_t *window, TTF_Font *font, char *text, SDL_Color color);

/**
 * @brief Détruit un sprite
 *
 * @param sprite une référence d'un pointeur sur un sprite
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int destroySprite(sprite_t **sprite);

/**
 * @brief Charge un audio depuis un fichier
 *
 * @param path chemin vers le fichier audio
 * @return un pointer sur un **Mix_Music**
 */
extern Mix_Music *loadMusic(char *path);

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
extern SDL_Rect positionFromCenter(window_t *window, int width, int height, int x, int y, transform_origin_e origin);

/**
 * @brief Calcule la position au centre de l'écran
 *
 * @param window un pointeur sur une fenêtre
 * @param width largeur de l'élément à afficher
 * @param height hauteur de l'élément à afficher
 * @return une **position** sur la fenêtre
 */
extern SDL_Rect positionToCenter(window_t *window, int width, int height);

#endif