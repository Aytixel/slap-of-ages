#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../timer/timer.h"
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
        tab[i].y = (src->h / nb_lines) * (line - 1);

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
 * @param dim
 * @param size
 * @return anim_t*
 */

extern anim_t *createAnim(int max_frames, int *state_frame_count, int state_count, SDL_Texture *sprite, SDL_Surface *dim, SDL_Rect *size)
{

    anim_t *new = malloc(sizeof(anim_t));

    new->state_frame_count = malloc(sizeof(int) * state_count);

    for (int i = 0; i < state_count; i++)
    {
        new->state_frame_count[i] = state_frame_count[i];
    }

    new->state_count = state_count;
    new->current_frame = 0;
    new->sprite = sprite;
    new->size = size;

    new->anims = malloc(sizeof(SDL_Rect *) * state_count);

    for (int i = 0; i < state_count; i++)
    {
        new->anims[i] = malloc(sizeof(SDL_Rect) * state_frame_count[i]);
        initFrames(new->anims[i], state_frame_count[i], dim, i, state_count);
    }

    return new;
}
