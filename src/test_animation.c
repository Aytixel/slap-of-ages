#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
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
  SDL_Window *window = SDL_CreateWindow("Tests Animations", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
  SDL_Surface *img = IMG_Load("asset/GreenPortal.png");
  SDL_Texture *img_tex = SDL_CreateTextureFromSurface(renderer, img);

  SDL_Rect portal_idle[8];
  SDL_Rect portal_spawn[8];
  SDL_Rect portal_unspawn[8];
  SDL_Rect portal_size = {10,10,100,100};

  int nbf = 0;
  int state = 1;

  int *win_w = malloc(sizeof(int));
  int *win_h = malloc(sizeof(int));
  

  //renderDrawColor(renderer);
  //checkTTFLib(window, renderer);

  //TTF_Font *font = loadFont(window, renderer);

  for(int i = 0; i < 8; i++){

    portal_idle[i].x = 64 * i;
    portal_idle[i].y = 0;

    portal_idle[i].w = img->w / 8;
    portal_idle[i].h = img->h / 3;

    portal_spawn[i].x = 64 * i;
    portal_spawn[i].y = 64;

    portal_spawn[i].w = img->w / 8;
    portal_spawn[i].h = img->h / 3;

    portal_unspawn[i].x = 64 * i;
    portal_unspawn[i].y = 128;

    portal_unspawn[i].w = img->w / 8;
    portal_unspawn[i].h = img->h / 3;
  }

  while (1)
  {
    SDL_Event e;

    // Gestion des évènements
    /*while (SDL_PollEvent(&event))
    {
      handleEvent(&event);
      //printf("c");
    }*/

    if (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			break;
		}
	}

  if(nbf > 7){
    nbf = 0;
    state++;
    if(state > 3){

      SDL_GetWindowSize(window, win_w, win_h);

      state = 1;
      portal_size.w = portal_size.h = (rand() % 500) + 50;
      portal_size.x = rand() % (*win_w - portal_size.w);
      portal_size.y = rand() % (*win_h - portal_size.w);
    }
  }


  SDL_RenderClear(renderer);


  switch (state)
  {
  case 1:
    SDL_RenderCopy(renderer, img_tex, &portal_spawn[nbf], &portal_size);
    break;
  
  case 2:
    SDL_RenderCopy(renderer, img_tex, &portal_idle[nbf], &portal_size);
    break;
  
  case 3:
    SDL_RenderCopy(renderer, img_tex, &portal_unspawn[nbf], &portal_size);
    break;
  
  default:
    break;
  }

	SDL_RenderPresent(renderer);
  nbf++;

  SDL_Delay(100);

    // Effacement de l'écran
    //SDL_RenderClear(renderer);

    // Mise à jour de l'affichage
    //SDL_RenderPresent(renderer);
  }

  //TTF_CloseFont(font);
  SDL_FreeSurface(img);
  SDL_DestroyTexture(img_tex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  free(win_h);
  free(win_w);
  TTF_Quit();
  SDL_Quit();

  return 0;
}