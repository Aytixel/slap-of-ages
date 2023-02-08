/**
 * @file socket.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de socket.c
 * @version 0.1
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

/**
 * @brief Paquet réseaux pour les sockets
 *
 */
typedef struct
{
    uint8_t id;         /**< id du packet (de 0 a 255)*/
    void *data;         /**< buffer de données*/
    size_t data_length; /**< taille du buffer de données*/
} packet_t;

extern int initSocket();

extern void endSocket();

extern int deletePacket(packet_t **packet);

extern int setupAddress(struct sockaddr_in *address, size_t address_length, char *hostname, uint16_t port);

#endif