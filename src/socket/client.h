/**
 * @file client.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de socket/client.c
 * @version 1.1
 * @date 31/01/2023
 *
 */

#ifndef __CLIENT_SOCKET_H
#define __CLIENT_SOCKET_H

#ifdef WIN32

#include <winsock2.h>

#define close closesocket
#define SHUT_RDWR SD_BOTH
#define poll WSAPoll

#else

#include <netinet/in.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>

#endif

#include <stdlib.h>
#include <string.h>
#include "socket.h"

/**
 * @brief Client socket
 *
 */
typedef struct
{
    unsigned int address_length; /**< longueur de l'adresse client*/
    struct sockaddr_in address;  /**< adresse du client*/
    int socket_fd;               /**< descipteur du socket client*/
    ssize_t recv_length;         /**< longueur des données reçues*/
    packet_t *packet_buffer;     /**< buffer de paquet*/
} client_t;

/**
 * @brief Créer un client
 *
 * @param hostname nom d'hôte du serveur
 * @param port port du serveur
 * @return un pointer sur un **client**
 */
extern client_t *createClient(char *hostname, uint16_t port);

/**
 * @brief Vérifie si la connexion au serveur est fermée
 *
 * @param client client à utiliser
 * @return **1 ou 0** en fonction se si la connexion est fermer ou non
 */
extern int isServerDown(client_t *client);

/**
 * @brief Envoie un paquet au server
 *
 * @param client client à utiliser
 * @param packet paquet à envoyer
 * @return **0** si tous se passe bien, **-1** si il y a un problème durant l'envoie
 */
extern int sendToServer(client_t *client, packet_t *packet);

/**
 * @brief Essaye de recevoir un paquet du serveur
 *
 * @param client client à utiliser
 * @return un pointeur sur un **paquet**, et si la réception n'est pas terminé, ou rien n'a été envoyer renvoie **null**
 */
extern packet_t *recvFromServer(client_t *client);

/**
 * @brief Détruit un client
 *
 * @param client une référence d'un pointeur sur un client
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteClient(client_t **client);

#endif