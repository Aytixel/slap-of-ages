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
 * @details La structure d'une animation est composée des éléments suivants :
 *         - state_frame_count : tableau des images par état
 *         - state_count : nombre d'états
 *         - anims : tableau des animations
 *         - size : taille de l'animation
 *         - sprite : image source de l'animation
 *         - current_state : état courant
 *         - current_frame : image courante
 * @param state_frame_count
 * @param state_count
 * @param anims
 * @param size
 * @param sprite
 * @param current_state
 * @param current_frame
 * @return struct
 */

typedef struct
{

    int *state_frame_count;
    int state_count;

    SDL_Rect **anims;
    SDL_Rect *size;

    sprite_t *sprite;

    int current_state;
    int current_frame;
} anim_t;

/*Fonctions externes*/

extern anim_t *createAnim(int max_frames, int *state_frame_count, int state_count, sprite_t *sprite, SDL_Rect *size);

extern int destroyAnim(anim_t **anim);

extern void updateAnim(anim_t *anim, int new_state, window_t *window);