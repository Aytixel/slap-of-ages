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

/**
 * @brief Créer un timer
 *
 * @param interval en milliseconde
 * @return un pointer sur un **timer**
 */
extern frame_timer_t *createTimer(time_t interval);

/**
 * @brief Donne le temps restant avant la fin de l'intervalle
 *
 * @param timer un pointeur sur un timer
 * @return un *long* représentant le temps avant la fin de l'intervalle **en milliseconde** (*0 si on lui passe un pointeur null en entrée*)
 */
extern long timeLeft(frame_timer_t *timer);

/**
 * @brief Vérifie si le code doit être éxécuter et reset le timer
 *
 * @param timer un pointeur sur un timer
 * @return **1 ou 0** en fonction de si on peut éxécuter le code ou non (*-1 si on lui passe un pointeur null en entrée*)
 */
extern int checkTime(frame_timer_t *timer);

/**
 * @brief Détruit un timer
 *
 * @param timer une référence d'un pointeur sur un timer
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteTimer(frame_timer_t **timer);

/**
 * @brief Attend le nombre de milliseconde spécifié
 *
 * @param time temps en milliseconde
 */
extern void sleepMs(time_t time);

#endif