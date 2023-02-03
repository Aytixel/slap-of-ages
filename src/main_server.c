#include <stdio.h>
#include "timer.h"
#include "server_socket.h"

int main()
{
  initSocket();

  frame_timer_t *main_timer = createTimer(1000 / 60);

  while (1)
  {
    if (checkTime(main_timer))
    {
      printf("Serveur\n");

      sleepMs(timeLeft(main_timer));
    }
  }

  deleteTimer(&main_timer);
  endSocket();

  return 0;
}