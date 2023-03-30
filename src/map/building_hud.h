/**
 * @file building_hud.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de map/building_hud.c
 * @version 0.1
 * @date 30/03/2023
 *
 */

#ifndef __BUILDING_HUD_H
#define __BUILDING_HUD_H

#include "client/game_data.h"
#include "window/input.h"

/**
 * @brief Structure de données contenant les données de l'interface de sélection de bâtiment
 *
 */
typedef struct
{
    TTF_Font *text_font;              /**< police d'écriture pour le texte*/
    SDL_Color text_color;             /**< couleur du texte*/
    building_sprite_rects_t rects;    /**< rectangle d'affichage des boutons*/
    button_t *house_1_button;         /**< bouton de sélection de la maison 1*/
    button_t *house_2_button;         /**< bouton de sélection de la maison 2*/
    button_t *house_3_button;         /**< bouton de sélection de la maison 3*/
    button_t *mill_button;            /**< bouton de sélection du moulin*/
    button_t *mine_button;            /**< bouton de sélection de la mine*/
    button_t *field_button;           /**< bouton de sélection du champs*/
    button_t *well_button;            /**< bouton de sélection du puit*/
    button_t *corner_wall_button;     /**< bouton de sélection du coin de mur*/
    button_t *vertical_wall_button;   /**< bouton de sélection du mur verticale*/
    button_t *horizontal_wall_button; /**< bouton de sélection du mur horizontale*/
} building_hud_t;

/**
 * @brief Créer les données de linterface de sélection de bâtiment
 *
 * @param window un pointeur sur une fenêtre
 *
 * @return un pointer sur les **données de linterface de sélection de bâtiment**
 */
extern building_hud_t *createBuildingHud(window_t *window);

/**
 * @brief Fonction d'écoute des événements du menu
 *
 * @param event un pointeur sur les événements
 * @param building_hud un pointeur sur linterface de sélection de bâtiment
 * @param game_data un pointeur sur les données du jeu
 */
extern void buildingHudEventHandler(SDL_Event *event, building_hud_t *building_hud, client_game_data_t *game_data);

/**
 * @brief Fonction d'affichage de linterface de sélection de bâtiment
 *
 * @param window un pointeur sur une fenêtre
 * @param building_hud un pointeur sur linterface de sélection de bâtiment
 * @param building_renderer un pointeur sur la structure qui gère l'affichage des bâtiments
 */
extern void renderBuildingHud(window_t *window, building_hud_t *building_hud, building_renderer_t *building_renderer);

/**
 * @brief Détruit les données de linterface de sélection de bâtiment
 *
 * @param building_hud une référence d'un pointeur sur les données de linterface de sélection de bâtiment
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteBuildingHud(building_hud_t **building_hud);

#endif