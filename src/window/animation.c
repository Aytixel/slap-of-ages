#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "timer/timer.h"
#include "animation.h"
#include "window.h"

/**
 * @file animation.c
 * @brief Gestion des animations
 * @author Hôa Le Luet
 * @version 1.0
 * @date 03/03/2023
 *
 *
 */

/**
 * @brief   Initialise les images d'une animation
 *
 * @param tab
 * @param nb_frames
 * @param src
 * @param line
 * @param nb_lines
 * @return int
 */

int initFrames(SDL_Rect *tab, int nb_frames, SDL_Surface *src, int line, int nb_lines)
{

    for (int i = 0; i < nb_frames; i++)
    {

        tab[i].x = (src->w / nb_frames) * i;
        tab[i].y = (src->h / nb_lines) * line;

        tab[i].w = src->w / nb_frames;
        tab[i].h = src->h / nb_lines;
    }
    return 0;
}

/**
 * @brief   Crée une animation
 *
 * @param max_frames
 * @param state_frame_count
 * @param state_count
 * @param sprite
 * @param size
 * @return anim_t*
 */

extern anim_t *createAnim(int max_frames, int *state_frame_count, int state_count, sprite_t *sprite, SDL_Rect *size)
{

    anim_t *anim = malloc(sizeof(anim_t));

    anim->state_frame_count = malloc(sizeof(int) * state_count);

    for (int i = 0; i < state_count; i++)
    {
        anim->state_frame_count[i] = state_frame_count[i];
    }

    anim->state_count = state_count;
    anim->current_frame = 0;
    anim->current_state = 0;
    anim->sprite = sprite;
    anim->size = size;

    anim->anims = malloc(sizeof(SDL_Rect *) * state_count);

    for (int i = 0; i < state_count; i++)
    {
        anim->anims[i] = malloc(sizeof(SDL_Rect) * state_frame_count[i]);
        initFrames(anim->anims[i], state_frame_count[i], sprite->surface, i, state_count);
    }

    return anim;
}

/**
 * @brief   Détruit une animation
 *
 * @param anim
 * @return int
 */

extern int destroyAnim(anim_t **anim)
{
    if (anim == NULL || *anim == NULL)
        return -1;

    free((*anim)->state_frame_count);

    for (int i = 0; i < (*anim)->state_count; i++)
    {
        free((*anim)->anims[i]);
    }

    destroySprite(&(*anim)->sprite);

    free((*anim)->anims);
    free(*anim);
    anim = NULL;

    return 0;
}

/**
 * @brief   Met à jour l'animation
 *
 * @param anim
 * @param new_state
 * @param window
 */

extern void updateAnim(anim_t *anim, int new_state, window_t *window)
{
    if (anim->current_state != new_state)
    {
        anim->current_state = new_state;
        anim->current_frame = 0;
    }

    SDL_RenderCopy(window->renderer, anim->sprite->texture, &anim->anims[anim->current_state][anim->current_frame], anim->size);

    anim->current_frame++;
    if (anim->current_frame >= anim->state_frame_count[anim->current_state])
    {
        anim->current_frame = 0;
    }
}
