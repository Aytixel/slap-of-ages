/**
 * @file test_menu.c
 * @author Arthur Dureau
 * @brief Fichier de création du menu multijoueur
 * @version 1.0
 * @date 03/03/2023
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <assert.h>
#include "timer/timer.h"
#include "test_menu.h"
#include "window/input.h"
#include "window/window.h"

/**
 * @brief Fonction de gestion des évènements
 * 
 * 
 * 
 * @param event
 * @param window
 * @return void
 * 
 */

void handleEvent(SDL_Event *event, window_t *window)
{
  switch (event->type)
  {
  // Si l'utilisateur ferme la fenêtre
  case SDL_QUIT:
    exit(0);
    break;
  case SDL_WINDOWEVENT:
        switch (event->window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
            window->width = event->window.data1;
            window->height = event->window.data2;
            break;
        }
        break;
  }
}

/**
 * @brief Fonction d'initialisation de la SDL
 * 
 * 
 * @return int 
 */

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

/**
 * @brief Fonction de définition de la couleur de fond
 * 
 * 
 * @param renderer 
 * @return void 
 */

void renderDrawColor(SDL_Renderer *renderer)
{
  // Définition de la couleur de fond
  SDL_SetRenderDrawColor(renderer, 100, 10, 0, 255);
}

/**
 * @brief Fonction de check de la librairie SDL_ttf
 * 
 * @param window 
 * @param renderer 
 * @return int 
 */

int checkTTFLib(window_t *window)
{
  // Check de la librairie
  if (TTF_Init() == -1)
  {
    fprintf(stderr, "Erreur lors de l'initialisation de la SDL_ttf : %s", TTF_GetError());
    SDL_DestroyRenderer(window->renderer);
    destroyWindow(&window);
    SDL_Quit();
    return 1;
  }

  return 0;
}




/**
 * @brief Fonction de chargement de la police
 * 
 * @param window 
 * @param renderer 
 * @return TTF_Font* 
 */

TTF_Font *loadFont(window_t *window, SDL_Renderer *renderer)
{
  // Chargement de la police
  TTF_Font *font = TTF_OpenFont("asset/font/8-BIT-WONDER.ttf", 24);
  if (font == NULL)
  {
    fprintf(stderr, "Erreur lors du chargement de la police : %s", TTF_GetError());
    SDL_DestroyRenderer(renderer);
    destroyWindow(&window);
    SDL_Quit();
    return NULL;
  }

  return font;
}


/**
 * @brief Fonction de chargement de la police pour la textbox
 * 
 * @param window 
 * @param renderer 
 * @return TTF_Font* 
 */

TTF_Font *loadFontTextBox(window_t *window, SDL_Renderer *renderer)
{
  // Chargement de la police
  TTF_Font *fontTextBox = TTF_OpenFont("asset/font/arial.ttf", 24);
  if (fontTextBox == NULL)
  {
    fprintf(stderr, "Erreur lors du chargement de la police : %s", TTF_GetError());
    SDL_DestroyRenderer(renderer);
    destroyWindow(&window);
    SDL_Quit();
    return NULL;
  }

  return fontTextBox;
}

/**
 * @brief Set the Text Input Rect object
 * 
 * @param rect 
 */

void setTextInputRect(SDL_Rect rect)
{
  SDL_SetTextInputRect(&rect);
}

int main(int argc, char **argv){

  menu();
  return 1;

}

/**
 * @brief Fonction de création du menu multijoueur
 * 
 * @return int 
 */

int menu()
{

  // Initialisation

  int inoption = 0;
  int box = 0;
  int width = 1000;
  int height = 680;

  // Initialisation de la SDL

  initialisationSDL();

  window_t *window = createWindow("Menu Multijoueur", width, height);

  renderDrawColor(window->renderer);

  checkTTFLib(window);

  TTF_Font *font = loadFont(window, window->renderer);
  TTF_Font *fontTextBox = loadFontTextBox(window, window->renderer);

  // Commencer la saisie de texte
  SDL_StartTextInput();

  // Création des boutons
  SDL_Color color = {52, 36, 20, 0}; // Rouge
  SDL_Color color2 = {255, 255, 255, 0}; // Blanc
  button_t buttonHost;
  button_t buttonJoin;
  button_t buttonQuitter;
  button_t buttonPseudo;
  button_t buttonPort;
  button_t buttonIp;

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

  SDL_Surface *imagep = IMG_Load("asset/pack/PixelBooksVers1.0/RADL_Book4.png");
  if (!imagep)
  {
    printf("Erreur de chargement de l'image : %s", SDL_GetError());
    return -1;
  }
  SDL_Texture *texturep = SDL_CreateTextureFromSurface(window->renderer, imagep);

  // Bouton "HOST"
  createButton(font, "HOST", color, 0.21f, 0.40f, 0.1f, 0.04f, &buttonHost, window);

  // Bouton "JOIN"
  createButton(font, "JOIN", color, 0.21f, 0.475f, 0.1f, 0.04f, &buttonJoin, window);

  // Bouton "QUITTER"
  createButton(font, "QUITTER", color, 0.21f, 0.55f, 0.15f, 0.04f, &buttonQuitter, window);

  // Bouton "PSEUDO"
  createButton(font, "Pseudo", color2,  0.60f, 0.515f, 0.06f, 0.04f, &buttonPseudo, window);

  // Bouton "PORT"
  createButton(font, "Port", color2, 0.60f, 0.435f, 0.06f, 0.04f, &buttonPort, window);

  // Bouton "IP"
  createButton(font, "Ip", color2, 0.60f, 0.35f, 0.06f, 0.04f, &buttonIp, window);

  Textbox_t textboxIp, textboxPort, textboxPseudo;

  SDL_Rect RectIp = {buttonHost.rect.x * 2.8, buttonHost.rect.y, buttonIp.rect.w * 4.5, buttonPort.rect.h};
  SDL_Rect RectPort = {buttonHost.rect.x * 2.8, buttonHost.rect.y * 1.2, buttonIp.rect.w * 4.5, buttonPort.rect.h};
  SDL_Rect RectPseudo = {buttonHost.rect.x * 2.8, buttonHost.rect.y * 1.4, buttonIp.rect.w * 4.5, buttonPort.rect.h};

  createTextbox(font, color, RectIp, &textboxIp, window);
  createTextbox(font, color, RectPort, &textboxPort, window);
  createTextbox(font, color, RectPseudo, &textboxPseudo, window);

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
      handleEvent(&event, window);

      if(height != window->height || width != window->width){
        
        destroyButton(&buttonHost);
        destroyButton(&buttonJoin);
        destroyButton(&buttonQuitter);
        destroyButton(&buttonIp);
        destroyButton(&buttonPort);
        destroyButton(&buttonPseudo);

        SDL_Rect RectIp = {buttonHost.rect.x * 2.8, buttonHost.rect.y, buttonIp.rect.w * 4.5, buttonPort.rect.h};
        SDL_Rect RectPort = {buttonHost.rect.x * 2.8, buttonHost.rect.y * 1.2, buttonIp.rect.w * 4.5, buttonPort.rect.h};
        SDL_Rect RectPseudo = {buttonHost.rect.x * 2.8, buttonHost.rect.y * 1.4, buttonIp.rect.w * 4.5, buttonPort.rect.h};

        createButton(font, "HOST", color, 0.21f, 0.40f, 0.1f, 0.04f, &buttonHost, window);
        createButton(font, "JOIN", color, 0.21f, 0.475f, 0.1f, 0.04f, &buttonJoin, window);
        createButton(font, "QUITTER", color,  0.21f, 0.55f, 0.15f, 0.04f, &buttonQuitter, window);

        createButton(font, "Pseudo", color2,  0.60f, 0.515f, 0.06f, 0.04f, &buttonPseudo, window);
        createButton(font, "Port", color2, 0.60f, 0.435f, 0.06f, 0.04f, &buttonPort, window);
        createButton(font, "Ip", color2, 0.60f, 0.35f, 0.06f, 0.04f, &buttonIp, window);

        createTextbox(font, color, RectIp, &textboxIp, window);
        createTextbox(font, color, RectPort, &textboxPort, window);
        createTextbox(font, color, RectPseudo, &textboxPseudo, window);

        height = window->height;
        width = window->width;

      }

      SDL_Color color = {52, 36, 20, 0};
      buttonHost.surface = TTF_RenderText_Solid(font, "HOST", color);
      buttonHost.texture = SDL_CreateTextureFromSurface(window->renderer, buttonHost.surface);
      buttonJoin.surface = TTF_RenderText_Solid(font, "JOIN", color);
      buttonJoin.texture = SDL_CreateTextureFromSurface(window->renderer, buttonJoin.surface);
      buttonQuitter.surface = TTF_RenderText_Solid(font, "QUITTER", color);
      buttonQuitter.texture = SDL_CreateTextureFromSurface(window->renderer, buttonQuitter.surface);

      buttonIp.surface = TTF_RenderText_Solid(font, "Ip", color2);
      buttonIp.texture = SDL_CreateTextureFromSurface(window->renderer, buttonIp.surface);
      buttonPort.surface = TTF_RenderText_Solid(font, "Port", color2);
      buttonPort.texture = SDL_CreateTextureFromSurface(window->renderer, buttonPort.surface);
      buttonPseudo.surface = TTF_RenderText_Solid(font, "Pseudo", color2);
      buttonPseudo.texture = SDL_CreateTextureFromSurface(window->renderer, buttonPseudo.surface);

      // Si l'utilisateur clique sur le bouton "QUITTER"
      if (isMouseClickInRect(event, buttonQuitter.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
      {
        SDL_DestroyTexture(buttonHost.texture);
        SDL_FreeSurface(buttonHost.surface);
        SDL_DestroyTexture(buttonJoin.texture);
        SDL_FreeSurface(buttonJoin.surface);
        SDL_DestroyTexture(buttonIp.texture);
        SDL_FreeSurface(buttonPort.surface);
        SDL_DestroyTexture(buttonPseudo.texture);
        SDL_FreeSurface(buttonIp.surface);
        SDL_DestroyTexture(buttonPort.texture);
        SDL_FreeSurface(buttonPseudo.surface);

        // Quitter le programme
        exit(0);
      }

      // Si l'utilisateur clique dans l'un des rectangles de texte
      if (isMouseClickInRect(event, textboxIp.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) && inoption == 0)
      {
        box = 1;
      }
      else if (isMouseClickInRect(event, textboxPort.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) && inoption == 0)
      {
        box = 2;
      }
      else if (isMouseClickInRect(event, textboxPseudo.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) && inoption == 0)
      {
        box = 3;
      }
      else
      {
      }

      if (box == 1)
      {
        updateTextboxText(event, font, inputTextIp, &widthIp, &heightIp);
      }
      else if (box == 2)
      {
        updateTextboxText(event, font, inputTextPort, &widthPort, &heightPort);
      }
      else if (box == 3)
      {
        updateTextboxText(event, font, inputTextPseudo, &widthPseudo, &heightPseudo);
      }
      else
      {
      }

      // Rendre le texte à partir de la surface
      SDL_FreeSurface(textSurfaceIp);
      textSurfaceIp = TTF_RenderText_Solid(fontTextBox, inputTextIp, textColor);
      textTextureIp = SDL_CreateTextureFromSurface(window->renderer, textSurfaceIp);
      SDL_FreeSurface(textSurfacePort);
      textSurfacePort = TTF_RenderText_Solid(fontTextBox, inputTextPort, textColor);
      textTexturePort = SDL_CreateTextureFromSurface(window->renderer, textSurfacePort);
      SDL_FreeSurface(textSurfacePseudo);
      textSurfacePseudo = TTF_RenderText_Solid(fontTextBox, inputTextPseudo, textColor);
      textTexturePseudo = SDL_CreateTextureFromSurface(window->renderer, textSurfacePseudo);

      //---------//

      // Si l'utilisateur met sa souris au dessus
      SDL_Point mousePoint = getMousePosition();
      if (SDL_PointInRect(&mousePoint, &buttonHost.rect))
      {
        SDL_Color color = {52, 36, 155, 0};
        buttonHost.surface = TTF_RenderText_Solid(font, "HOST", color);
        buttonHost.texture = SDL_CreateTextureFromSurface(window->renderer, buttonHost.surface);
      }
      else if (SDL_PointInRect(&mousePoint, &buttonJoin.rect))
      {
        SDL_Color color = {52, 36, 155, 0};
        buttonJoin.surface = TTF_RenderText_Solid(font, "JOIN", color);
        buttonJoin.texture = SDL_CreateTextureFromSurface(window->renderer, buttonJoin.surface);
      }
      else if (SDL_PointInRect(&mousePoint, &buttonQuitter.rect))
      {
        SDL_Color color = {52, 36, 155, 0};
        buttonQuitter.surface = TTF_RenderText_Solid(font, "QUITTER", color);
        buttonQuitter.texture = SDL_CreateTextureFromSurface(window->renderer, buttonQuitter.surface);
      }
      else
      {
        SDL_Color color = {52, 36, 20, 0};
        buttonHost.surface = TTF_RenderText_Solid(font, "HOST", color);
        buttonHost.texture = SDL_CreateTextureFromSurface(window->renderer, buttonHost.surface);
        buttonJoin.surface = TTF_RenderText_Solid(font, "JOIN", color);
        buttonJoin.texture = SDL_CreateTextureFromSurface(window->renderer, buttonJoin.surface);
        buttonQuitter.surface = TTF_RenderText_Solid(font, "QUITTER", color);
        buttonQuitter.texture = SDL_CreateTextureFromSurface(window->renderer, buttonQuitter.surface);
      }

      //------------------//

      // Effacement de l'écran
      SDL_RenderClear(window->renderer);

      // Affichage de l'image de fond
      SDL_RenderCopy(window->renderer, texturep, NULL, NULL);

      // Affichage des boutons
      SDL_RenderCopy(window->renderer, buttonHost.texture, NULL, &buttonHost.rect);
      SDL_RenderCopy(window->renderer, buttonJoin.texture, NULL, &buttonJoin.rect);
      SDL_RenderCopy(window->renderer, buttonQuitter.texture, NULL, &buttonQuitter.rect);
      SDL_RenderCopy(window->renderer, buttonIp.texture, NULL, &buttonIp.rect);
      SDL_RenderCopy(window->renderer, buttonPort.texture, NULL, &buttonPort.rect);
      SDL_RenderCopy(window->renderer, buttonPseudo.texture, NULL, &buttonPseudo.rect);

      //---------//

      
      // Dessiner le rectangle de saisie de texte pour l'IP
      SDL_SetRenderDrawColor(window->renderer, 192, 148, 115, 0);
      SDL_RenderFillRect(window->renderer, &RectIp);
      // Dessiner le rectangle de saisie de texte pour le port
      SDL_SetRenderDrawColor(window->renderer, 192, 148, 115, 0);
      SDL_RenderFillRect(window->renderer, &RectPort);
      // Dessiner le rectangle de saisie de texte pour le pseudo
      SDL_SetRenderDrawColor(window->renderer, 192, 148, 115, 0);
      SDL_RenderFillRect(window->renderer, &RectPseudo);
      

      // Dessiner le texte des text box
      SDL_Rect textRectIp = {buttonHost.rect.x * 2.8, buttonHost.rect.y, (widthIp / 2), (buttonJoin.rect.h)};
      SDL_RenderCopy(window->renderer, textTextureIp, NULL, &textRectIp);
      SDL_Rect textRectPort = {buttonHost.rect.x * 2.8, buttonHost.rect.y * 1.2, (widthPort / 2), (buttonJoin.rect.h)};
      SDL_RenderCopy(window->renderer, textTexturePort, NULL, &textRectPort);
      SDL_Rect textRectPseudo = {buttonHost.rect.x * 2.8, buttonHost.rect.y * 1.4, (widthPseudo / 2), (buttonJoin.rect.h)};
      SDL_RenderCopy(window->renderer, textTexturePseudo, NULL, &textRectPseudo);

      

      // Mise à jour de l'affichage
      SDL_RenderPresent(window->renderer);

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

  destroyButton(&buttonHost);
  destroyButton(&buttonJoin);
  destroyButton(&buttonQuitter);
  destroyButton(&buttonIp);
  destroyButton(&buttonPort);
  destroyButton(&buttonPseudo);

  destroyTextbox(&textboxIp);
  destroyTextbox(&textboxPort);
  destroyTextbox(&textboxPseudo);

  SDL_DestroyTexture(texturep);
  SDL_FreeSurface(imagep);


  TTF_CloseFont(font);
  SDL_DestroyRenderer(window->renderer);
  destroyWindow(&window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}