/**
 * @file game_state.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de server/game_state.c
 * @version 0.1
 * @date 02/03/2023
 *
 */

#ifndef __GAME_DATA_H
#define __GAME_DATA_H

typedef struct client_data_s client_data_t;
typedef struct game_data_array_s game_data_array_t;

#include <string.h>
#include "client_data.h"
#include "connection/server.h"
#include "packet/packet.h"

/**
 * @brief Structure de données contenant les informations d'une partie pour un joueur
 *
 */
typedef struct
{
    int socket_fd;                  /**< descripteur du socket du joueur*/
    float destruction_percentage;   /**< pourcentage de destruction*/
    long time_left;                 /**< temps restant avant la fin de partie*/
    int has_finished;               /**< indique si le joueur a finie*/
    client_data_t *client_data;     /**< un pointeur sur les données client*/
    server_client_t *server_client; /**< un pointeur sur la connexion client*/
} player_game_data_t;

/**
 * @brief Structure de données contenant les informations d'une partie
 *
 */
typedef struct game_data_s
{
    player_game_data_t *player[2]; /**< joueurs*/
    game_data_array_t *array;      /**< pointeur sur la structure parente*/
} game_data_t;

/**
 * @brief Structure de données contenant un tableau des données de partie
 *
 */
typedef struct game_data_array_s
{
    game_data_t **game_data; /**< tableau des données de partie*/
    int count;               /**< nombre de partie*/
} game_data_array_t;

/**
 * @brief Créer un tableau avec les données de partie
 *
 * @return un pointer sur le **tableau des données de partie**
 */
extern game_data_array_t *createGameDataArray();

/**
 * @brief Ajoute une nouvelle partie au tableau
 *
 * @param game_data_array une référence d'un pointeur sur un tableau avec les données de partie
 */
extern void addGameDataToArray(game_data_array_t *game_data_array);

/**
 * @brief Trouve une partie qui n'est pas commencé
 *
 * @param game_data_array une référence d'un pointeur sur un tableau avec les données de partie
 * @return l'**index** de la partie trouvé, **-1** sinon
 */
extern int findGame(game_data_array_t *game_data_array);

/**
 * @brief Enlève les données d'une partie du tableau
 *
 * @param game_data_array une référence d'un pointeur sur un tableau avec les données de partie
 * @param index dans le tableau
 * @return **1** si la partie a pu être enlevé, **0** sinon
 */
extern int removeGameDataFromArray(game_data_array_t *game_data_array, int index);

/**
 * @brief Détruit un tableau avec les données de partie
 *
 * @param game_data_array une référence d'un pointeur sur un tableau avec les données de partie
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteGameDataArray(game_data_array_t **game_data_array);

/**
 * @brief Créer les données d'un joueur pour la partie
 *
 * @param socket_fd id du socket client
 * @return un pointer sur les **données d'un joueur pour la partie**
 */
extern player_game_data_t *createPlayerGameData(int socket_fd, client_data_t *client_data, server_client_t *server_client);

/**
 * @brief Détruit les données d'un joueur pour la partie
 *
 * @param player_game_data une référence d'un pointeur sur les données d'un joueur pour la partie
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deletePlayerGameData(player_game_data_t **player_game_data);

/**
 * @brief Créer les données d'un partie
 *
 * @return un pointer sur les **données d'une partie**
 */
extern game_data_t *createGameData();

/**
 * @brief Ajoute un joueur à la partie
 *
 * @param game_data un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @param client_data un pointeur sur les données client
 * @return **1** si le joueur a pu être ajouté, **0** sinon
 */
extern int addPlayerToGame(game_data_t *game_data, int socket_fd, client_data_t *client_data, server_client_t *server_client);

/**
 * @brief Enlève un joueur à la partie
 *
 * @param game_data un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @return **1** si le joueur a pu être enlevé, **0** sinon
 */
extern int removePlayerFromGame(game_data_t *game_data, int socket_fd);

/**
 * @brief Récupère si la partie est vide
 *
 * @param game_data un pointeur sur les données d'une partie
 * @return **1** si la partie est vide, **0** sinont
 */
extern int isGameEmpty(game_data_t *game_data);

/**
 * @brief Récupère si la partie est commencé
 *
 * @param game_data un pointeur sur les données d'une partie
 * @return **1** si la partie est commencé, **0** sinon
 */
extern int isGameStarted(game_data_t *game_data);

/**
 * @brief Récupère si la partie est finie
 *
 * @param game_data un pointeur sur les données d'une partie
 * @return **1** si la partie est finie, **0** sinon
 */
extern int isGameFinished(game_data_t *game_data);

/**
 * @brief Récupère si ce joueur est dans cette partie
 *
 * @param game_data un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @return **0** si ce joueur est le joueur 1, **1** si il est le 2ème, **-1** si il n'en fait pas partie
 */
extern int gameHasPlayer(game_data_t *game_data, int socket_fd);

/**
 * @brief Définit un joueur comme ayant fini
 *
 * @param game_data un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @param packet un pointeur le paquet contenant les informations de fin de partie
 * @return **0** si le joueur est dans cette partie, **-1** sinon
 */
extern int setPlayerFinished(game_data_t *game_data, int socket_fd, packet_t *packet);

/**
 * @brief Détruit les données d'une partie
 *
 * @param game_data une référence d'un pointeur sur les données d'une partie
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteGameData(game_data_t **game_data);

/**
 * @brief Définit un joueur comme étant prêt a jouer ou non
 *
 * @param game_data_array une référence d'un pointeur sur un tableau avec les données de partie
 * @param client_data un pointeur sur les données client
 * @param packet un pointeur le paquet contenant si le joeur est prêt
 */
extern void setPlayerIsReady(game_data_array_t *game_data_array, client_data_t *client_data, packet_t *packet);

#endif
