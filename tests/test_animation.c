#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "timer/timer.h"
#include "test_animation.h"
#include "window/window.h"

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

void updateAnim(anim_t *anim, portal_e current_state, window_t *window)
{
  if (current_state != ANIMATION_CURRENT || current_state != ANIMATION_DELETE)
  {
    anim->current_state = (int)current_state;

    return;
  }

  SDL_RenderCopy(window->renderer, anim->sprite, &anim->anims[anim->current_state][anim->current_frame], anim->size);

  anim->current_frame++;

  if (anim->current_frame >= anim->state_frame_count[anim->current_state])
  {
    anim->current_frame = 0;
  }
}

int main(int argc, char *argv[])
{

  window_t *window = createWindow("Test Animations", 640, 480);

  SDL_Surface *dim = IMG_Load("asset/sprite/portal/GreenPortal.png");

  SDL_Rect portal_size = {0, 0, 150, 150};
  int states[] = {8, 8, 6};

  anim_t *green_portal = createAnim(
      8,
      states,
      3,
      SDL_CreateTextureFromSurface(window->renderer, dim),
      dim,
      &portal_size);

  SDL_FreeSurface(dim);

  dim = IMG_Load("asset/sprite/portal/PurplePortal.png");
  anim_t *purple_portal = createAnim(
      8,
      states,
      3,
      SDL_CreateTextureFromSurface(window->renderer, dim),
      dim,
      &portal_size);

  SDL_FreeSurface(dim);

  frame_timer_t *main_timer = createTimer(1000 / 10);

  int running = 1;
  while (running)
  {
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
        running = 0;
    }

    // Début boucle animations

    if (checkTime(main_timer))
    {
      SDL_RenderClear(window->renderer);

      if (SDL_PollEvent(&e))
      {
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {

          if (e.button.clicks == 1 && (e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_RIGHT))
          {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
              green_portal->size->x = e.button.x - (green_portal->size->w / 2);
              green_portal->size->y = e.button.y - (green_portal->size->h / 2);
              updateAnim(green_portal, ANIMATION_IDLE, window);
            }

            else
            {
              purple_portal->size->x = e.button.x - (purple_portal->size->w / 2);
              purple_portal->size->y = e.button.y - (purple_portal->size->h / 2);
              updateAnim(purple_portal, ANIMATION_SPAWN, window);
            }
          }
        }
      }
      updateAnim(green_portal, ANIMATION_CURRENT, window);
      updateAnim(purple_portal, ANIMATION_CURRENT, window);
      SDL_RenderPresent(window->renderer);
    }
  }

  destroyAnim(&green_portal);
  destroyAnim(&purple_portal);
  destroyWindow(&window);
  deleteTimer(&main_timer);
  TTF_Quit();
  SDL_Quit();

  return 0;
}