/**
 * @file animation.c
 * @brief Gestion des animations
 * @author Hôa Le Luet, Lucas Dureau
 * @version 1.0
 * @date 03/03/2023
 *
 *
 */
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
 * @brief Initialise les images d'une animation
 *
 * @param tab tableau contenant les positions et tailles des différentes images de l'animation dans la source
 * @param nb_frames nombre d'images de l'animation
 * @param src image source de l'animation
 * @param line ligne contenant les différentes étapes d'une animation dans l'image source
 * @param nb_lines nombre de lignes dans l'image source
 */
void initFrames(SDL_Rect *tab, int max_frames, int nb_frames, sprite_t *sprite, int line, int nb_lines)
{

    for (int i = 0; i < nb_frames; i++)
    {

        tab[i].x = (sprite->width / max_frames) * i;
        tab[i].y = (sprite->height / nb_lines) * line;

        tab[i].w = sprite->width / max_frames;
        tab[i].h = sprite->height / nb_lines;
    }
}

extern anim_t *createAnim(int tile_size, int *state_frame_count, sprite_t *sprite, int frame_rate)
{

    int max_frames = 0;
    int state_count = 0;

    anim_t *anim = malloc(sizeof(anim_t));

    for (int i = 0; state_frame_count[i] > 0; i++)
    {
        state_count++;
    }

    anim->state_frame_count = malloc(sizeof(int) * state_count);

    for (int i = 0; i < state_count; i++)
    {
        if (state_frame_count[i] > max_frames)
            max_frames = state_frame_count[i];
    }

    for (int i = 0; i < state_count; i++)
    {
        anim->state_frame_count[i] = state_frame_count[i];
    }

    anim->state_count = state_count;
    anim->current_frame = 0;
    anim->current_state = 0;
    anim->sprite = sprite;
    anim->frame_rect.x = 0;
    anim->frame_rect.y = 0;
    anim->frame_rect.w = 0;
    anim->frame_rect.h = 0;

    anim->timer = createTimer(1000 / frame_rate);

    anim->state_frames = malloc(sizeof(SDL_Rect *) * state_count);

    for (int i = 0; i < state_count; i++)
    {
        anim->state_frames[i] = malloc(sizeof(SDL_Rect) * state_frame_count[i]);
        initFrames(anim->state_frames[i], max_frames, state_frame_count[i], sprite, i, state_count);
    }

    anim->frame_tile_width = anim->state_frames[0][0].w / tile_size;
    anim->frame_tile_height = anim->state_frames[0][0].h / tile_size;

    return anim;
}

extern int destroyAnim(anim_t **anim)
{
    if (anim == NULL || *anim == NULL)
        return -1;

    free((*anim)->state_frame_count);

    for (int i = 0; i < (*anim)->state_count; i++)
    {
        free((*anim)->state_frames[i]);
    }

    destroySprite(&(*anim)->sprite);
    deleteTimer(&(*anim)->timer);

    free((*anim)->state_frames);
    free(*anim);
    *anim = NULL;

    return 0;
}

extern int updateAnim(anim_t *anim, int new_state, int tile_size, SDL_Point *position, window_t *window, transform_origin_e origin)
{
    if (anim == NULL)
        return -1;

    if (anim->current_state != new_state)
    {
        anim->current_state = new_state;
        anim->current_frame = 0;
    }

    anim->frame_rect = positionFromCenter(window, anim->frame_tile_width * tile_size, anim->frame_tile_height * tile_size, position->x, position->y, origin);

    SDL_RenderCopy(window->renderer, anim->sprite->texture, &anim->state_frames[anim->current_state][anim->current_frame], &anim->frame_rect);

    if (checkTime(anim->timer))
    {
        anim->current_frame++;
        if (anim->current_frame >= anim->state_frame_count[anim->current_state])
        {
            anim->current_frame = 0;
            return 1;
        }
    }

    return 0;
}
