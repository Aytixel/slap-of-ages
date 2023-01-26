#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// Fonction de gestion des évènements
void handleEvent(SDL_Event *event)
{
  switch (event->type)
  {
  // Si l'utilisateur ferme la fenêtre
  case SDL_QUIT:
    exit(0);
    break;
  }
}

int initialisationSDL()
{
  // Initialisation de la SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Erreur lors de l'initialisation de la SDL : %s", SDL_GetError());
    return 1;
  }

  return 0;
}

SDL_Window *creationFenetre()
{
  // Création de la fenêtre
  SDL_Window *window = SDL_CreateWindow("Premier test SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL)
  {
    fprintf(stderr, "Erreur lors de la création de la fenêtre : %s", SDL_GetError());
    SDL_Quit();
    return NULL;
  }
  return window;
}

SDL_Renderer *creationRenderer(SDL_Window *window)
{
  // Création du renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
  {
    fprintf(stderr, "Erreur lors de la création du renderer : %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return NULL;
  }
  return renderer;
}

void renderDrawColor(SDL_Renderer *renderer)
{
  // Définition de la couleur de fond
  SDL_SetRenderDrawColor(renderer, 100, 10, 0, 255);
}

int checkTTFLib(SDL_Window *window, SDL_Renderer *renderer)
{
  // Check de la librairie
  if (TTF_Init() == -1)
  {
    fprintf(stderr, "Erreur lors de l'initialisation de la SDL_ttf : %s", TTF_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  return 0;
}

TTF_Font *loadFont(SDL_Window *window, SDL_Renderer *renderer)
{
  // Chargement de la police
  TTF_Font *font = TTF_OpenFont("asset/font/arial.ttf", 24);
  if (font == NULL)
  {
    fprintf(stderr, "Erreur lors du chargement de la police : %s", TTF_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return NULL;
  }

  return font;
}

int main(int argc, char *argv[])
{

  initialisationSDL();

  SDL_Window *window = creationFenetre();

  SDL_Renderer *renderer = creationRenderer(window);

  renderDrawColor(renderer);

  checkTTFLib(window, renderer);

  //TTF_Font *font = loadFont(window, renderer);

  while (1)
  {
    SDL_Event event;

    // Gestion des évènements
    while (SDL_PollEvent(&event))
    {
      handleEvent(&event);
      printf("c");
    }

    // Effacement de l'écran
    SDL_RenderClear(renderer);

    // Mise à jour de l'affichage
    SDL_RenderPresent(renderer);
  }

  //TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}