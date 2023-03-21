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
#include "menu/menu.h"
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

/**
 * @brief Fonction de création du menu multijoueur
 *
 * @return int
 */

int menu_event(connection_t *connection, SDL_Event *event, menu_t *menu)
{

  static int box = 0;
  static char inputTextIp[1024] = "";
  static char inputTextPort[6] = "";
  static char inputTextPseudo[16] = "";
  menu->inputTextIp = inputTextIp;
  menu->inputTextPort = inputTextPort;
  menu->inputTextPseudo = inputTextPseudo;

  connection->pseudo = malloc(sizeof(char) * 1024);
  connection->ip = malloc(sizeof(char) * 1024);

  if (isMouseClickInRect(*event, menu->buttonQuitter.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
  {
    SDL_DestroyTexture(menu->buttonHost.texture);
    SDL_FreeSurface(menu->buttonHost.surface);
    SDL_DestroyTexture(menu->buttonJoin.texture);
    SDL_FreeSurface(menu->buttonJoin.surface);
    SDL_DestroyTexture(menu->buttonIp.texture);
    SDL_FreeSurface(menu->buttonPort.surface);
    SDL_DestroyTexture(menu->buttonPseudo.texture);
    SDL_FreeSurface(menu->buttonIp.surface);
    SDL_DestroyTexture(menu->buttonPort.texture);
    SDL_FreeSurface(menu->buttonPseudo.surface);

    return 2;
  }

  if (isMouseClickInRect(*event, menu->buttonHost.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
  {
  }

  if (isMouseClickInRect(*event, menu->buttonJoin.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
  {

    strcpy(connection->pseudo, inputTextPseudo); // Assigner une valeur au pseudo
    strcpy(connection->ip, inputTextIp);         // Assigner une valeur au pseudo
    connection->port = atoi(inputTextPort);      // Assigner une valeur au pseudo

    // Libération de la mémoire

    destroyWindow(&menu->window);
    //printf("Destroy window\n");

    return 1;
  }

  if (isMouseClickInRect(*event, menu->textboxIp.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
  {
    box = 1;
  }
  else if (isMouseClickInRect(*event, menu->textboxPort.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
  {
    box = 2;
  }
  else if (isMouseClickInRect(*event, menu->textboxPseudo.rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
  {
    box = 3;
  }
  else
  {
  }

  if (box == 1)
  {
    SDL_StartTextInput();
    updateTextboxText(*event, menu->font, menu->inputTextIp, &menu->widthIp, &menu->heightIp);
  }
  else if (box == 2)
  {
    SDL_StartTextInput();
    updateTextboxText(*event, menu->font, menu->inputTextPort, &menu->widthPort, &menu->heightPort);
  }
  else if (box == 3)
  {
    SDL_StartTextInput();
    updateTextboxText(*event, menu->font, menu->inputTextPseudo, &menu->widthPseudo, &menu->heightPseudo);
  }
  else
  {
    SDL_StopTextInput();
  }

  SDL_Point mousePoint = getMousePosition();
  if (SDL_PointInRect(&mousePoint, &menu->buttonHost.rect))
  {
    SDL_Color color = {52, 36, 155, 0};
    menu->colorH = color;
  }
  else if (SDL_PointInRect(&mousePoint, &menu->buttonJoin.rect))
  {
    SDL_Color color = {52, 36, 155, 0};
    menu->colorJ = color;
  }
  else if (SDL_PointInRect(&mousePoint, &menu->buttonQuitter.rect))
  {
    SDL_Color color = {52, 36, 155, 0};
    menu->colorQ = color;
  }
  else
  {
    SDL_Color color = {52, 36, 20, 0};
    menu->colorH = color;
    menu->colorJ = color;
    menu->colorQ = color;
  }

  return 0;
}

int menu_renderer(window_t *window, menu_t *menu)
{
  // Initialisation

  renderDrawColor(window->renderer);

  TTF_Font *font = loadFont(window, window->renderer);
  TTF_Font *fontTextBox = loadFontTextBox(window, window->renderer);

  menu->window = window;
  menu->font = font;
  SDL_Color color2 = {255, 255, 255, 0}; // Blanc

  SDL_Color textColor = {255, 255, 255};

  SDL_Surface *imagep = IMG_Load("asset/pack/PixelBooksVers1.0/RADL_Book4.png");
  if (!imagep)
  {
    printf("Erreur de chargement de l'image : %s", SDL_GetError());
    return -1;
  }
  SDL_Texture *texturep = SDL_CreateTextureFromSurface(window->renderer, imagep);

  createButton(menu->font, "HOST", menu->colorH, 0.21f, 0.40f, 0.1f, 0.04f, &menu->buttonHost, window);
  createButton(menu->font, "JOIN", menu->colorJ, 0.21f, 0.475f, 0.1f, 0.04f, &menu->buttonJoin, window);
  createButton(menu->font, "QUITTER", menu->colorQ, 0.21f, 0.55f, 0.15f, 0.04f, &menu->buttonQuitter, window);
  createButton(font, "Pseudo", color2, 0.60f, 0.515f, 0.06f, 0.04f, &menu->buttonPseudo, window);
  createButton(font, "Port", color2, 0.60f, 0.435f, 0.06f, 0.04f, &menu->buttonPort, window);
  createButton(font, "Ip", color2, 0.60f, 0.35f, 0.06f, 0.04f, &menu->buttonIp, window);
  
  menu->texturep = SDL_CreateTextureFromSurface(window->renderer, imagep);

  SDL_Rect RectIp = {menu->buttonHost.rect.x * 2.8, menu->buttonHost.rect.y, menu->buttonIp.rect.w * 4.5, menu->buttonPort.rect.h};
  SDL_Rect RectPort = {menu->buttonHost.rect.x * 2.8, menu->buttonHost.rect.y * 1.2, menu->buttonIp.rect.w * 4.5, menu->buttonPort.rect.h};
  SDL_Rect RectPseudo = {menu->buttonHost.rect.x * 2.8, menu->buttonHost.rect.y * 1.4, menu->buttonIp.rect.w * 4.5, menu->buttonPort.rect.h};

  createTextbox(font, menu->colorH, RectIp, &menu->textboxIp, window);
  createTextbox(font, menu->colorH, RectPort, &menu->textboxPort, window);
  createTextbox(font, menu->colorH, RectPseudo, &menu->textboxPseudo, window);

  menu->buttonHost.surface = TTF_RenderText_Solid(menu->font, "HOST", menu->colorH);
  menu->buttonHost.texture = SDL_CreateTextureFromSurface(window->renderer, menu->buttonHost.surface);
  menu->buttonJoin.surface = TTF_RenderText_Solid(menu->font, "JOIN", menu->colorJ);
  menu->buttonJoin.texture = SDL_CreateTextureFromSurface(window->renderer, menu->buttonJoin.surface);
  menu->buttonQuitter.surface = TTF_RenderText_Solid(menu->font, "QUITTER", menu->colorQ);
  menu->buttonQuitter.texture = SDL_CreateTextureFromSurface(window->renderer, menu->buttonQuitter.surface);

  menu->buttonIp.surface = TTF_RenderText_Solid(font, "Ip", color2);
  menu->buttonIp.texture = SDL_CreateTextureFromSurface(window->renderer, menu->buttonIp.surface);
  menu->buttonPort.surface = TTF_RenderText_Solid(font, "Port", color2);
  menu->buttonPort.texture = SDL_CreateTextureFromSurface(window->renderer, menu->buttonPort.surface);
  menu->buttonPseudo.surface = TTF_RenderText_Solid(font, "Pseudo", color2);
  menu->buttonPseudo.texture = SDL_CreateTextureFromSurface(window->renderer, menu->buttonPseudo.surface);

  //SDL_RenderClear(window->renderer);

  // Affichage de l'image de fond
  SDL_RenderCopy(window->renderer, texturep, NULL, NULL);

  // Affichage des boutons
  SDL_RenderCopy(window->renderer, menu->buttonHost.texture, NULL, &menu->buttonHost.rect);
  SDL_RenderCopy(window->renderer, menu->buttonJoin.texture, NULL, &menu->buttonJoin.rect);
  SDL_RenderCopy(window->renderer, menu->buttonQuitter.texture, NULL, &menu->buttonQuitter.rect);
  SDL_RenderCopy(window->renderer, menu->buttonIp.texture, NULL, &menu->buttonIp.rect);
  SDL_RenderCopy(window->renderer, menu->buttonPort.texture, NULL, &menu->buttonPort.rect);
  SDL_RenderCopy(window->renderer, menu->buttonPseudo.texture, NULL, &menu->buttonPseudo.rect);

  //---------//

  // Dessiner le rectangle de saisie de texte
  drawRect(window->renderer, RectIp, 192, 148, 115, 0);
  drawRect(window->renderer, RectPort, 192, 148, 115, 0);
  drawRect(window->renderer, RectPseudo, 192, 148, 115, 0);

  //SDL_RenderPresent(window->renderer);

  SDL_Rect textRectIp = {menu->buttonHost.rect.x * 2.8, menu->buttonHost.rect.y, (menu->widthIp / 2), (menu->buttonJoin.rect.h)};
  drawText(window->renderer, fontTextBox, menu->inputTextIp, textRectIp, textColor);

  SDL_Rect textRectPort = {menu->buttonHost.rect.x * 2.8, menu->buttonHost.rect.y * 1.2, (menu->widthPort / 2), (menu->buttonJoin.rect.h)};
  drawText(window->renderer, fontTextBox, menu->inputTextPort, textRectPort, textColor);

  SDL_Rect textRectPseudo = {menu->buttonHost.rect.x * 2.8, menu->buttonHost.rect.y * 1.4, (menu->widthPseudo / 2), (menu->buttonJoin.rect.h)};
  drawText(window->renderer, fontTextBox, menu->inputTextPseudo, textRectPseudo, textColor);



  return 0;
}