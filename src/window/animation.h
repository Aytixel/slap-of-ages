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
 * @brief   Enumération des états d'une animation
 * @details Les états sont les suivants :
 *         - ANIMATION_IDLE : l'animation est en pause
 *        - ANIMATION_SPAWN : l'animation apparait
 *       - ANIMATION_DESPAWN : l'animation disparait
 *     - ANIMATION_DELETE : l'animation est supprimée
 *   - ANIMATION_CURRENT : l'animation est en cours d'affichage
 * @param ANIMATION_IDLE
 * @param ANIMATION_SPAWN
 * @param ANIMATION_DESPAWN
 * @param ANIMATION_DELETE
 * @param ANIMATION_CURRENT
 * @return enum
 */

typedef enum
{
    ANIMATION_IDLE,
    ANIMATION_SPAWN,
    ANIMATION_DESPAWN,
    ANIMATION_DELETE,
    ANIMATION_CURRENT

} portal_e;

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

    SDL_Texture *sprite;

    int current_state;
    int current_frame;
} anim_t;

/*Fonctions externes*/

extern anim_t *createAnim(int max_frames, int *state_frame_count, int state_count, SDL_Texture *sprite, SDL_Surface *dim, SDL_Rect *size);