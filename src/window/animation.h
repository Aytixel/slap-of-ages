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
} anim_t;

/*Fonctions externes*/

extern anim_t *createAnim(int tile_size, int *state_frame_count, sprite_t *sprite, int frame_rate);

extern int destroyAnim(anim_t **anim);

extern int updateAnim(anim_t *anim, int new_state, int tile_size, SDL_Point *position, window_t *window);