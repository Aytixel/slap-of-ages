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

anim_t *createAnim(int max_frames, int *state_frame_count, int state_count, SDL_Texture *sprite, SDL_Surface *dim, SDL_Rect *size)
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

/**
 * @brief   Détruit une animation
 *
 * @param anim
 * @return int
 */

int destroyAnim(anim_t **anim)
{
    free((*anim)->state_frame_count);

    for (int i = 0; i < (*anim)->state_count; i++)
    {
        free((*anim)->anims[i]);
    }

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

void updateAnim(anim_t *anim, portal_e new_state, window_t *window)
{
    if (anim->current_state != new_state)
    {
        anim->current_state = new_state;
        anim->current_frame = 0;
    }

    SDL_RenderCopy(window->renderer, anim->sprite, &anim->anims[anim->current_state][anim->current_frame], anim->size);

    anim->current_frame++;

    if (new_state == ANIMATION_DELETE)
    {

        destroyAnim(&anim);
    }
}

/**
 * @brief   Crée et initialise une liste d'animation
 *
 * @return anim_list_t*
 */

anim_list_t *createAnimList()
{

    anim_list_t *new = malloc(sizeof(anim_list_t));

    new->flag = malloc(sizeof(anim_elem_t));
    new->flag->next = new->flag;
    new->current = new->flag;
    new->flag->anim = NULL;

    return new;
}

/**
 * @brief   Ajoute une animation à la liste
 *
 * @param list
 * @param anim
 */

void addAnimList(anim_list_t *list, anim_t *anim)
{

    anim_elem_t *new = malloc(sizeof(anim_elem_t));

    new->anim = anim;
    new->next = list->flag;
    list->current->next = new;
    list->current = new;
}

/**
 * @brief   Supprime une animation de la liste
 *
 * @param list
 * @param anim
 */

void removeAnimList(anim_list_t *list, anim_t *anim)
{

    anim_elem_t *current = list->flag->next;
    anim_elem_t *previous = list->flag;

    while (current != list->flag)
    {
        if (current->anim == anim)
        {
            previous->next = current->next;
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }
}

/**
 * @brief   Met à jour la liste d'animation
 *
 * @param list
 * @param window
 */

void updateAnimList(anim_list_t *list, window_t *window)
{

    anim_elem_t *current = list->flag->next;

    while (current != list->flag)
    {
        updateAnim(current->anim, current->anim->current_state, window);
        current = current->next;
    }
}