#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "timer.h"
#include "test_animation.h"

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

int initFrames(SDL_Rect *tab, int nb_frames, SDL_Surface *src, int line, int nb_lines)
{

  for (int i = 0; i < nb_frames; i++)
  {

    tab[i].x = (src->w / nb_frames) * i;
    tab[i].y = (src->h / nb_lines) * (line - 1);

    tab[i].w = src->w / nb_frames;
    tab[i].h = src->h / nb_lines;
  }
  return 0;
}

anim_t *createAnim(int max_frames, int *state_frame_count, int state_count, SDL_Texture *sprite, SDL_Surface *dim, SDL_Rect *size)
{

  anim_t *new = malloc(sizeof(anim_t));

  new->state_frame_count = malloc(sizeof(int) * state_count);

  for (int i = 0; i < state_count; i++)
  {

    new->state_frame_count[i] = state_frame_count[i];
  }

  new->state_count = state_count;
  new->current_frame = 0;
  new->sprite = sprite;
  new->size = size;

  new->anims = malloc(sizeof(SDL_Rect *) * state_count);

  for (int i = 0; i < state_count; i++)
  {

    new->anims[i] = malloc(sizeof(SDL_Rect) * max_frames);
    initFrames(new->anims[i], max_frames, dim, i, state_count);
  }

  return new;
}

int destroyAnim(anim_t **anim)
{
  free((*anim)->state_frame_count);

  for (int i = 0; i < (*anim)->state_count; i++)
  {
    free((*anim)->anims[i]);
  }

  free((*anim)->anims);
  free(*anim);
  anim = NULL;

  return 0;
}

void updateAnim(anim_t *anim, portal_e current_state, SDL_Renderer *renderer)
{

  anim->current_frame++;

  if (current_state != ANIMATION_CURRENT)
  {
    anim->current_state = (int)current_state;
  }

  if (current_state == ANIMATION_DELETE)
  {
    destroyAnim(&anim);
    SDL_RenderClear(renderer);
    return;
  }

  if (anim->current_frame > anim->state_frame_count[anim->current_state])
  {
    anim->current_frame = 0;
  }

  SDL_RenderCopy(renderer, anim->sprite, &anim->anims[anim->current_state][anim->current_frame], anim->size);
  
}

int main(int argc, char *argv[])
{

  initialisationSDL();

  SDL_Window *window = creationFenetre();
  SDL_Renderer *renderer = creationRenderer(window);

  SDL_Surface *dim;

  SDL_Rect portal_size = {0, 0, 150, 150};
  int states[] = {8, 8, 6};

  anim_t *green_portal = createAnim(
      8,
      states,
      3,
      SDL_CreateTextureFromSurface(renderer, dim = IMG_Load("asset/GreenPortal.png")),
      dim,
      &portal_size);

  anim_t *purple_portal = createAnim(
      8,
      states,
      3,
      SDL_CreateTextureFromSurface(renderer, dim = IMG_Load("asset/PurplePortal.png")),
      dim,
      &portal_size);

  frame_timer_t *main_timer = createTimer(1000 / 10);

  while (1)
  {
    SDL_Event e;

    if (checkTime(main_timer))
    {
      SDL_RenderClear(renderer);

      if (SDL_PollEvent(&e))
      {
        if (e.type == SDL_QUIT)
        {
          break;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {

          if (e.button.clicks == 1 && (e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_RIGHT))
          {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
              green_portal->size->x = e.button.x - (green_portal->size->w/2);
              green_portal->size->y = e.button.y - (green_portal->size->h/2);
              updateAnim(green_portal, ANIMATION_IDLE, renderer);
            }

            else
            {
              purple_portal->size->x = e.button.x - (purple_portal->size->w/2);
              purple_portal->size->y = e.button.y - (purple_portal->size->h/2);
              updateAnim(purple_portal, ANIMATION_SPAWN, renderer);
            }
          }
        }
      }
      updateAnim(green_portal, ANIMATION_CURRENT, renderer);
      updateAnim(purple_portal, ANIMATION_CURRENT, renderer);
      SDL_RenderPresent(renderer);
      SDL_Delay(timeLeft(main_timer));

      // Effacement de l'écran
      // SDL_RenderClear(renderer);

      // Mise à jour de l'affichage
      // SDL_RenderPresent(renderer);
    }
  }

  // TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}