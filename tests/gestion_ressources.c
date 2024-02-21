//@import sdl

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

// Structure pour stocker les informations sur les ressources
typedef struct
{
    int gold;
} Resources;

// Fonction pour mettre à jour les ressources
void update_resources(Resources *res, int gold)
{
    res->gold += gold;
}

// Fonction pour afficher les ressources
void render_resources(SDL_Renderer *renderer, Resources *res)
{
    char buffer[64];

    sprintf(buffer, "Gold: %d", res->gold);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("SLAP OF AGES",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur de création du rendu: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialisez la SDL_ttf et chargez les polices

    Resources res = {1000, 1000, 100};
    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        render_resources(renderer, &res);

        SDL_RenderPresent(renderer);

        // Mettre à jour les ressources
        // avec des valeurs en fonction des bâtiments.

        SDL_Delay(1000); // Délai pour ne pas surcharger le processeur
    }

    // Libérez les polices et quittez SDL_ttf

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
