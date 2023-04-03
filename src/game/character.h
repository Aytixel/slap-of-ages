/**
 * @file character.h
 * @author Arthur Dureau, Lucas Dureau
 * @brief Fonction de gestion des characters
 * @version 1.0
 * @date 19/03/2023
 *
 */

#ifndef __CHARACTER_H
#define __CHARACTER_H

#include "character_renderer.h"
#include "map/map_renderer.h"
#include "client/game_data.h"
#include "window/animation.h"
#include "window/animation_states.h"

#include <SDL2/SDL.h>

/**
 * @brief Structure contenant les données d'une troupes
 *
 */
typedef struct character_s
{
    character_type_e type;  /**< type de troupe*/
    SDL_Point position;     /**< position de la troupe*/
    animation_t *animation; /**< animation de la troupe*/
    int hp;                 /**< point de vie de la troupe*/
    int attack;             /**< point d'attack de la troupe*/
    int speed;              /**< vitesse de la troupe*/
} character_t;

/**
 * @brief Créer la structure qui gère les données d'une troupes
 *
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 * @param type type de troupe à créer
 * @param position position de la troupe
 * @return character_t*
 */
extern character_t *createCharacter(character_renderer_t *character_renderer, character_type_e type, SDL_Point *position);

/**
 * @brief Créer la matrice de troupe
 *
 * @return retourne un pointeur sur la matrice
 */
extern character_t ***createCharacterMatrix();

/**
 * @brief Affiche la matrice de troupe
 *
 * @param window un pointeur sur une fenêtre
 * @param character_map matrice contenant la totalité des troupes
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 */
extern void renderCharacterMatrix(window_t *window, character_t ***character_map, character_renderer_t *character_renderer);

/**
 * @brief Détruit la structure de troupe
 *
 * @param character un pointeur sur une troupe
 */
extern void destroyCharacter(character_t **character);

/**
 * @brief Permet de détruire toute les troupes sur la carte
 *
 * @param character_matrix matrice contenant la totalité des troupes
 */
extern void clearCharacterMatrix(character_t ***character_matrix);

/**
 * @brief Détruit la matrice de troupe
 *
 * @param character_matrix matrice contenant la totalité des troupes
 */
extern void destroyCharacterMatrix(character_t ****character_matrix);

/**
 * @brief Permet d'ajouter une troupe sur la carte
 *
 * @param character_matrix matrice contenant la totalité des troupes
 * @param character un pointeur sur une troupe
 */
extern void addCharacterInMatrix(character_t ***character_matrix, character_t *character);

/**
 * @brief Permet de supprimer une troupe de la carte
 *
 * @param character_matrix matrice contenant la totalité des troupes
 * @param character un pointeur sur une troupe
 */
extern void removeCharacterFromMatrix(character_t ***character_matrix, character_t *character);

/**
 * @brief Permet de vérifier si une troupe peut être placé à une position donnée
 *
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 * @param position la position à vérifier
 * @param character_matrix matrice contenant la totalité des troupes
 * @return int
 */
extern int canPlaceCharacter(character_renderer_t *character_renderer, SDL_Point *position, character_t ***character_matrix);

/**
 * @brief Récupère la troupe à une position donnée
 *
 * @param character_matrix matrice contenant la totalité des troupes
 * @param x position de la troupe à retourner en x
 * @param y position de la troupe à retourner en y
 * @return character_t* si la troupe existe, NULL sinon
 */
extern character_t *getCharacter(character_t ***character_matrix, int x, int y);

/**
 * @brief Récupère la troupe à une position donnée
 *
 * @param character_matrix matrice contenant la totalité des troupes
 * @param position position de la troupe à retourner
 * @return character_t* si la troupe existe, NULL sinon
 */
extern character_t *getCharacterWithPoint(character_t ***character_matrix, SDL_Point *position);

/**
 * @brief Fonction d'écoute des événements du système de placement de troupe
 *
 * @param event un pointeur sur les données du jeu
 * @param game_data un pointeur sur les données du jeu
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 * @param window un pointeur sur une fenêtre
 */
extern void characterEventHandler(SDL_Event *event, client_game_data_t *game_data, character_renderer_t *character_renderer, window_t *window);

#endif