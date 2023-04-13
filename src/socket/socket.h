/**
 * @file socket.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de socket/socket.c
 * @version 1.1
 * @date 31/01/2023
 *
 */

#ifndef __SOCKET_H
#define __SOCKET_H

#ifdef WIN32

#include <stdint.h>
#include <winsock2.h>

typedef struct SOCKADDR_IN sockaddr_in;

#else

#include <netinet/in.h>

#endif

#include "packet/packet.h"

/**
 * @brief Initialisation des sockets
 *
 * Windows à besoin de charger des bibliothèques pour les sockets
 *
 * @return **0** si tous se passe bien
 */
extern int initSocket();

/**
 * @brief Déinitialisation des sockets
 *
 * Windows à besoin de décharger les bibliothèques pour les sockets
 *
 */
extern void endSocket();

/**
 * @brief Détruit un paquet
 *
 * Utilitaire pour détruire un paquet retourné par le serveur ou le client
 *
 * @param packet une référence d'un pointeur sur un paquet
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deletePacket(packet_t **packet);

/**
 * @brief Initialise l'adresse à partir du nom d'hôte et du port
 *
 * @param address
 * @param address_length
 * @param hostname
 * @param port
 * @return **0** si tous se passe bien, **-1** si elle n'arrive pas à convertir le nom d'hôte et le port en adresse
 */
extern int setupAddress(struct sockaddr_in *address, size_t address_length, char *hostname, uint16_t port);

#endif