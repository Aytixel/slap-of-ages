#include <SDL2/SDL_image.h>

// commentaires doxygen

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

/**
 * @brief   Structure d'un élément d'une liste d'animation
 * @details La structure d'un élément d'une liste d'animation est composée des éléments suivants :
 *         - anim : animation
 *         - next : élément suivant
 * @param anim
 * @param next
 * @return struct
 */

typedef struct anim_elem_s
{

    anim_t *anim;
    struct anim_elem_s *next;

} anim_elem_t;

/**
 * @brief   Structure d'une liste d'animation
 * @details La structure d'une liste d'animation est composée des éléments suivants :
 *         - flag : élément de début de liste
 *         - current : élément courant
 * @param flag
 * @param current
 * @return struct
 */

typedef struct
{

    anim_elem_t *flag;
    anim_elem_t *current;

} anim_list_t;