#ifndef __ANIMATION_H
#define __ANIMATION_H

#include <SDL2/SDL_image.h>
#include "window.h"

/**
 * @file animation.h
 * @brief Animation fonctions
 * @author Hôa Le Luet
 * @version 1.0
 * @date 03/03/2023
 *
 *
 */

/**
 * @brief   Structure d'une animation
 *
 * @param state_frame_count : tableau contenant le nombre d'images par état
 * @param state_count : nombre d'états
 * @param state_frames : tableau contenant les images par état
 * @param frame_tile_width : largeur d'une image
 * @param frame_tile_height : hauteur d'une image
 * @param timer : timer permettant de jouer une animation à une vitesse définie
 * @param sprite : image source de l'animation
 * @param current_state : état courant
 * @param current_frame : image courante
 * @return structure de type anim_t
 */

typedef struct
{
    int *state_frame_count;
    int state_count;

    SDL_Rect **state_frames;

    int frame_tile_width;
    int frame_tile_height;

    frame_timer_t *timer;

    sprite_t *sprite;

    int current_state;
    int current_frame;
    SDL_Rect frame_rect;
} anim_t;

/*Fonctions externes*/

/**
 * @brief   Crée une animation à partir de la taille d'une tuile, du nombre d'images par état, de l'image source et du nombre d'images par seconde
 *
 * @param tile_size : taille d'une tuile
 * @param state_frame_count : tableau contenant le nombre d'images par état
 * @param sprite : image source de l'animation
 * @param frame_rate : nombre d'images par seconde
 * @return un objet de type anim_t*
 */
extern anim_t *createAnim(int tile_size, int *state_frame_count, sprite_t *sprite, int frame_rate);

/**
 * @brief   Détruit une animation
 *
 * @param anim : animation à détruire
 * @return un int valant -1 si l'animation n'existe pas, 0 sinon
 */
extern int destroyAnim(anim_t **anim);

/**
 * @brief   Met à jour l'animation en modifiant l'état et la position de l'animation
 *
 * @param anim : animation à mettre à jour
 * @param new_state : nouvel état de l'animation
 * @param tile_size : taille d'une tuile
 * @param position : position de l'animation
 * @param window : la fenêtre où l'animation est affichée
 *
 * @return un int valant 1 si l'animation est terminée, -1 si l'animation n'existe pas, 0 sinon
 */
extern int updateAnim(anim_t *anim, int new_state, int tile_size, SDL_Point *position, window_t *window);

#endif