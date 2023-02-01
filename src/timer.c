/**
 * @file timer.c
 * @author Lucas Dureau
 * @brief Timer permettant l'éxécution de code à intervalle régulier
 * @version 0.1
 * @date 31/01/2023
 *
 */

#include <stdlib.h>
#include <time.h>
#include "timer.h"

/**
 * @brief Créer un timer
 *
 * @param interval en milliseconde
 * @return Un pointer sur un **timer**
 */
extern frame_timer_t *createTimer(time_t interval)
{
    frame_timer_t *timer = malloc(sizeof(frame_timer_t));

    timer->interval = interval;

    gettimeofday(&timer->last_time, NULL);

    timer->frame_per_second = 0;

    return timer;
}

/**
 * @brief Donne le temps restant avant la fin de l'intervalle
 *
 * @param timer un pointeur sur un timer
 * @return un *long* représentant le temps avant la fin de l'intervalle **en milliseconde** (*0 si on lui passe un pointeur null en entrée*)
 */
extern long timeLeft(frame_timer_t *timer)
{
    if (timer == NULL)
        return 0;

    struct timeval now;

    gettimeofday(&now, NULL);

    time_t time_spend = (now.tv_sec - timer->last_time.tv_sec) * 1000 + (now.tv_usec - timer->last_time.tv_usec) / 1000;

    return timer->interval - time_spend;
}

/**
 * @brief Vérifie si le code doit être éxécuter et reset le timer
 *
 * @param timer un pointeur sur un timer
 * @return **1 ou 0** en fonction de si on peut éxécuter le code ou non (*-1 si on lui passe un pointeur null en entrée*)
 */
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

/**
 * @brief Détruit un timer
 *
 * @param timer une référence d'un pointeur sur un timer
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteTimer(frame_timer_t **timer)
{
    if (timer == NULL || *timer == NULL)
        return -1;

    free(*timer);
    *timer = NULL;

    return 0;
}