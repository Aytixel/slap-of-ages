/**
 * @file timer.c
 * @author Lucas Dureau
 * @brief Timer permettant l'éxécution de code à intervalle régulier
 * @version 0.1
 * @date 31/01/2023
 *
 */

#ifdef WIN32

#include <windows.h>

#else

#include <unistd.h>

#endif

#include <stdlib.h>
#include <time.h>
#include "timer.h"

extern frame_timer_t *createTimer(time_t interval)
{
    frame_timer_t *timer = malloc(sizeof(frame_timer_t));

    timer->interval = interval;

    gettimeofday(&timer->last_time, NULL);

    timer->frame_per_second = 0;

    return timer;
}

extern long timeLeft(frame_timer_t *timer)
{
    if (timer == NULL)
        return 0;

    struct timeval now;

    gettimeofday(&now, NULL);

    time_t time_spend = (now.tv_sec - timer->last_time.tv_sec) * 1000 + (now.tv_usec - timer->last_time.tv_usec) / 1000;

    return timer->interval - time_spend;
}

extern int checkTime(frame_timer_t *timer)
{
    if (timer == NULL)
        return -1;

    time_t time_spend = timeLeft(timer);

    if (time_spend < 0)
    {
        timer->frame_per_second = 1000.0 / (-time_spend + timer->interval);

        gettimeofday(&timer->last_time, NULL);

        return 1;
    }

    return 0;
}

extern int deleteTimer(frame_timer_t **timer)
{
    if (timer == NULL || *timer == NULL)
        return -1;

    free(*timer);
    *timer = NULL;

    return 0;
}

extern void sleepMs(time_t time)
{
    if (time > 0)
    {
#ifdef WIN32
        Sleep(time);
#else
        usleep(time * 1000);
#endif
    }
}