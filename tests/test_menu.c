#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <assert.h>
#include "timer/timer.h"
#include "test_menu.h"

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
  TTF_Font *font = TTF_OpenFont("asset/font/8-BIT-WONDER.ttf", 24);
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
  TTF_Font *fontTextBox = TTF_OpenFont("asset/font/arial.ttf", 24);
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
  // printf("Mouse position : (%d, %d)\n", mouseX, mouseY);
  return (SDL_Point){mouseX, mouseY};
}

void SetTextInputRect(SDL_Rect rect)
{
  SDL_SetTextInputRect(&rect);
}

void createButton(SDL_Renderer *renderer, TTF_Font *font, const char* buttonText, SDL_Color color, float buttonXRatio, float buttonYRatio, float buttonWidthRatio, float buttonHeightRatio, Button *button) {
    // Création de la surface du bouton
    button->surface = TTF_RenderText_Solid(font, buttonText, color);

    // Création de la texture du bouton à partir de la surface
    button->texture = SDL_CreateTextureFromSurface(renderer, button->surface);

    // Calcul des dimensions du bouton en pixels
    button->rect.w = (int)(WINDOW_WIDTH * buttonWidthRatio);
    button->rect.h = (int)(WINDOW_HEIGHT * buttonHeightRatio);

    // Calcul de la position du bouton en pixels
    button->rect.x = (int)(WINDOW_WIDTH * buttonXRatio);
    button->rect.y = (int)(WINDOW_HEIGHT * buttonYRatio);

    // Initialisation du texte du bouton
    button->text = strdup(buttonText);  // duplique la chaîne de caractères pour éviter toute modification ultérieure
}



void createTextbox(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, int x, int y, int w, int h, SDL_Rect* rect) {
    SDL_Surface* surface = NULL;
    SDL_Texture* texture = NULL;
    surface = TTF_RenderText_Solid(font, " ", color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    SDL_SetTextInputRect(rect);
}

int main(int argc, char **argv){

  menu();
  return 1;

}

int menu()
{

  // Initialisation

  int inoption = 0;
  int box = 0;

  // Initialisation de la SDL

  initialisationSDL();

  SDL_Window *window = creationFenetre();

  SDL_Renderer *renderer = creationRenderer(window);

  renderDrawColor(renderer);

  checkTTFLib(window, renderer);

  TTF_Font *font = loadFont(window, renderer);
  TTF_Font *fontTextBox = loadFontTextBox(window, renderer);

  // Commencer la saisie de texte
  SDL_StartTextInput();

  // Création des boutons
  SDL_Color color = {52, 36, 20, 0}; // Rouge
  Button buttonHost;
  Button buttonJoin;
  Button buttonQuitter;

  // Variables pour le texte adresse ip
  char inputTextIp[1024] = {0};
  SDL_Surface *textSurfaceIp = NULL;
  SDL_Texture *textTextureIp = NULL;

  // Variables pour le texte port
  char inputTextPort[1024] = {0};
  SDL_Surface *textSurfacePort = NULL;
  SDL_Texture *textTexturePort = NULL;

  // Variables pour le texte pseudo
  char inputTextPseudo[1024] = {0};
  SDL_Surface *textSurfacePseudo = NULL;
  SDL_Texture *textTexturePseudo = NULL;

  SDL_Color textColor = {255, 255, 255};

  // Création de l'Image du Menu Principal

  SDL_Surface *imagep = IMG_Load("asset/sprite/menu/RADL_Book4.png");
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

  // Bouton "NEWHOST"
  createButton(renderer, font, "HOST", color, 0.21f, 0.40f, 0.1f, 0.04f, &buttonHost);

  // Bouton "NEWJOIN"
  createButton(renderer, font, "JOIN", color, 0.21f, 0.475f, 0.1f, 0.04f, &buttonJoin);

  // Bouton "NEWQUITTER"
  createButton(renderer, font, "QUITTER", color, 0.21f, 0.55f, 0.15f, 0.04f, &buttonQuitter);

  SDL_Rect TextInputRectIp;
  createTextbox(renderer, font, color, buttonHost.rect.x * 2.8, buttonHost.rect.y, buttonJoin.rect.w * 2.5, buttonJoin.rect.h, &TextInputRectIp);
  SDL_Rect TextInputRectPort;
  createTextbox(renderer, font, color, buttonHost.rect.x * 2.8, buttonHost.rect.y * 1.2, buttonJoin.rect.w * 2.5, buttonJoin.rect.h, &TextInputRectPort);
  SDL_Rect TextInputRectPseudo;
  createTextbox(renderer, font, color, buttonHost.rect.x * 2.8, buttonHost.rect.y * 1.4, buttonJoin.rect.w * 2.5, buttonJoin.rect.h, &TextInputRectPseudo);

  frame_timer_t *multi_timer = createTimer(1000 / 60);

  int widthIp, heightIp;
  int widthPort, heightPort;
  int widthPseudo, heightPseudo;

  // Boucle principale
  while (1)
  {
    SDL_Event event;

    // Gestion des évènements
    while (SDL_PollEvent(&event))
    {
      handleEvent(&event);

      SDL_Color color = {52, 36, 20, 0};
      buttonHost.surface = TTF_RenderText_Solid(font, "HOST", color);
      buttonHost.texture = SDL_CreateTextureFromSurface(renderer, buttonHost.surface);
      buttonJoin.surface = TTF_RenderText_Solid(font, "JOIN", color);
      buttonJoin.texture = SDL_CreateTextureFromSurface(renderer, buttonJoin.surface);
      buttonQuitter.surface = TTF_RenderText_Solid(font, "QUITTER", color);
      buttonQuitter.texture = SDL_CreateTextureFromSurface(renderer, buttonQuitter.surface);

      // Si l'utilisateur clique sur le bouton "QUITTER"
      if (event.type == SDL_MOUSEBUTTONDOWN &&
          event.button.button == SDL_BUTTON_LEFT &&
          event.button.x >= buttonQuitter.rect.x &&
          event.button.x <= buttonQuitter.rect.x + buttonQuitter.rect.w &&
          event.button.y >= buttonQuitter.rect.y &&
          event.button.y <= buttonQuitter.rect.y + buttonQuitter.rect.h && inoption == 0)
      {
        SDL_DestroyTexture(buttonHost.texture);
        SDL_FreeSurface(buttonHost.surface);
        SDL_DestroyTexture(buttonJoin.texture);
        SDL_FreeSurface(buttonJoin.surface);
        SDL_DestroyTexture(buttonQuitter.texture);
        SDL_FreeSurface(buttonQuitter.surface);

        // Quitter le programme
        exit(0);
      }

      // Si l'utilisateur clique dans l'un des rectangles de texte
      if (event.type == SDL_MOUSEBUTTONDOWN &&
          event.button.button == SDL_BUTTON_LEFT &&
          event.button.x >= TextInputRectIp.x &&
          event.button.x <= TextInputRectIp.x + TextInputRectIp.w &&
          event.button.y >= TextInputRectIp.y &&
          event.button.y <= TextInputRectIp.y + TextInputRectIp.h && inoption == 0)
      {
        box = 1;
      }
      else if (event.type == SDL_MOUSEBUTTONDOWN &&
               event.button.button == SDL_BUTTON_LEFT &&
               event.button.x >= TextInputRectPort.x &&
               event.button.x <= TextInputRectPort.x + TextInputRectPort.w &&
               event.button.y >= TextInputRectPort.y &&
               event.button.y <= TextInputRectPort.y + TextInputRectPort.h && inoption == 0)
      {
        box = 2;
      }
      else if (event.type == SDL_MOUSEBUTTONDOWN &&
               event.button.button == SDL_BUTTON_LEFT &&
               event.button.x >= TextInputRectPseudo.x &&
               event.button.x <= TextInputRectPseudo.x + TextInputRectPseudo.w &&
               event.button.y >= TextInputRectPseudo.y &&
               event.button.y <= TextInputRectPseudo.y + TextInputRectPseudo.h && inoption == 0)
      {
        box = 3;
      }
      else
      {
      }

      if (box == 1)
      {

        switch (event.type)
        {
        case SDL_QUIT:
          return 0;
        case SDL_TEXTINPUT:
          strcat(inputTextIp, event.text.text);
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputTextIp) > 0)
          {
            inputTextIp[strlen(inputTextIp) - 1] = '\0';
          }
          break;
        }
        TTF_SizeUTF8(font, inputTextIp, &widthIp, &heightIp);
      }
      else if (box == 2)
      {

        switch (event.type)
        {
        case SDL_QUIT:
          return 0;
        case SDL_TEXTINPUT:
          strcat(inputTextPort, event.text.text);
          // printf("Texte saisi : %s", inputTextPort);
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputTextPort) > 0)
          {
            inputTextPort[strlen(inputTextPort) - 1] = '\0';
          }
          break;
        }
        TTF_SizeUTF8(font, inputTextPort, &widthPort, &heightPort);
      }
      else if (box == 3)
      {

        switch (event.type)
        {
        case SDL_QUIT:
          return 0;
        case SDL_TEXTINPUT:
          strcat(inputTextPseudo, event.text.text);
          // printf("Texte saisi : %s", inputTextPseudo);
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputTextPseudo) > 0)
          {
            inputTextPseudo[strlen(inputTextPseudo) - 1] = '\0';
          }
          break;
        }
        TTF_SizeUTF8(font, inputTextPseudo, &widthPseudo, &heightPseudo);
      }
      else
      {
      }

      // Rendre le texte à partir de la surface
      SDL_FreeSurface(textSurfaceIp);
      textSurfaceIp = TTF_RenderText_Solid(fontTextBox, inputTextIp, textColor);
      textTextureIp = SDL_CreateTextureFromSurface(renderer, textSurfaceIp);
      SDL_FreeSurface(textSurfacePort);
      textSurfacePort = TTF_RenderText_Solid(fontTextBox, inputTextPort, textColor);
      textTexturePort = SDL_CreateTextureFromSurface(renderer, textSurfacePort);
      SDL_FreeSurface(textSurfacePseudo);
      textSurfacePseudo = TTF_RenderText_Solid(fontTextBox, inputTextPseudo, textColor);
      textTexturePseudo = SDL_CreateTextureFromSurface(renderer, textSurfacePseudo);

      //---------//

      // Si l'utilisateur met sa souris au dessus
      SDL_Point mousePoint = getMousePosition();
      if (SDL_PointInRect(&mousePoint, &buttonHost.rect))
      {
        SDL_Color color = {52, 36, 155, 0};
        buttonHost.surface = TTF_RenderText_Solid(font, "HOST", color);
        buttonHost.texture = SDL_CreateTextureFromSurface(renderer, buttonHost.surface);
      }
      else if (SDL_PointInRect(&mousePoint, &buttonJoin.rect))
      {
        SDL_Color color = {52, 36, 155, 0};
        buttonJoin.surface = TTF_RenderText_Solid(font, "JOIN", color);
        buttonJoin.texture = SDL_CreateTextureFromSurface(renderer, buttonJoin.surface);
      }
      else if (SDL_PointInRect(&mousePoint, &buttonQuitter.rect))
      {
        SDL_Color color = {52, 36, 155, 0};
        buttonQuitter.surface = TTF_RenderText_Solid(font, "QUITTER", color);
        buttonQuitter.texture = SDL_CreateTextureFromSurface(renderer, buttonQuitter.surface);
      }
      else
      {
        SDL_Color color = {52, 36, 20, 0};
        buttonHost.surface = TTF_RenderText_Solid(font, "HOST", color);
        buttonHost.texture = SDL_CreateTextureFromSurface(renderer, buttonHost.surface);
        buttonJoin.surface = TTF_RenderText_Solid(font, "JOIN", color);
        buttonJoin.texture = SDL_CreateTextureFromSurface(renderer, buttonJoin.surface);
        buttonQuitter.surface = TTF_RenderText_Solid(font, "QUITTER", color);
        buttonQuitter.texture = SDL_CreateTextureFromSurface(renderer, buttonQuitter.surface);
      }

      //------------------//

      // Effacement de l'écran
      SDL_RenderClear(renderer);

      // Affichage de l'image de fond
      SDL_RenderCopy(renderer, texturep, NULL, NULL);

      // Affichage des boutons
      SDL_RenderCopy(renderer, buttonHost.texture, NULL, &buttonHost.rect);
      SDL_RenderCopy(renderer, buttonJoin.texture, NULL, &buttonJoin.rect);
      SDL_RenderCopy(renderer, buttonQuitter.texture, NULL, &buttonQuitter.rect);

      //---------//

      // Dessiner le rectangle de saisie de texte pour l'IP
      SDL_SetRenderDrawColor(renderer, 192, 148, 115, 0);
      SDL_RenderFillRect(renderer, &TextInputRectIp);
      // Dessiner le rectangle de saisie de texte pour le port
      SDL_SetRenderDrawColor(renderer, 192, 148, 115, 0);
      SDL_RenderFillRect(renderer, &TextInputRectPort);
      // Dessiner le rectangle de saisie de texte pour le pseudo
      SDL_SetRenderDrawColor(renderer, 192, 148, 115, 0);
      SDL_RenderFillRect(renderer, &TextInputRectPseudo);

      // Dessiner le texte des text box
      SDL_Rect textRectIp = {buttonHost.rect.x * 2.8, buttonHost.rect.y, (widthIp / 2), (buttonJoin.rect.h)};
      SDL_RenderCopy(renderer, textTextureIp, NULL, &textRectIp);
      SDL_Rect textRectPort = {buttonHost.rect.x * 2.8, buttonHost.rect.y * 1.2, (widthPort / 2), (buttonJoin.rect.h)};
      SDL_RenderCopy(renderer, textTexturePort, NULL, &textRectPort);
      SDL_Rect textRectPseudo = {buttonHost.rect.x * 2.8, buttonHost.rect.y * 1.4, (widthPseudo / 2), (buttonJoin.rect.h)};
      SDL_RenderCopy(renderer, textTexturePseudo, NULL, &textRectPseudo);

      

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

  SDL_DestroyTexture(buttonHost.texture);
  SDL_FreeSurface(buttonHost.surface);
  SDL_DestroyTexture(buttonJoin.texture);
  SDL_FreeSurface(buttonJoin.surface);
  SDL_DestroyTexture(buttonQuitter.texture);
  SDL_FreeSurface(buttonQuitter.surface);
  SDL_DestroyTexture(texturep);
  SDL_FreeSurface(imagep);

  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}