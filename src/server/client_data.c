/**
 * @file client_data.h
 * @author Lucas Dureau
 * @brief Implémente les fonctions associées la structure de données pour les clients connectés au serveur
 * @version 0.1
 * @date 02/03/2023
 *
 */

#include <stdlib.h>
#include "client_data.h"

/**
 * @brief Créer les données d'un client
 *
 * @return un pointer sur les **données client**
 */
extern client_data_t *createClientData()
{
    client_data_t *client_data = malloc(sizeof(client_data_t));

    client_data->pseudo = NULL;
    client_data->is_player_ready = 0;
    client_data->is_in_game = 0;

    return client_data;
}

/**
 * @brief Détruit les données d'un client
 *
 * @param server une référence d'un pointeur sur les données client
 * @return **0** si tous se passe bien, **-1** si le pointeur en entrée est null
 */
extern int deleteClientData(client_data_t **client_data)
{
    if (client_data == NULL || *client_data == NULL)
        return -1;

    if ((*client_data)->pseudo != NULL)
        free((*client_data)->pseudo);

    free(*client_data);
    *client_data = NULL;

    return 0;
}