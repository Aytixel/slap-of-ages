/**
 * @file menu.c
 * @author Arthur Dureau, Lucas Dureau
 * @brief Fichier de création du menu multijoueur
 * @version 1.0
 * @date 03/03/2023
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

  menu->textbox_font = loadFont("asset/font/arial.ttf", 50);

  if (menu->textbox_font == NULL)
  {
    TTF_CloseFont(menu->text_font);
    free(menu);

    return NULL;
  }

  SDL_Rect rect = {0, 0, 0, 0};

  menu->hostname_rect = rect;
  menu->port_rect = rect;
  menu->pseudo_rect = rect;

  SDL_Color light_text_color = {255, 255, 255, 0};
  SDL_Color dark_text_color = {52, 36, 20, 0};

  int book_states[] = {4, 4, 1, -1};
  menu->book_animation = createAnimation(16, book_states, loadSprite(window, "asset/sprite/menu/book.png"), 8);
  menu->book_animation->current_state = BOOK_OPEN_ANIM;

  menu->join_button = createButton(window, menu->text_font, "REJOINDRE", dark_text_color, light_text_color);
  menu->quit_button = createButton(window, menu->text_font, "QUITTER", dark_text_color, light_text_color);

  menu->hostname_label = createTextSprite(window, menu->text_font, "Ip", light_text_color);
  menu->port_label = createTextSprite(window, menu->text_font, "Port", light_text_color);
  menu->pseudo_label = createTextSprite(window, menu->text_font, "Pseudo", light_text_color);

  char port_textbox_default_text[6];

  SDL_itoa(game_data->port, port_textbox_default_text, 10);

  menu->hostname_textbox = createTextbox(window, menu->textbox_font, game_data->hostname, dark_text_color, HOSTNAME_SIZE);
  menu->port_textbox = createTextbox(window, menu->textbox_font, port_textbox_default_text, dark_text_color, 7);
  menu->pseudo_textbox = createTextbox(window, menu->textbox_font, game_data->pseudo, dark_text_color, PSEUDO_SIZE);
  menu->hostname_textbox->max_width = 440;
  menu->port_textbox->max_width = 440;
  menu->pseudo_textbox->max_width = 440;

  return menu;
}

extern int menuEventHandler(client_game_data_t *game_data, SDL_Event *event, menu_t *menu)
{
  static int selected_textbox = 0;

  if (event->type == SDL_QUIT || isMouseClickInRect(*event, menu->quit_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
    menu->book_animation->current_state = BOOK_CLOSE_ANIM;

  if (isMouseClickInRect(*event, menu->join_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) && strlen(menu->pseudo_textbox->text) >= 3)
  {
    strcpy(game_data->hostname, menu->hostname_textbox->text);
    game_data->port = atoi(menu->port_textbox->text);
    strcpy(game_data->pseudo, menu->pseudo_textbox->text);

    return 1;
  }

  if (isMouseClickInRect(*event, menu->hostname_rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
    selected_textbox = 1;
  else if (isMouseClickInRect(*event, menu->port_rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
    selected_textbox = 2;
  else if (isMouseClickInRect(*event, menu->pseudo_rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
    selected_textbox = 3;

  switch (selected_textbox)
  {
  case 1:
    SDL_SetTextInputRect(&(menu->hostname_textbox->rect));
    SDL_StartTextInput();
    updateTextbox(event, menu->hostname_textbox);
    break;
    SDL_SetTextInputRect(&(menu->port_textbox->rect));
  case 2:
    SDL_StartTextInput();
    updateTextbox(event, menu->port_textbox);
    break;
  case 3:
    SDL_SetTextInputRect(&(menu->pseudo_textbox->rect));
    SDL_StartTextInput();
    updateTextbox(event, menu->pseudo_textbox);
    break;
  default:
    SDL_StopTextInput();
    break;
  }

  SDL_Point mouse_point = getMousePosition();

  menu->join_button->is_selected = SDL_PointInRect(&mouse_point, &menu->join_button->rect);
  menu->quit_button->is_selected = SDL_PointInRect(&mouse_point, &menu->quit_button->rect);

  return 0;
}

extern int menuRenderer(window_t *window, menu_t *menu)
{
  float scale_factor = window->height_scale_factor;

  SDL_SetRenderDrawColor(window->renderer, 192, 148, 115, 0);

  // Affichage de l'image de fond
  SDL_Point book_position = {0, 0};
  if (updateAnimation(menu->book_animation, menu->book_animation->current_state, scale_factor * 70, &book_position, window, TRANSFORM_ORIGIN_CENTER) == 1)
  {
    if (menu->book_animation->current_state == BOOK_OPEN_ANIM)
      menu->book_animation->current_state = BOOK_OPENED_ANIM;
    else if (menu->book_animation->current_state == BOOK_CLOSE_ANIM)
      return 0;
  }

  if (menu->book_animation->current_state == BOOK_OPENED_ANIM)
  {
    // Affichage des boutons
    menu->join_button->rect = positionFromCenter(window, menu->join_button->sprite->width * scale_factor, menu->join_button->sprite->height * scale_factor, -140 * scale_factor, -25 * scale_factor, TRANSFORM_ORIGIN_CENTER);
    menu->quit_button->rect = positionFromCenter(window, menu->quit_button->sprite->width * scale_factor, menu->quit_button->sprite->height * scale_factor, -140 * scale_factor, 25 * scale_factor, TRANSFORM_ORIGIN_CENTER);

    renderButton(window, menu->join_button);
    renderButton(window, menu->quit_button);

    // Affichage des étiquettes des zones de saisie de texte
    SDL_Rect hostname_label_rect = positionFromCenter(window, menu->hostname_label->width * scale_factor, menu->hostname_label->height * scale_factor, 50 * scale_factor, -125 * scale_factor, TRANSFORM_ORIGIN_LEFT);
    SDL_Rect port_label_rect = positionFromCenter(window, menu->port_label->width * scale_factor, menu->port_label->height * scale_factor, 50 * scale_factor, -50 * scale_factor, TRANSFORM_ORIGIN_LEFT);
    SDL_Rect pseudo_label_rect = positionFromCenter(window, menu->pseudo_label->width * scale_factor, menu->pseudo_label->height * scale_factor, 50 * scale_factor, 25 * scale_factor, TRANSFORM_ORIGIN_LEFT);

    SDL_RenderCopy(window->renderer, menu->hostname_label->texture, NULL, &hostname_label_rect);
    SDL_RenderCopy(window->renderer, menu->port_label->texture, NULL, &port_label_rect);
    SDL_RenderCopy(window->renderer, menu->pseudo_label->texture, NULL, &pseudo_label_rect);

    // Affichage des zones de saisie de texte
    float textbox_font_scale_factor = 0.5;

    menu->hostname_textbox->rect = positionFromCenter(window, menu->hostname_textbox->text_rect.w * scale_factor * textbox_font_scale_factor, menu->hostname_textbox->text_rect.h * scale_factor * textbox_font_scale_factor, 50 * scale_factor, -90 * scale_factor, TRANSFORM_ORIGIN_LEFT);
    menu->port_textbox->rect = positionFromCenter(window, menu->port_textbox->text_rect.w * scale_factor * textbox_font_scale_factor, menu->port_textbox->text_rect.h * scale_factor * textbox_font_scale_factor, 50 * scale_factor, -15 * scale_factor, TRANSFORM_ORIGIN_LEFT);
    menu->pseudo_textbox->rect = positionFromCenter(window, menu->pseudo_textbox->text_rect.w * scale_factor * textbox_font_scale_factor, menu->pseudo_textbox->text_rect.h * scale_factor * textbox_font_scale_factor, 50 * scale_factor, 60 * scale_factor, TRANSFORM_ORIGIN_LEFT);

    menu->hostname_rect = menu->hostname_textbox->rect;
    menu->hostname_rect.x -= 5;
    menu->hostname_rect.y -= 5;
    menu->hostname_rect.w = 227 * scale_factor;
    menu->hostname_rect.h += 10;

    menu->port_rect = menu->port_textbox->rect;
    menu->port_rect.x -= 5;
    menu->port_rect.y -= 5;
    menu->port_rect.w = 227 * scale_factor;
    menu->port_rect.h += 10;

    menu->pseudo_rect = menu->pseudo_textbox->rect;
    menu->pseudo_rect.x -= 5;
    menu->pseudo_rect.y -= 5;
    menu->pseudo_rect.w = 227 * scale_factor;
    menu->pseudo_rect.h += 10;

    drawRect(window->renderer, menu->hostname_rect, 192, 148, 115, 0);
    drawRect(window->renderer, menu->port_rect, 192, 148, 115, 0);
    drawRect(window->renderer, menu->pseudo_rect, 192, 148, 115, 0);

    renderTextbox(window, menu->hostname_textbox);
    renderTextbox(window, menu->port_textbox);
    renderTextbox(window, menu->pseudo_textbox);
  }

  return 1;
}

extern int deleteMenu(menu_t **menu)
{
  if (menu == NULL || *menu == NULL)
    return -1;

  destroyAnimation(&(*menu)->book_animation);

  TTF_CloseFont((*menu)->text_font);
  TTF_CloseFont((*menu)->textbox_font);

  destroyButton(&(*menu)->join_button);
  destroyButton(&(*menu)->quit_button);

  destroySprite(&(*menu)->hostname_label);
  destroySprite(&(*menu)->port_label);
  destroySprite(&(*menu)->pseudo_label);

  destroyTextbox(&(*menu)->pseudo_textbox);
  destroyTextbox(&(*menu)->port_textbox);
  destroyTextbox(&(*menu)->hostname_textbox);

  free(*menu);
  *menu = NULL;

  return 0;
}