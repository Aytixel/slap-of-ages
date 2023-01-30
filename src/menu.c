#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"

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

void bidon(){
  printf("a");
}

int menu(int argc, char **argv)
{

  initialisationSDL();

  SDL_Window *window = creationFenetre();

  SDL_Renderer *renderer = creationRenderer(window);

  renderDrawColor(renderer);

  checkTTFLib(window, renderer);

  TTF_Font *font = loadFont(window, renderer);

  // Création des boutons
  SDL_Color color = {255, 0, 0, 0}; // Rouge
  SDL_Surface *buttonSurface0 = NULL;
  SDL_Surface *buttonSurface1 = NULL;
  SDL_Surface *buttonSurface2 = NULL;
  SDL_Texture *buttonTexture0 = NULL;
  SDL_Texture *buttonTexture1 = NULL;
  SDL_Texture *buttonTexture2 = NULL;
  SDL_Rect buttonRect0;
  SDL_Rect buttonRect1;
  SDL_Rect buttonRect2;
  
  // Bouton "JOUER"
  buttonSurface0 = TTF_RenderText_Solid(font, "JOUER", color);
  buttonTexture0 = SDL_CreateTextureFromSurface(renderer, buttonSurface0);
  buttonRect0.w = buttonSurface0->w;
  buttonRect0.h = buttonSurface0->h;
  buttonRect0.x = WINDOW_WIDTH / 2 - buttonRect0.w / 2; // Centrer horizontalement
  buttonRect0.y = WINDOW_HEIGHT / 2 - buttonRect0.h / 2 - 50; // Placer au-dessus du centre

  // Bouton "OPTION"
  buttonSurface1 = TTF_RenderText_Solid(font, "OPTION", color);
  buttonTexture1 = SDL_CreateTextureFromSurface(renderer, buttonSurface1);
  buttonRect1.w = buttonSurface1->w;
  buttonRect1.h = buttonSurface1->h;
  buttonRect1.x = WINDOW_WIDTH / 2 - buttonRect1.w / 2; // Centrer horizontalement
  buttonRect1.y = WINDOW_HEIGHT / 2 - buttonRect1.h / 2; // Placer au centre

  // Bouton "QUITTER"
  buttonSurface2 = TTF_RenderText_Solid(font, "QUITTER", color);
  buttonTexture2 = SDL_CreateTextureFromSurface(renderer, buttonSurface2);
  buttonRect2.w = buttonSurface2->w;
  buttonRect2.h = buttonSurface2->h;
  buttonRect2.x = WINDOW_WIDTH / 2 - buttonRect2.w / 2; // Centrer horizontalement
  buttonRect2.y = WINDOW_HEIGHT / 2 - buttonRect2.h / 2 + 50; // Placer en-dessous du centre

  

 // Boucle principale
  while (1)
  {
    SDL_Event event;

    // Gestion des évènements
    while (SDL_PollEvent(&event))
    {
      handleEvent(&event);
      printf("c");

      // Si l'utilisateur clique sur le bouton "QUITTER"
      if (event.type == SDL_MOUSEBUTTONDOWN &&
          event.button.button == SDL_BUTTON_LEFT &&
          event.button.x >= buttonRect2.x &&
          event.button.x <= buttonRect2.x + buttonRect2.w &&
          event.button.y >= buttonRect2.y &&
          event.button.y <= buttonRect2.y + buttonRect2.h)
      {
        // Quitter le programme
        exit(0);
      }

      // Si l'utilisateur clique sur le bouton "OPTION"
      if (event.type == SDL_MOUSEBUTTONDOWN &&
          event.button.button == SDL_BUTTON_LEFT &&
          event.button.x >= buttonRect1.x &&
          event.button.x <= buttonRect1.x + buttonRect1.w &&
          event.button.y >= buttonRect1.y &&
          event.button.y <= buttonRect1.y + buttonRect1.h)
      {
        // Libération de la mémoire
        SDL_DestroyTexture(buttonTexture0);
        SDL_FreeSurface(buttonSurface0);
        SDL_DestroyTexture(buttonTexture1);
        SDL_FreeSurface(buttonSurface1);
        SDL_DestroyTexture(buttonTexture2);
        SDL_FreeSurface(buttonSurface2);
        // Bouton "Francais"
        SDL_Color color_fr = {0, 0, 255, 0}; // bleu
        buttonSurface1 = TTF_RenderText_Solid(font, "Francais", color_fr);
        buttonTexture1 = SDL_CreateTextureFromSurface(renderer, buttonSurface1);
        buttonRect1.w = buttonSurface1->w;
        buttonRect1.h = buttonSurface1->h;
        buttonRect1.x = WINDOW_WIDTH / 2 - buttonRect1.w * 2; // Centrer horizontalement
        buttonRect1.y = WINDOW_HEIGHT / 2 - buttonRect1.h / 2; // Placer au centre

        // Bouton "Anglais"
        SDL_Color color_eng = {255, 0, 0, 0}; // Rouge
        buttonSurface2 = TTF_RenderText_Solid(font, "Anglais", color_eng);
        buttonTexture2 = SDL_CreateTextureFromSurface(renderer, buttonSurface2);
        buttonRect2.w = buttonSurface2->w;
        buttonRect2.h = buttonSurface2->h;
        buttonRect2.x = WINDOW_WIDTH / 2 - buttonRect2.w * -2; // Centrer horizontalement
        buttonRect2.y = WINDOW_HEIGHT / 2 - buttonRect2.h / 2; // Placer au centre

        // Affichage des boutons
        SDL_RenderCopy(renderer, buttonTexture0, NULL, &buttonRect0);
        SDL_RenderCopy(renderer, buttonTexture1, NULL, &buttonRect1);
        SDL_RenderCopy(renderer, buttonTexture2, NULL, &buttonRect2);
      }

      // Effacement de l'écran
      SDL_RenderClear(renderer);

      // Affichage des boutons
      SDL_RenderCopy(renderer, buttonTexture0, NULL, &buttonRect0);
      SDL_RenderCopy(renderer, buttonTexture1, NULL, &buttonRect1);
      SDL_RenderCopy(renderer, buttonTexture2, NULL, &buttonRect2);

      // Mise à jour de l'affichage
      SDL_RenderPresent(renderer);
    }
  }

  // Libération de la mémoire
    SDL_DestroyTexture(buttonTexture0);
    SDL_FreeSurface(buttonSurface0);
    SDL_DestroyTexture(buttonTexture1);
    SDL_FreeSurface(buttonSurface1);
    SDL_DestroyTexture(buttonTexture2);
    SDL_FreeSurface(buttonSurface2);

  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}