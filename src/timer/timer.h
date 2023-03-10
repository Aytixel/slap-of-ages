/**
 * @file timer.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de timer/timer.c
 * @version 0.1
 * @date 31/01/2023
 *
 */

#ifndef __TIMER_H
#define __TIMER_H

#include <sys/time.h>

/**
 * @brief Timer permettant l'éxécution de code à intervalle régulier
 *
 */
typedef struct
{
    time_t interval;          /**< intervalle de temps en milliseconde*/
    struct timeval last_time; /**< temps à la dernière éxécution*/
    double frame_per_second;  /**< nombre d'image par seconde*/
} frame_timer_t;

extern frame_timer_t *createTimer(time_t interval);

extern long timeLeft(frame_timer_t *timer);

extern int checkTime(frame_timer_t *timer);

extern int deleteTimer(frame_timer_t **timer);

extern void sleepMs(time_t time);

#endif