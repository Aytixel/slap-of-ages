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

typedef struct node_s node_t;

#include "character_renderer.h"
#include "map/map_renderer.h"
#include "client/game_data.h"
#include "window/animation.h"
#include "window/animation_states.h"
#include "game/game.h"

/**
 * @brief Structure contenant les données d'une troupe
 *
 */
typedef struct character_s
{
    character_type_e type;           /**< type de troupe*/
    SDL_FPoint position;             /**< position de la troupe*/
    animation_t *animation;          /**< animation de la troupe*/
    int hp;                          /**< point de vie de la troupe*/
    int attack;                      /**< point d'attack de la troupe*/
    float speed;                     /**< vitesse de la troupe*/
    int is_defender;                 /**< indique si la troupe est défenseure*/
    node_t *path;                    /**< chemin jusqu'à la cible*/
    building_t *targeted_building;   /**< bâtiment ciblé*/
    character_t *targeted_character; /**< troupe ciblé*/
} character_t;

/**
 * @brief Structure de liste pour les troupes
 *
 */
typedef struct character_list_s
{
    character_t **list;
    int count;
    int capacity;
} character_list_t;

/**
 * @brief Créer la structure qui gère les données d'une troupes
 *
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 * @param type type de troupe à créer
 * @param position position de la troupe
 * @param is_defender la troupe est-elle défenseure
 * @return character_t*
 */
extern character_t *createCharacter(character_renderer_t *character_renderer, character_type_e type, SDL_Point *position, int is_defender);

/**
 * @brief Récupère le coût d'une troupe
 *
 * @param type type de troupe
 * @return coût de la troupe en élixir
 */
extern int getCharacterElixirCost(character_type_e type);

/**
 * @brief Créer la liste de troupe
 *
 * @return retourne un pointeur sur la liste
 */
extern character_list_t *createCharacterList();

/**
 * @brief Affiche la liste de troupe
 *
 * @param window un pointeur sur une fenêtre
 * @param character_list liste contenant la totalité des troupes
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 */
extern void renderCharacterList(window_t *window, character_list_t *character_list, character_renderer_t *character_renderer);

/**
 * @brief Détruit la structure de troupe
 *
 * @param character un pointeur sur une troupe
 */
extern void destroyCharacter(character_t **character);

/**
 * @brief Permet de gérer les dégâts subis par une troupe
 *
 * @param game_data un pointeur sur les données du jeu
 * @param character un pointeur sur une troupe
 * @param damages les dégâts subis par la troup
 * @return int
 */
extern int characterTakesDamages(client_game_data_t *game_data, character_t *character, int damages);

/**
 * @brief Permet de détruire toute les troupes sur la carte
 *
 * @param character_list liste contenant la totalité des troupes
 */
extern void clearCharacterList(character_list_t *character_list);

/**
 * @brief Détruit la liste de troupe
 *
 * @param character_list liste contenant la totalité des troupes
 */
extern void destroyCharacterList(character_list_t **character_list);

/**
 * @brief Permet d'ajouter une troupe sur la carte
 *
 * @param character_list liste contenant la totalité des troupes
 * @param character un pointeur sur une troupe
 */
extern void addCharacterInList(character_list_t *character_list, character_t *character);

/**
 * @brief Permet de supprimer une troupe de la carte
 *
 * @param character_list liste contenant la totalité des troupes
 * @param character un pointeur sur une troupe
 */
extern void removeCharacterFromList(character_list_t *character_list, character_t *character);

/**
 * @brief Permet de vérifier si une troupe peut être placé à une position donnée
 *
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 * @param position la position à vérifier
 * @param building_matrix matrice contenant la totalité des bâtiments placés sur la carte
 * @param character_list liste contenant la totalité des troupes
 * @return int
 */
extern int canPlaceCharacter(character_renderer_t *character_renderer, SDL_Point *position, building_t ***building_matrix, character_list_t *character_list);

/**
 * @brief Récupère la troupe la plus proche
 *
 * @param character_list liste contenant la totalité des troupes
 * @param character un pointeur sur une troupe
 * @return character_t*
 */
extern character_t *getNearestCharacter(character_list_t *character_list, character_t *character);

/**
 * @brief Récupère la troupe à une position donnée
 *
 * @param character_list liste contenant la totalité des troupes
 * @param x position de la troupe à retourner en x
 * @param y position de la troupe à retourner en y
 * @return character_t* si la troupe existe, NULL sinon
 */
extern character_t *getCharacter(character_list_t *character_list, int x, int y);

/**
 * @brief Récupère la troupe à une position donnée
 *
 * @param character_list liste contenant la totalité des troupes
 * @param position position de la troupe à retourner
 * @return character_t* si la troupe existe, NULL sinon
 */
extern character_t *getCharacterWithPoint(character_list_t *character_list, SDL_Point *position);

/**
 * @brief Ajoute les troupes de défence à la carte
 *
 * @param character_renderer un pointeur sur la structure qui gère l'affichage des troupes
 * @param game_data un pointeur sur les données du jeu
 */
extern void addDefenceCharacter(character_renderer_t *character_renderer, client_game_data_t *game_data);

/**
 * @brief Met à jours les troupes
 *
 * @param game_data un pointeur sur les données du jeu
 */
extern void updateCharacter(client_game_data_t *game_data);

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