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
#include "window/input.h"
#include "client/game_data.h"
#include "map/map_renderer.h"

/**
 * @brief Structure de données contenant les données de l'interface en jeux
 *
 */
typedef struct
{
    TTF_Font *text_font;           /**< police d'écriture pour le texte*/
    SDL_Color text_color;          /**< couleur du texte*/
    button_t *fight_button;        /**< bouton pour entrer en file d'attente*/
    button_t *cancel_fight_button; /**< bouton pour sortir de la file d'attente*/
} hud_t;

/**
 * @brief Créer les données de l'interface en jeux
 *
 * @param window un pointeur sur une fenêtre
 *
 * @return un pointer sur les **données de l'interface en jeux**
 */
extern hud_t *createHud(window_t *window);

/**
 * @brief Fonction d'écoute des événements du menu
 *
 * @param event un pointeur sur les événements
 * @param hud un pointeur sur l'interface en jeux
 * @param client un pointeur sur le client socket
 * @param game_data un pointeur sur les données du jeux
 */
extern void hudEventHandler(SDL_Event *event, hud_t *hud, client_t *client, client_game_data_t *game_data);

/**
 * @brief Fonction d'affichage de l'interface en jeux
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