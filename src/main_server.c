#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "timer.h"
#include "server_socket.h"

int running = 1;

void handler(int s)
{
    running = 0;
}

int get_connection_info(int argc, char *argv[], char **hostname, uint16_t *port)
{
    for (int i = 1; i < argc; i++)
    {
        if (*hostname == NULL && (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--hostname") == 0) && ++i < argc)
        {
            *hostname = malloc(sizeof(char) * (strlen(argv[i]) + 1));
            strcpy(*hostname, argv[i]);
        }
        else if (*port == 0 && (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) && ++i < argc)
        {
            *port = atoi(argv[i]);
        }
    }

    if (*hostname == NULL)
    {
        return -1;
    }
    if (*port == 0)
    {
        free(*hostname);

        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    char *hostname = NULL;
    uint16_t port = 0;

    if (get_connection_info(argc, argv, &hostname, &port) == -1)
    {
        printf("Erreur : Nom d'hôte ou port non spécifié.\n");

        return 1;
    }

    printf("Ecoute sur %s:%d\n", hostname, port);

    signal(SIGINT, handler);
    signal(SIGABRT, handler);
    signal(SIGTERM, handler);

    initSocket();

    server_t *server = createServer(hostname, port);

    if (server == NULL)
    {
        endSocket();
        free(hostname);

        return 2;
    }

    frame_timer_t *main_timer = createTimer(1000 / 60);

    // boucle principale
    while (running)
    {
        if (checkTime(main_timer))
        {
            sleepMs(timeLeft(main_timer));
        }
    }

    deleteTimer(&main_timer);
    deleteServer(&server);
    endSocket();
    free(hostname);

    return 0;
}