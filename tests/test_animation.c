#include <stdio.h>
#include <stdlib.h>
#include "timer/timer.h"
#include "window/animation.h"
#include "window/animation_states.h"
#include "window/window.h"

#define TILE_SIZE 16

int main(int argc, char *argv[])
{
  window_t *window = createWindow("Test Animations", 640, 480);

  SDL_Rect portal_size = {10, 10, 0, 0};
  SDL_Rect rat_size = {100, 300, 0, 0};
  SDL_Rect goblin_size = {300, 200, 0, 0};
  int states[] = {8, 8, 6};
  int rat_states[] = {4, 8, 12, 4, 5};
  int goblin_states[] = {2, 8, 7, 4, 6};

  anim_t *green_portal = createAnim(
      TILE_SIZE,
      states,
      3,
      loadSprite(window, "asset/sprite/portal/GreenPortal.png"),
      &portal_size);

  anim_t *rat = createAnim(
      TILE_SIZE,
      rat_states,
      5,
      loadSprite(window, "asset/pack/characters/ratfolk_axe.png"),
      &rat_size);

  anim_t *goblin = createAnim(
      TILE_SIZE,
      goblin_states,
      5,
      loadSprite(window, "asset/pack/characters/giant_goblin.png"),
      &goblin_size);

  frame_timer_t *main_timer = createTimer(1000 / 10);

  int running = 1;
  while (running)
  {
    SDL_Event event;
    int time_left = timeLeft(main_timer);

    if (SDL_WaitEventTimeout(&event, time_left > 0 ? time_left : 0))
    {
      switch (event.type)
      {
      case SDL_QUIT:

        running = 0;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.clicks == 1 && (event.button.button == SDL_BUTTON_LEFT))
        {
          rat->size->x = event.button.x - (rat->size->w / 2);
          rat->size->y = event.button.y - (rat->size->h / 2);
        }
        else{
          destroyAnim(&green_portal);
          destroyAnim(&rat);
          destroyAnim(&goblin);
        }
        break;
      case SDL_WINDOWEVENT:
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
          window->width = event.window.data1;
          window->height = event.window.data2;
          break;
        }
        break;
      }
    }

    if (checkTime(main_timer))
    {
      SDL_RenderClear(window->renderer);

      updateAnim(goblin, 1, 100, window);
      updateAnim(rat, 2, 100, window);
      updateAnim(green_portal, PORTAL_IDLE, 50, window);

      SDL_RenderPresent(window->renderer);
    }
  }

  deleteTimer(&main_timer);
  destroyAnim(&green_portal);
  destroyAnim(&rat);
  destroyAnim(&goblin);
  destroyWindow(&window);

  return 0;
}
