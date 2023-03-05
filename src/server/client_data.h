/**
 * @file client_data.h
 * @author Lucas Dureau
 * @brief Implémentation des prototypes de server/client_data.c
 * @version 0.1
 * @date 02/03/2023
 *
 */

#ifndef __CLIENT_DATA_H
#define __CLIENT_DATA_H

/**
 * @brief Structure de données associée à un client du serveur
 *
 */
typedef struct
{
    char *pseudo;
} client_data_t;

extern client_data_t *createClientData();

extern int deleteClientData(client_data_t **client_data);

#endif
