/**
 * @file animation.h
 * @brief Animation fonctions
 * @author Hôa Le Luet
 * @version 1.0
 * @date 03/03/2023
 *
 *
 */
#ifndef __ANIMATION_H
#define __ANIMATION_H

#include <SDL2/SDL_image.h>
#include "window.h"

typedef struct
{
    int *state_frame_count; /**< tableau contenant le nombre d'images par état*/
    int state_count;        /**< nombre d'états*/

    SDL_Rect **state_frames; /**< tableau contenant les images par état*/

    int frame_tile_width;  /**< largeur d'une image*/
    int frame_tile_height; /**< hauteur d'une image*/

    frame_timer_t *timer; /**< timer permettant de jouer une animation à une vitesse définie*/

    sprite_t *sprite; /**< image source de l'animation*/

    int current_state;   /**< état courant*/
    int current_frame;   /**< image courante*/
    SDL_Rect frame_rect; /**< rectangle d'affichage de l'animation en cour*/
} anim_t;

/**
 * @brief Crée une animation à partir de la taille d'une tuile, du nombre d'images par état, de l'image source et du nombre d'images par seconde
 *
 * @param tile_size taille d'une tuile
 * @param state_frame_count tableau contenant le nombre d'images par état
 * @param sprite image source de l'animation
 * @param frame_rate nombre d'images par seconde
 * @return un objet de type anim_t*
 */
extern anim_t *createAnim(int tile_size, int *state_frame_count, sprite_t *sprite, int frame_rate);

/**
 * @brief Détruit une animation
 *
 * @param anim animation à détruire
 * @return un int valant -1 si l'animation n'existe pas, 0 sinon
 */
extern int destroyAnim(anim_t **anim);

/**
 * @brief Met à jour l'animation en modifiant l'état et la position de l'animation
 *
 * @param anim animation à mettre à jour
 * @param new_state nouvel état de l'animation
 * @param tile_size taille d'une tuile
 * @param position position de l'animation
 * @param window la fenêtre où l'animation est affichée
 *
 * @return un int valant 1 si l'animation est terminée, -1 si l'animation n'existe pas, 0 sinon
 */
extern int updateAnim(anim_t *anim, int new_state, int tile_size, SDL_Point *position, window_t *window);

#endif