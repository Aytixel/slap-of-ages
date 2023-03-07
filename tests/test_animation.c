#include <stdio.h>
#include <stdlib.h>
#include "timer/timer.h"
#include "window/animation.h"
#include "window/animation_states.h"
#include "window/window.h"

int main(int argc, char *argv[])
{
  window_t *window = createWindow("Test Animations", 640, 480);

  SDL_Rect portal_size = {100, 100, 150, 150};
  SDL_Rect rat_size = {0, 0, 200, 200};
  int states[] = {8, 8, 6};
  int rat_states[] = {4, 8, 12, 4, 5};

  anim_t *green_portal = createAnim(
      8,
      states,
      3,
      loadSprite(window, "asset/sprite/portal/GreenPortal.png"),
      &portal_size);

  anim_t *rat = createAnim(
      12,
      rat_states,
      5,
      loadSprite(window, "asset/pack/characters/ratfolk_axe.png"),
      &rat_size);

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

      updateAnim(rat, 2, window);
      printf("frame: %d\nnb_frames: %d\nx: %d\ny: %d\nw: %d\nh: %d\n", rat->current_frame, rat->state_frame_count[0], rat->size->x, rat->size->y, rat->anims[rat->current_state][rat->current_frame].w, rat->anims[rat->current_state][rat->current_frame].h);

      SDL_RenderPresent(window->renderer);
    }
  }

  deleteTimer(&main_timer);
  destroyAnim(&green_portal);
  destroyAnim(&rat);
  destroyWindow(&window);

  return 0;
}
