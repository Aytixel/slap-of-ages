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

extern menu_t *createMenu(window_t *window, client_game_data_t *game_data)
{
  menu_t *menu = malloc(sizeof(menu_t));

  menu->text_font = loadFont("asset/font/8-BIT-WONDER.ttf", 24);

  if (menu->text_font == NULL)
  {
    free(menu);

    return NULL;
  }

  menu->textbox_font = loadFont("asset/font/arial.ttf", 24);

  if (menu->textbox_font == NULL)
  {
    TTF_CloseFont(menu->text_font);
    free(menu);

    return NULL;
  }

  SDL_Color light_text_color = {255, 255, 255, 0};
  menu->light_text_color = light_text_color;

  SDL_Color dark_text_color = {52, 36, 20, 0};
  menu->dark_text_color = dark_text_color;

  SDL_Color selected_button_color = {52, 36, 155, 0};
  menu->selected_button_color = selected_button_color;

  menu->join_button_color = dark_text_color;
  menu->quit_button_color = dark_text_color;

  menu->background_sprite = loadSprite(window, "asset/pack/PixelBooksVers1.0/RADL_Book4.png");

  menu->join_button = createButton(menu->text_font, "JOIN", menu->join_button_color, 0.21f, 0.475f, 0.1f, 0.04f, window);
  menu->quit_button = createButton(menu->text_font, "QUITTER", menu->quit_button_color, 0.21f, 0.55f, 0.15f, 0.04f, window);
  menu->pseudo_button = createButton(menu->text_font, "Pseudo", menu->light_text_color, 0.60f, 0.515f, 0.06f, 0.04f, window);
  menu->port_button = createButton(menu->text_font, "Port", menu->light_text_color, 0.60f, 0.435f, 0.06f, 0.04f, window);
  menu->hostname_button = createButton(menu->text_font, "Ip", menu->light_text_color, 0.60f, 0.35f, 0.06f, 0.04f, window);

  // temporaire
  SDL_Rect hostname_rect = {menu->join_button->rect.x * 2.8, menu->join_button->rect.y, menu->hostname_button->rect.w * 4.5, menu->port_button->rect.h};
  SDL_Rect port_rect = {menu->join_button->rect.x * 2.8, menu->join_button->rect.y * 1.2, menu->hostname_button->rect.w * 4.5, menu->port_button->rect.h};
  SDL_Rect pseudo_rect = {menu->join_button->rect.x * 2.8, menu->join_button->rect.y * 1.4, menu->hostname_button->rect.w * 4.5, menu->port_button->rect.h};

  menu->hostname_textbox = createTextbox(menu->text_font, menu->join_button_color, hostname_rect, window);
  menu->port_textbox = createTextbox(menu->text_font, menu->join_button_color, port_rect, window);
  menu->pseudo_textbox = createTextbox(menu->text_font, menu->join_button_color, pseudo_rect, window);

  menu->hostname_input_text[0] = 0;
  menu->port_input_text[0] = 0;
  menu->pseudo_input_text[0] = 0;

  menu->widthIp = 0;
  menu->widthPort = 0;
  menu->widthPseudo = 0;
  menu->heightIp = 0;
  menu->heightPort = 0;
  menu->heightPseudo = 0;

  char port_input_text[6];

  SDL_itoa(game_data->port, port_input_text, 10);

  strcpy(menu->hostname_input_text, game_data->hostname);
  strcpy(menu->port_input_text, port_input_text);
  strcpy(menu->pseudo_input_text, game_data->pseudo);

  return menu;
}

extern int menuEventHandler(client_game_data_t *game_data, SDL_Event *event, menu_t *menu)
{
  static int selected_textbox = 0;

  if (isMouseClickInRect(*event, menu->quit_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
    return 2;

  if (isMouseClickInRect(*event, menu->join_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
  {
    strcpy(game_data->pseudo, menu->pseudo_input_text);
    strcpy(game_data->hostname, menu->hostname_input_text);
    game_data->port = atoi(menu->port_input_text);

    return 1;
  }

  if (isMouseClickInRect(*event, menu->hostname_textbox->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
    selected_textbox = 1;
  else if (isMouseClickInRect(*event, menu->port_textbox->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
    selected_textbox = 2;
  else if (isMouseClickInRect(*event, menu->pseudo_textbox->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
    selected_textbox = 3;

  switch (selected_textbox)
  {
  case 1:
    SDL_StartTextInput();
    updateTextboxText(*event, menu->text_font, menu->hostname_input_text, &menu->widthIp, &menu->heightIp);
    break;
  case 2:
    SDL_StartTextInput();
    updateTextboxText(*event, menu->text_font, menu->port_input_text, &menu->widthPort, &menu->heightPort);
    break;
  case 3:
    SDL_StartTextInput();
    updateTextboxText(*event, menu->text_font, menu->pseudo_input_text, &menu->widthPseudo, &menu->heightPseudo);
    break;
  default:
    SDL_StopTextInput();
    break;
  }

  SDL_Point mousePoint = getMousePosition();

  if (SDL_PointInRect(&mousePoint, &menu->join_button->rect))
    menu->join_button_color = menu->selected_button_color;
  else
    menu->join_button_color = menu->dark_text_color;

  if (SDL_PointInRect(&mousePoint, &menu->quit_button->rect))
    menu->quit_button_color = menu->selected_button_color;
  else
    menu->quit_button_color = menu->dark_text_color;

  return 0;
}

extern int menuRenderer(window_t *window, menu_t *menu)
{
  SDL_SetRenderDrawColor(window->renderer, 100, 10, 0, 255);

  SDL_Rect hostname_rect = {menu->join_button->rect.x * 2.8, menu->join_button->rect.y, menu->hostname_button->rect.w * 4.5, menu->port_button->rect.h};
  SDL_Rect port_rect = {menu->join_button->rect.x * 2.8, menu->join_button->rect.y * 1.2, menu->hostname_button->rect.w * 4.5, menu->port_button->rect.h};
  SDL_Rect pseudo_rect = {menu->join_button->rect.x * 2.8, menu->join_button->rect.y * 1.4, menu->hostname_button->rect.w * 4.5, menu->port_button->rect.h};

  SDL_DestroyTexture(menu->join_button->texture);
  SDL_FreeSurface(menu->join_button->surface);
  menu->join_button->surface = TTF_RenderText_Solid(menu->text_font, "JOIN", menu->join_button_color);
  menu->join_button->texture = SDL_CreateTextureFromSurface(window->renderer, menu->join_button->surface);

  SDL_DestroyTexture(menu->quit_button->texture);
  SDL_FreeSurface(menu->quit_button->surface);
  menu->quit_button->surface = TTF_RenderText_Solid(menu->text_font, "QUITTER", menu->quit_button_color);
  menu->quit_button->texture = SDL_CreateTextureFromSurface(window->renderer, menu->quit_button->surface);

  SDL_DestroyTexture(menu->hostname_button->texture);
  SDL_FreeSurface(menu->hostname_button->surface);
  menu->hostname_button->surface = TTF_RenderText_Solid(menu->text_font, "Ip", menu->light_text_color);
  menu->hostname_button->texture = SDL_CreateTextureFromSurface(window->renderer, menu->hostname_button->surface);

  SDL_DestroyTexture(menu->port_button->texture);
  SDL_FreeSurface(menu->port_button->surface);
  menu->port_button->surface = TTF_RenderText_Solid(menu->text_font, "Port", menu->light_text_color);
  menu->port_button->texture = SDL_CreateTextureFromSurface(window->renderer, menu->port_button->surface);

  SDL_DestroyTexture(menu->pseudo_button->texture);
  SDL_FreeSurface(menu->pseudo_button->surface);
  menu->pseudo_button->surface = TTF_RenderText_Solid(menu->text_font, "Pseudo", menu->light_text_color);
  menu->pseudo_button->texture = SDL_CreateTextureFromSurface(window->renderer, menu->pseudo_button->surface);

  // Affichage de l'image de fond
  SDL_RenderCopy(window->renderer, menu->background_sprite->texture, NULL, NULL);

  // Affichage des boutons
  SDL_RenderCopy(window->renderer, menu->join_button->texture, NULL, &menu->join_button->rect);
  SDL_RenderCopy(window->renderer, menu->quit_button->texture, NULL, &menu->quit_button->rect);
  SDL_RenderCopy(window->renderer, menu->hostname_button->texture, NULL, &menu->hostname_button->rect);
  SDL_RenderCopy(window->renderer, menu->port_button->texture, NULL, &menu->port_button->rect);
  SDL_RenderCopy(window->renderer, menu->pseudo_button->texture, NULL, &menu->pseudo_button->rect);

  //---------//

  // Dessiner le rectangle de saisie de texte
  drawRect(window->renderer, hostname_rect, 192, 148, 115, 0);
  drawRect(window->renderer, port_rect, 192, 148, 115, 0);
  drawRect(window->renderer, pseudo_rect, 192, 148, 115, 0);

  SDL_Rect textRectIp = {menu->join_button->rect.x * 2.8, menu->join_button->rect.y, (menu->widthIp / 2), (menu->join_button->rect.h)};
  drawText(window->renderer, menu->textbox_font, menu->hostname_input_text, textRectIp, menu->light_text_color);

  SDL_Rect textRectPort = {menu->join_button->rect.x * 2.8, menu->join_button->rect.y * 1.2, (menu->widthPort / 2), (menu->join_button->rect.h)};
  drawText(window->renderer, menu->textbox_font, menu->port_input_text, textRectPort, menu->light_text_color);

  SDL_Rect textRectPseudo = {menu->join_button->rect.x * 2.8, menu->join_button->rect.y * 1.4, (menu->widthPseudo / 2), (menu->join_button->rect.h)};
  drawText(window->renderer, menu->textbox_font, menu->pseudo_input_text, textRectPseudo, menu->light_text_color);

  return 0;
}

extern int deleteMenu(menu_t **menu)
{
  if (menu == NULL || *menu == NULL)
    return -1;

  destroySprite(&(*menu)->background_sprite);

  TTF_CloseFont((*menu)->text_font);
  TTF_CloseFont((*menu)->textbox_font);

  destroyButton(&(*menu)->join_button);
  destroyButton(&(*menu)->quit_button);
  destroyButton(&(*menu)->pseudo_button);
  destroyButton(&(*menu)->port_button);
  destroyButton(&(*menu)->hostname_button);

  destroyTextbox(&(*menu)->pseudo_textbox);
  destroyTextbox(&(*menu)->port_textbox);
  destroyTextbox(&(*menu)->hostname_textbox);

  free(*menu);
  *menu = NULL;

  return 0;
}