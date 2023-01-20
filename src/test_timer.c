#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include "timer.h"

int main()
{
    printf("Testing timer.h\n\n");

    frame_timer_t *timer = createTimer(1000 / 30);

    assert(timer != NULL);
    printf("Timer creation OK\n");

    assert(timeLeft(timer) > 0);
    printf("\n\tTime left before OK\n");
    assert(!checkTime(timer));
    printf("\tCheck time before OK\n");

    printf("\nWait for 1 second\n");
    sleep(1);

    assert(timeLeft(timer) < 0);
    printf("\n\tTime left after OK\n");
    assert(checkTime(timer));
    printf("\tCheck time after OK\n");

    assert(timeLeft(timer) > 0);
    printf("\n\tTime left before OK\n");
    assert(!checkTime(timer));
    printf("\tCheck time before OK\n");

    assert(timer->frame_per_second > 0.99 && timer->frame_per_second < 1.01);
    printf("\n\tFrame per second OK\n");

    deleteTimer(&timer);

    assert(timer == NULL);
    printf("\nTimer deletion OK\n");

    printf("\nTesting timer.h successful\n");

    return 0;
}