#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <assert.h>
#include "timer.h"
#include "menu.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 680;

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
  TTF_Font *font = TTF_OpenFont("asset/font/8-BIT WONDER.TTF", 24);
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

TTF_Font *loadFontTextBox(SDL_Window *window, SDL_Renderer *renderer)
{
  // Chargement de la police
  TTF_Font *fontTextBox = TTF_OpenFont("asset/font/arial.TTF", 24);
  if (fontTextBox == NULL)
  {
    fprintf(stderr, "Erreur lors du chargement de la police : %s", TTF_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return NULL;
  }

  return fontTextBox;
}

SDL_Point getMousePosition()
{
  int mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  //printf("Mouse position : (%d, %d)\n", mouseX, mouseY);
  return (SDL_Point){mouseX, mouseY};
}

void SetTextInputRect(SDL_Rect rect) {
    SDL_SetTextInputRect(&rect);
}

int menu_multi(int argc, char **argv){

  //Initialisation 

  int inoption = 0;

  // Initialisation de la SDL

  initialisationSDL();

  SDL_Window *window = creationFenetre();

  SDL_Renderer *renderer = creationRenderer(window);

  renderDrawColor(renderer);

  checkTTFLib(window, renderer);

  TTF_Font *font = loadFont(window, renderer);

  // Commencer la saisie de texte
  SDL_StartTextInput();

  // Création des boutons
  SDL_Color color = {52, 36, 20, 0}; // Rouge
  SDL_Surface *buttonSurfaceHost = NULL;
  SDL_Surface *buttonSurfaceJoin = NULL;
  SDL_Surface *buttonSurfaceQuitter = NULL;
  SDL_Texture *buttonTextureHost = NULL;
  SDL_Texture *buttonTextureJoin = NULL;
  SDL_Texture *buttonTextureQuitter = NULL;
  SDL_Rect buttonRectHost;
  SDL_Rect buttonRectJoin;
  SDL_Rect buttonRectQuitter;

  // Variables pour le texte
    char inputText[1024] = {0};
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* textSurface = NULL;
    SDL_Texture* textTexture = NULL;
    int taille_textbox = 1;

  // Création de l'Image du Menu Principal

  SDL_Surface *imagep = IMG_Load("asset/PixelBooksVers1.0/RADL_Book4.png");
  if (!imagep)
  {
    printf("Erreur de chargement de l'image : %s", SDL_GetError());
    return -1;
  }
  SDL_Texture *texturep = SDL_CreateTextureFromSurface(renderer, imagep);
  SDL_Rect ImageRect;

  ImageRect.h = imagep->h;
  ImageRect.w = imagep->w;
  ImageRect.x = WINDOW_WIDTH / 2;
  ImageRect.x = WINDOW_HEIGHT / 2;

  // Bouton "HOST"
  buttonSurfaceHost = TTF_RenderText_Solid(font, "HOST", color);
  buttonTextureHost = SDL_CreateTextureFromSurface(renderer, buttonSurfaceHost);
  buttonRectHost.w = buttonSurfaceHost->w;
  buttonRectHost.h = buttonSurfaceHost->h;
  buttonRectHost.x = WINDOW_WIDTH / 2 - buttonRectHost.w * 2;       
  buttonRectHost.y = WINDOW_HEIGHT / 2 - buttonRectHost.h / 2 - 50; 

  // Bouton "JOIN"
  buttonSurfaceJoin = TTF_RenderText_Solid(font, "JOIN", color);
  buttonTextureJoin = SDL_CreateTextureFromSurface(renderer, buttonSurfaceJoin);
  buttonRectJoin.w = buttonSurfaceJoin->w;
  buttonRectJoin.h = buttonSurfaceJoin->h;
  buttonRectJoin.x = buttonRectHost.x;                        
  buttonRectJoin.y = WINDOW_HEIGHT / 2 - buttonRectJoin.h / 2; 

  // Bouton "QUITTER"
  buttonSurfaceQuitter = TTF_RenderText_Solid(font, "QUITTER", color);
  buttonTextureQuitter = SDL_CreateTextureFromSurface(renderer, buttonSurfaceQuitter);
  buttonRectQuitter.w = buttonSurfaceQuitter->w;
  buttonRectQuitter.h = buttonSurfaceQuitter->h;
  buttonRectQuitter.x = buttonRectHost.x;                         // Centrer horizontalement
  buttonRectQuitter.y = buttonRectJoin.y + 50 ; // Placer au centre

  SDL_Rect TextInputRect = {buttonRectHost.x*2, buttonRectHost.y, buttonRectJoin.w*2.5, buttonRectJoin.h};
  SDL_SetTextInputRect(&TextInputRect);

  frame_timer_t *multi_timer = createTimer(1000 / 60);

  // Boucle principale
  while (1)
  {
      SDL_Event event;

      // Gestion des évènements
      while (SDL_PollEvent(&event))
      {
        handleEvent(&event);

        
          SDL_Color color = {52, 36, 20, 0};
          buttonSurfaceHost = TTF_RenderText_Solid(font, "HOST", color);
          buttonTextureHost = SDL_CreateTextureFromSurface(renderer, buttonSurfaceHost);
          buttonSurfaceJoin = TTF_RenderText_Solid(font, "JOIN", color);
          buttonTextureJoin = SDL_CreateTextureFromSurface(renderer, buttonSurfaceJoin);
          buttonSurfaceQuitter = TTF_RenderText_Solid(font, "QUITTER", color);
          buttonTextureQuitter = SDL_CreateTextureFromSurface(renderer, buttonSurfaceQuitter);
          
          // Si l'utilisateur clique sur le bouton "QUITTER"
          if (event.type == SDL_MOUSEBUTTONDOWN &&
              event.button.button == SDL_BUTTON_LEFT &&
              event.button.x >= buttonRectQuitter.x &&
              event.button.x <= buttonRectQuitter.x + buttonRectQuitter.w &&
              event.button.y >= buttonRectQuitter.y &&
              event.button.y <= buttonRectQuitter.y + buttonRectQuitter.h 
              && inoption == 0)
          {
            SDL_DestroyTexture(buttonTextureHost);
            SDL_FreeSurface(buttonSurfaceHost);
            SDL_DestroyTexture(buttonTextureJoin);
            SDL_FreeSurface(buttonSurfaceJoin);
            SDL_DestroyTexture(buttonTextureQuitter);
            SDL_FreeSurface(buttonSurfaceQuitter);

            // Quitter le programme
            exit(0);
          }

          switch (event.type)
          {
            case SDL_QUIT: 
              return 0;
            case SDL_TEXTINPUT:
              strcat(inputText, event.text.text);
              printf("Texte saisi : %s", inputText);
              break;
            case SDL_KEYDOWN:
              if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0)
              {
                inputText[strlen(inputText) - 1] = '\0';
              }
              break;
          }

          //calcule taille du text
          if(strlen(inputText) == 0){
            taille_textbox = 1;
          }
          else{
            taille_textbox = strlen(inputText);
          }

          // Rendre le texte à partir de la surface
          SDL_FreeSurface(textSurface);
          textSurface = TTF_RenderText_Solid(font, inputText, textColor);
          textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

          // Effacement de l'écran
          SDL_RenderClear(renderer);

          // Affichage de l'image de fond
          SDL_RenderCopy(renderer, texturep, NULL, NULL);

          // Affichage des boutons
          SDL_RenderCopy(renderer, buttonTextureHost, NULL, &buttonRectHost);
          SDL_RenderCopy(renderer, buttonTextureJoin, NULL, &buttonRectJoin);
          SDL_RenderCopy(renderer, buttonTextureQuitter, NULL, &buttonRectQuitter);

          //---------//

          // Dessiner le rectangle de saisie de texte
          SDL_SetRenderDrawColor(renderer, 192, 148, 115, 0);
          SDL_RenderFillRect(renderer, &TextInputRect);

          // Dessiner le texte
          SDL_Rect textRect = {buttonRectHost.x*2,  buttonRectHost.y, ((buttonRectJoin.w*2)/16+(taille_textbox)*10), (buttonRectJoin.h)};
          //printf("Valeur actuel : %d, valeur diviser : %d", buttonRectJoin.w*2, ((buttonRectJoin.w*2)/16+(taille_textbox*10)));
          SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                                       

          // Mise à jour de l'affichage
          SDL_RenderPresent(renderer);

          // Gestion du temps

          long time_left = timeLeft(multi_timer);

          if (time_left > 0)
          {
            SDL_Delay(time_left);
          }


      }

      


  }

  // Libération de la mémoire
  deleteTimer(&multi_timer);

  SDL_DestroyTexture(buttonTextureHost);
  SDL_FreeSurface(buttonSurfaceHost);
  SDL_DestroyTexture(buttonTextureJoin);
  SDL_FreeSurface(buttonSurfaceJoin);
  SDL_DestroyTexture(buttonTextureQuitter);
  SDL_FreeSurface(buttonSurfaceQuitter);
  SDL_DestroyTexture(texturep);
  SDL_FreeSurface(imagep);

  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;


}

int menu_principal(int argc, char **argv)
{

  //Initialisation 

  int inoption = 0;

  // Initialisation de la SDL

  initialisationSDL();

  SDL_Window *window = creationFenetre();

  SDL_Renderer *renderer = creationRenderer(window);

  renderDrawColor(renderer);

  checkTTFLib(window, renderer);

  TTF_Font *font = loadFont(window, renderer);

  // Création des boutons
  SDL_Color color = {52, 36, 20, 0}; // Rouge
  SDL_Surface *buttonSurface0 = NULL;
  SDL_Surface *buttonSurface1 = NULL;
  SDL_Surface *buttonSurface2 = NULL;
  SDL_Surface *buttonSurface3 = NULL;
  SDL_Surface *buttonSurfaceRetour = NULL;
  SDL_Texture *buttonTexture0 = NULL;
  SDL_Texture *buttonTexture1 = NULL;
  SDL_Texture *buttonTexture2 = NULL;
  SDL_Texture *buttonTexture3 = NULL;
  SDL_Texture *buttonTextureRetour = NULL;
  SDL_Rect buttonRect0;
  SDL_Rect buttonRect1;
  SDL_Rect buttonRect2;
  SDL_Rect buttonRect3;
  SDL_Rect buttonRectRetour;

  // Création de l'Image du Menu Principal

  SDL_Surface *imagep = IMG_Load("asset/PixelBooksVers1.0/RADL_Book4.png");
  if (!imagep)
  {
    printf("Erreur de chargement de l'image : %s", SDL_GetError());
    return -1;
  }
  SDL_Texture *texturep = SDL_CreateTextureFromSurface(renderer, imagep);
  SDL_Rect ImageRect;

  ImageRect.h = imagep->h;
  ImageRect.w = imagep->w;
  ImageRect.x = WINDOW_WIDTH / 2;
  ImageRect.x = WINDOW_HEIGHT / 2;

  // Bouton "JOUER"
  buttonSurface0 = TTF_RenderText_Solid(font, "JOUER", color);
  buttonTexture0 = SDL_CreateTextureFromSurface(renderer, buttonSurface0);
  buttonRect0.w = buttonSurface0->w;
  buttonRect0.h = buttonSurface0->h;
  buttonRect0.x = WINDOW_WIDTH / 2 - buttonRect0.w * 2;       // Centrer horizontalement
  buttonRect0.y = WINDOW_HEIGHT / 2 - buttonRect0.h / 2 - 50; // Placer au-dessus du centre

  // Bouton "OPTION"
  buttonSurface1 = TTF_RenderText_Solid(font, "OPTION", color);
  buttonTexture1 = SDL_CreateTextureFromSurface(renderer, buttonSurface1);
  buttonRect1.w = buttonSurface1->w;
  buttonRect1.h = buttonSurface1->h;
  buttonRect1.x = buttonRect0.x;                         // Centrer horizontalement
  buttonRect1.y = WINDOW_HEIGHT / 2 - buttonRect1.h / 2; // Placer au centre

  // Bouton "QUITTER"
  buttonSurface2 = TTF_RenderText_Solid(font, "QUITTER", color);
  buttonTexture2 = SDL_CreateTextureFromSurface(renderer, buttonSurface2);
  buttonRect2.w = buttonSurface2->w;
  buttonRect2.h = buttonSurface2->h;
  buttonRect2.x = buttonRect0.x;                              // Centrer horizontalement
  buttonRect2.y = WINDOW_HEIGHT / 2 - buttonRect2.h / 2 + 50; // Placer en-dessous du centre

  frame_timer_t *main_timer = createTimer(1000 / 60);

  // Boucle principale
  while (1)
  {
      SDL_Event event;

      // Gestion des évènements
      while (SDL_PollEvent(&event))
      {
        handleEvent(&event);

        // Si l'utilisateur clique sur le bouton "QUITTER"
        if (event.type == SDL_MOUSEBUTTONDOWN &&
            event.button.button == SDL_BUTTON_LEFT &&
            event.button.x >= buttonRect2.x &&
            event.button.x <= buttonRect2.x + buttonRect2.w &&
            event.button.y >= buttonRect2.y &&
            event.button.y <= buttonRect2.y + buttonRect2.h 
            && inoption == 0)
        {
          SDL_DestroyTexture(buttonTexture0);
          SDL_FreeSurface(buttonSurface0);
          SDL_DestroyTexture(buttonTexture1);
          SDL_FreeSurface(buttonSurface1);
          SDL_DestroyTexture(buttonTexture2);
          SDL_FreeSurface(buttonSurface2);

          // Quitter le programme
          exit(0);
        }

        // Si l'utilisateur clique sur le bouton "OPTION"
        if (event.type == SDL_MOUSEBUTTONDOWN &&
            event.button.button == SDL_BUTTON_LEFT &&
            event.button.x >= buttonRect1.x &&
            event.button.x <= buttonRect1.x + buttonRect1.w &&
            event.button.y >= buttonRect1.y &&
            event.button.y <= buttonRect1.y + buttonRect1.h && inoption == 0)
        {
            SDL_DestroyTexture(buttonTexture0);
            SDL_FreeSurface(buttonSurface0);
            SDL_DestroyTexture(buttonTexture1);
            SDL_FreeSurface(buttonSurface1);
            SDL_DestroyTexture(buttonTexture2);
            SDL_FreeSurface(buttonSurface2);
            inoption = 1;

            // Bouton "Retour"
            buttonSurfaceRetour = TTF_RenderText_Solid(font, "Retour", color);
            buttonTextureRetour = SDL_CreateTextureFromSurface(renderer, buttonSurfaceRetour);
            buttonRectRetour.w = buttonSurfaceRetour->w;
            buttonRectRetour.h = buttonSurfaceRetour->h;
            buttonRectRetour.x = WINDOW_WIDTH / 2 - buttonRectRetour.w * 2;       // Centrer horizontalement
            buttonRectRetour.y = WINDOW_HEIGHT / 2 - buttonRectRetour.h / 2 - 50; // Placer au-dessus du centre

            SDL_RenderCopy(renderer, buttonTextureRetour, NULL, &buttonRectRetour);

            // Mise à jour de l'affichage
            SDL_RenderPresent(renderer);
        }

        // Si l'utilisateur met sa souris au dessus
        SDL_Point mousePoint = getMousePosition();
        if (SDL_PointInRect(&mousePoint, &buttonRect0) && inoption == 0)
        {
          SDL_Color color = {0, 255, 0, 0}; // Vert
          buttonSurface0 = TTF_RenderText_Solid(font, "JOUER", color);
          buttonTexture0 = SDL_CreateTextureFromSurface(renderer, buttonSurface0);
        }
        else if (SDL_PointInRect(&mousePoint, &buttonRect1) && inoption == 0)
        {
          SDL_Color color = {0, 0, 255, 0}; // bleu
          buttonSurface1 = TTF_RenderText_Solid(font, "OPTION", color);
          buttonTexture1 = SDL_CreateTextureFromSurface(renderer, buttonSurface1);
        }
        else if (SDL_PointInRect(&mousePoint, &buttonRect2) && inoption == 0)
        {
          SDL_Color color = {255, 0, 0, 0}; // Rouge
          buttonSurface2 = TTF_RenderText_Solid(font, "QUITTER", color);
          buttonTexture2 = SDL_CreateTextureFromSurface(renderer, buttonSurface2);
        }
        else if(inoption == 0)
        {
          SDL_Color color = {52, 36, 20, 0};
          buttonSurface0 = TTF_RenderText_Solid(font, "JOUER", color);
          buttonTexture0 = SDL_CreateTextureFromSurface(renderer, buttonSurface0);
          buttonSurface1 = TTF_RenderText_Solid(font, "OPTION", color);
          buttonTexture1 = SDL_CreateTextureFromSurface(renderer, buttonSurface1);
          buttonSurface2 = TTF_RenderText_Solid(font, "QUITTER", color);
          buttonTexture2 = SDL_CreateTextureFromSurface(renderer, buttonSurface2);
        }
        

        // Effacement de l'écran
        SDL_RenderClear(renderer);

        // Affichage de l'image de fond
        SDL_RenderCopy(renderer, texturep, NULL, NULL);

        // Affichage des boutons
        SDL_RenderCopy(renderer, buttonTexture0, NULL, &buttonRect0);
        SDL_RenderCopy(renderer, buttonTexture1, NULL, &buttonRect1);
        SDL_RenderCopy(renderer, buttonTexture2, NULL, &buttonRect2);

        // Mise à jour de l'affichage
        SDL_RenderPresent(renderer);

        long time_left = timeLeft(main_timer);

        if (time_left > 0)
        {
          SDL_Delay(time_left);
        }


      }

      //SDL_Delay(timeLeft(main_timer));
    //}
  }

  // Libération de la mémoire
  deleteTimer(&main_timer);

  SDL_DestroyTexture(buttonTexture0);
  SDL_FreeSurface(buttonSurface0);
  SDL_DestroyTexture(buttonTexture1);
  SDL_FreeSurface(buttonSurface1);
  SDL_DestroyTexture(buttonTexture2);
  SDL_FreeSurface(buttonSurface2);
  SDL_DestroyTexture(buttonTexture3);
  SDL_FreeSurface(buttonSurface3);
  SDL_DestroyTexture(buttonTextureRetour);
  SDL_FreeSurface(buttonSurfaceRetour);
  SDL_DestroyTexture(texturep);
  SDL_FreeSurface(imagep);

  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}