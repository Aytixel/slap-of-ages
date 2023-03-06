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
  SDL_Rect book_size = {100, 100, 300, 300};
  int states[] = {8, 8, 6};
  int book_states[] = {4, 4, 1};

  anim_t *green_portal = createAnim(
      8,
      states,
      3,
      loadSprite(window, "asset/sprite/portal/GreenPortal.png"),
      &portal_size);

  anim_t *book = createAnim(
      4,
      book_states,
      3,
      loadSprite(window, "asset/sprite/menu/book.png"),
      &book_size);

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
          book->size->x = event.button.x - (book->size->w / 2);
          book->size->y = event.button.y - (book->size->h / 2);
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

      updateAnim(book, 0, window);

      SDL_RenderPresent(window->renderer);
    }
  }

  deleteTimer(&main_timer);
  destroyAnim(&green_portal);
  destroyAnim(&book);
  destroyWindow(&window);

  return 0;
}
