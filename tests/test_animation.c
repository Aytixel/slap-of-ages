#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "timer/timer.h"
#include "window/animation.h"
#include "window/window.h"

int main(int argc, char *argv[])
{

  window_t *window = createWindow("Test Animations", 640, 480);

  SDL_Surface *dim = IMG_Load("asset/sprite/portal/GreenPortal.png");

  SDL_Rect portal_size = {0, 0, 150, 150};
  int states[] = {8, 8, 6};


  anim_list_t *list = createAnimList();
  
  anim_t *portal = NULL;

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

      if (e.type == SDL_MOUSEBUTTONDOWN)
        {

          if (e.button.clicks == 1 && (e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_RIGHT))
          {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
              green_portal->size->x = e.button.x - (green_portal->size->w / 2);
              green_portal->size->y = e.button.y - (green_portal->size->h / 2);

              portal = createAnim(
                                  8,
                                  states,
                                  3,
                                  SDL_CreateTextureFromSurface(window->renderer, dim),
                                  dim,
                                  &portal_size);

              updateStateAnim(portal, ANIMATION_IDLE);
              addAnimList(list, portal);
              portal = NULL;
              printf("Click gauche\nNombre d'éléments dans la liste: %d", countAnimList(list));
            }

            else
            {
              /*
              purple_portal->size->x = e.button.x - (purple_portal->size->w / 2);
              purple_portal->size->y = e.button.y - (purple_portal->size->h / 2);
              updateStateAnim(purple_portal, ANIMATION_IDLE);*/
              removeAnimList(list, green_portal);
              printf("Click droit\nNombre d'éléments dans la liste: %d", countAnimList(list));
            }
          }
        }
    }


    if (checkTime(main_timer))
    {
      SDL_RenderClear(window->renderer);
      updateAnimList(list , window);
      SDL_RenderPresent(window->renderer);
    }
  }

  destroyAnimList(&list);
  destroyWindow(&window);
  deleteTimer(&main_timer);
  TTF_Quit();
  SDL_Quit();

  return 0;
}