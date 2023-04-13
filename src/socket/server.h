/**
 * @file server.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de socket/server.c
 * @version 1.1
 * @date 31/01/2023
 *
 */

#ifndef __SERVER_SOCKET_H
#define __SERVER_SOCKET_H

#ifdef WIN32

#include <winsock2.h>

typedef struct SOCKADDR_IN sockaddr_in;

#else

#include <netinet/in.h>

#endif

#include "socket.h"

/**
 * @brief Serveur socket
 *
 */
typedef struct
{
    unsigned int address_length; /**< longueur de l'adresse serveur*/
    struct sockaddr_in address;  /**< adresse du serveur*/
    int socket_fd;               /**< descripteur du socket serveur*/
} server_t;

/**
 * @brief Client serveur socket
 *
 */
typedef struct
{
    unsigned int address_length; /**< longueur de l'adresse client*/
    struct sockaddr_in address;  /**< adresse du client*/
    int socket_fd;               /**< descipteur du socket client*/
    ssize_t recv_length;         /**< longueur des données reçues*/
    packet_t *packet_buffer;     /**< buffer de paquet*/
} server_client_t;

/**
 * @brief Créer un serveur
 *
 * @param hostname nom d'hôte à écouter
 * @param port port à écouter
 * @return un pointer sur un **serveur**
 */
extern server_t *createServer(char *hostname, uint16_t port);

/**
 * @brief Vérifie si il y a des connexions à accepter, et les accepte
 *
 * @param server serveur à utiliser
 * @return un pointeur sur un **client serveur**
 */
extern server_client_t *acceptServerClient(server_t *server);

/**
 * @brief Vérifie si la connexion au client est fermée
 *
 * @param client client serveur à utiliser
 * @return **1 ou 0** en fonction se si la connexion est fermer ou non
 */
extern int isClientDown(server_client_t *client);

/**
 * @brief Envoie un paquet au client
 *
 * @param client client serveur à utiliser
 * @param packet paquet à envoyer
 * @return **0** si tous se passe bien, **-1** si il y a un problème durant l'envoie
 */
extern int sendToServerClient(server_client_t *client, packet_t *packet);

/**
 * @brief Essaye de recevoir un paquet du client
 *
 * @param client client serveur à utiliser
 * @return un pointeur sur un **paquet**, et si la réception n'est pas terminé, ou rien n'a été envoyer renvoie **null**
 */
extern packet_t *recvFromServerClient(server_client_t *client);

/**
 * @brief Détruit un client serveur
 *
 * @param client une référence d'un pointeur sur un client serveur
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteServerClient(server_client_t **client);

/**
 * @brief Détruit un serveur
 *
 * @param server une référence d'un pointeur sur un serveur
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteServer(server_t **server);

#endif