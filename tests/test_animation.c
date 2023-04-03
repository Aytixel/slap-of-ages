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

  SDL_Point portal_position = {10, 10};
  SDL_Point rat_position = {100, 300};
  SDL_Point goblin_position = {300, 200};

  int portal_states[] = {8, 8, 6, -1};
  int rat_states[] = {4, 8, 12, 4, 5, -1};
  int goblin_states[] = {2, 8, 7, 4, 6, -1};

  animation_t *green_portal = createAnimation(
      TILE_SIZE,
      portal_states,
      loadSprite(window, "asset/sprite/portal/GreenPortal.png"),
      10);

  animation_t *rat = createAnimation(
      TILE_SIZE,
      rat_states,
      loadSprite(window, "asset/sprite/characters/ratfolk_axe.png"),
      13);

  animation_t *goblin = createAnimation(
      TILE_SIZE,
      goblin_states,
      loadSprite(window, "asset/sprite/characters/giant_goblin.png"),
      5);

  frame_timer_t *main_timer = createTimer(1000 / 60);

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

          rat_position.x = event.button.x - window->width / 2;
          rat_position.y = event.button.y - window->height / 2;
        }
        else
        {
          destroyAnimation(&green_portal);
          destroyAnimation(&rat);
          destroyAnimation(&goblin);
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

      updateAnimation(goblin, GOBLIN_GIANT_ATTACK_ANIM, 100, &goblin_position, window, TRANSFORM_ORIGIN_CENTER);
      updateAnimation(rat, RAT_IDLE_ANIM, 100, &rat_position, window, TRANSFORM_ORIGIN_CENTER);
      updateAnimation(green_portal, PORTAL_DESPAWN_ANIM, 50, &portal_position, window, TRANSFORM_ORIGIN_CENTER);

      SDL_RenderPresent(window->renderer);
    }
  }

  deleteTimer(&main_timer);
  destroyAnimation(&green_portal);
  destroyAnimation(&rat);
  destroyAnimation(&goblin);
  destroyWindow(&window);

  return 0;
}
