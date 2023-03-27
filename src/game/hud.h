/**
 * @file hud.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de game/hud.c
 * @version 0.1
 * @date 27/03/2023
 *
 */
#ifndef __GAME_HUD_H
#define __GAME_HUD_H

#include "window/window.h"
#include "client/game_data.h"
#include "map/map_renderer.h"

/**
 * @brief Structure de données contenant les données de l'interface en jeux
 *
 */
typedef struct
{
    TTF_Font *text_font; /**< police d'écriture pour le texte*/
} hud_t;

/**
 * @brief Créer les données de l'interface en jeux
 *
 * @return un pointer sur les **données de l'interface en jeux**
 */
extern hud_t *createHud();

/**
 * @brief
 *
 * @param window un pointeur sur une fenêtre
 * @param hud un pointeur sur l'interface en jeux
 * @param map_renderer un pointeur sur la carte
 * @param game_data un pointeur sur les données du jeux
 */
extern void renderHud(window_t *window, hud_t *hud, map_renderer_t *map_renderer, client_game_data_t *game_data);

/**
 * @brief Détruit les données de l'interface en jeux
 *
 * @param hud une référence d'un pointeur sur les données de l'interface en jeux
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteHud(hud_t **hud);

#endif