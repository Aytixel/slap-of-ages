/**
 * @file game_state.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de server/game_state.c
 * @version 1.1
 * @date 02/03/2023
 *
 */

#ifndef __SERVER_GAME_STATE_H
#define __SERVER_GAME_STATE_H

typedef struct server_client_data_s server_client_data_t;
typedef struct server_game_state_array_s server_game_state_array_t;

#include <string.h>
#include "client_data.h"
#include "connection/server.h"
#include "packet/packet.h"
#include "timer/timer.h"

/**
 * @brief Structure de données contenant les informations d'une partie pour un joueur
 *
 */
typedef struct
{
    int socket_fd;                     /**< descripteur du socket du joueur*/
    int destruction_percentage;        /**< pourcentage de destruction*/
    long time_left;                    /**< temps restant avant la fin de partie*/
    int has_finished;                  /**< indique si le joueur a finie*/
    server_client_data_t *client_data; /**< un pointeur sur les données client*/
    server_client_t *server_client;    /**< un pointeur sur la connexion client*/
} player_game_state_t;

/**
 * @brief Structure de données contenant les informations d'une partie
 *
 */
typedef struct server_game_state_s
{
    player_game_state_t *player[2];   /**< joueurs*/
    server_game_state_array_t *array; /**< pointeur sur la structure parente*/
    frame_timer_t *timer;             /**< chronomètre de fin de partie*/
} server_game_state_t;

/**
 * @brief Structure de données contenant un tableau des données de partie
 *
 */
typedef struct server_game_state_array_s
{
    server_game_state_t **game_state; /**< tableau des données de partie*/
    int count;                        /**< nombre de partie*/
    int capacity;                     /**< capacité du tableau*/
} server_game_state_array_t;

/**
 * @brief Créer un tableau avec les données de partie
 *
 * @return un pointer sur le **tableau des données de partie**
 */
extern server_game_state_array_t *createGameStateArray();

/**
 * @brief Ajoute une nouvelle partie au tableau
 *
 * @param game_state_array une référence d'un pointeur sur un tableau avec les données de partie
 */
extern void addGameStateToArray(server_game_state_array_t *game_state_array);

/**
 * @brief Trouve une partie qui n'est pas commencé
 *
 * @param game_state_array une référence d'un pointeur sur un tableau avec les données de partie
 * @return l'**index** de la partie trouvé, **-1** sinon
 */
extern int findGame(server_game_state_array_t *game_state_array);

/**
 * @brief Enlève les données d'une partie du tableau
 *
 * @param game_state_array une référence d'un pointeur sur un tableau avec les données de partie
 * @param index dans le tableau
 * @return **1** si la partie a pu être enlevé, **0** sinon
 */
extern int removeGameStateFromArray(server_game_state_array_t *game_state_array, int index);

/**
 * @brief Détruit un tableau avec les données de partie
 *
 * @param game_state_array une référence d'un pointeur sur un tableau avec les données de partie
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteGameStateArray(server_game_state_array_t **game_state_array);

/**
 * @brief Créer les données d'un joueur pour la partie
 *
 * @param socket_fd id du socket client
 * @return un pointer sur les **données d'un joueur pour la partie**
 */
extern player_game_state_t *createPlayerState(int socket_fd, server_client_data_t *client_data, server_client_t *server_client);

/**
 * @brief Détruit les données d'un joueur pour la partie
 *
 * @param player_game_state une référence d'un pointeur sur les données d'un joueur pour la partie
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deletePlayerState(player_game_state_t **player_game_state);

/**
 * @brief Créer les données d'un partie
 *
 * @return un pointer sur les **données d'une partie**
 */
extern server_game_state_t *createGameState();

/**
 * @brief Ajoute un joueur à la partie
 *
 * @param game_state un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @param client_data un pointeur sur les données client
 * @param server_client un pointeur sur client serveur
 * @return **1** si le joueur a pu être ajouté, **0** sinon
 */
extern int addPlayerToGame(server_game_state_t *game_state, int socket_fd, server_client_data_t *client_data, server_client_t *server_client);

/**
 * @brief Enlève un joueur à la partie
 *
 * @param game_state un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @return **1** si le joueur a pu être enlevé, **0** sinon
 */
extern int removePlayerFromGame(server_game_state_t *game_state, int socket_fd);

/**
 * @brief Récupère si la partie est vide
 *
 * @param game_state un pointeur sur les données d'une partie
 * @return **1** si la partie est vide, **0** sinont
 */
extern int isGameEmpty(server_game_state_t *game_state);

/**
 * @brief Récupère si la partie est commencé
 *
 * @param game_state un pointeur sur les données d'une partie
 * @return **1** si la partie est commencé, **0** sinon
 */
extern int isGameStarted(server_game_state_t *game_state);

/**
 * @brief Récupère si la partie est finie
 *
 * @param game_state un pointeur sur les données d'une partie
 * @return **1** si la partie est finie, **0** sinon
 */
extern int isGameFinished(server_game_state_t *game_state);

/**
 * @brief Récupère si ce joueur est dans cette partie
 *
 * @param game_state un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @return **0** si ce joueur est le joueur 1, **1** si il est le 2ème, **-1** si il n'en fait pas partie
 */
extern int gameHasPlayer(server_game_state_t *game_state, int socket_fd);

/**
 * @brief Définit un joueur comme ayant fini
 *
 * @param game_state un pointeur sur les données d'une partie
 * @param socket_fd id du socket client
 * @param packet un pointeur le paquet contenant les informations de fin de partie
 * @return **0** si le joueur est dans cette partie, **-1** sinon
 */
extern int setPlayerFinished(server_game_state_t *game_state, int socket_fd, packet_t *packet);

/**
 * @brief Récupère quel joueur à gagner
 *
 * @param game_state un pointeur sur les données d'une partie
 * @return **0** si ce joueur est le joueur 1, **1** si il est le 2ème, **2** si ils sont ex-aequo, **-1** si la partie n'est pas fini
 */
extern int gameWinner(server_game_state_t *game_state);

/**
 * @brief Détruit les données d'une partie
 *
 * @param game_state une référence d'un pointeur sur les données d'une partie
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteGameState(server_game_state_t **game_state);

/**
 * @brief Définit un joueur comme ayant fini, dans le liste des parties
 *
 * @param game_state_array une référence d'un pointeur sur un tableau avec les données de partie
 * @param socket_fd id du socket client
 * @param packet un pointeur le paquet contenant les informations de fin de partie
 */
extern void setPlayerFinishedInArray(server_game_state_array_t *game_state_array, int socket_fd, packet_t *packet);

/**
 * @brief Vérifie si une partie à dépassé le temps limite, et la supprime
 *
 * @param game_state_array une référence d'un pointeur sur un tableau avec les données de partie
 */
extern void checkServerGameTimeout(server_game_state_array_t *game_state_array);

/**
 * @brief Définit un joueur comme étant prêt à jouer, ou non, dans le liste des parties
 *
 * @param game_state_array une référence d'un pointeur sur un tableau avec les données de partie
 * @param client_data un pointeur sur les données client
 * @param server_client un pointeur sur client serveur
 * @param packet un pointeur le paquet contenant si le joeur est prêt
 */
extern void setPlayerIsReadyInArray(server_game_state_array_t *game_state_array, server_client_data_t *client_data, server_client_t *server_client, packet_t *packet);

#endif
