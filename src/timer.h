#ifndef __TIMER_H
#define __TIMER_H

#include <sys/time.h>

typedef struct
{
    time_t interval;
    struct timeval last_time;
    double frame_per_second;
} frame_timer_t;

extern frame_timer_t *createTimer(time_t);

extern long timeLeft(frame_timer_t *);

extern int checkTime(frame_timer_t *);

extern int deleteTimer(frame_timer_t **);

#endif