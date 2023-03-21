/**
 * @file input.c
 * @author Arthur Dureau
 * @brief Fichier de création des boutons et des textes
 * @version 1.0
 * @date 03/03/2023
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "input.h"
#include "window.h"

button_t *createButton(TTF_Font *font, const char *buttonText, SDL_Color color, float buttonXRatio, float buttonYRatio, float buttonWidthRatio, float buttonHeightRatio, window_t *window)
{
    button_t *button = malloc(sizeof(button_t));

    // Création de la surface du bouton
    button->surface = TTF_RenderText_Solid(font, buttonText, color);

    // Création de la texture du bouton à partir de la surface
    button->texture = SDL_CreateTextureFromSurface(window->renderer, button->surface);

    // Calcul des dimensions du bouton en pixels
    button->rect.w = (int)(window->width * buttonWidthRatio);
    button->rect.h = (int)(window->height * buttonHeightRatio);

    // Calcul de la position du bouton en pixels
    button->rect.x = (int)(window->width * buttonXRatio);
    button->rect.y = (int)(window->height * buttonYRatio);

    // Initialisation du texte du bouton
    button->text = strdup(buttonText);

    return button;
}

void destroyButton(button_t **button)
{
    SDL_DestroyTexture((*button)->texture);
    SDL_FreeSurface((*button)->surface);
    free((*button)->text);
    free(*button);

    *button = NULL;
}

textbox_t *createTextbox(TTF_Font *font, SDL_Color color, SDL_Rect rect, window_t *window)
{
    textbox_t *textbox = malloc(sizeof(textbox_t));

    textbox->surface = TTF_RenderText_Solid(font, "", color);
    textbox->texture = SDL_CreateTextureFromSurface(window->renderer, textbox->surface);
    memcpy(&(textbox->rect), &rect, sizeof(SDL_Rect));
    SDL_SetTextInputRect(&(textbox->rect));
    textbox->text = malloc(1024);
    textbox->text[0] = 0;

    return textbox;
}

void updateTextboxText(SDL_Event event, TTF_Font *font, char *inputText, int *width, int *height)
{
    switch (event.type)
    {
    case SDL_TEXTINPUT:
        strcat(inputText, event.text.text);
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0)
        {
            inputText[strlen(inputText) - 1] = '\0';
        }
        break;
    default:
        break;
    }
    TTF_SizeUTF8(font, inputText, width, height);
}

void destroyTextbox(textbox_t **textbox)
{
    SDL_DestroyTexture((*textbox)->texture);
    SDL_FreeSurface((*textbox)->surface);
    free((*textbox)->text);
    free(*textbox);

    *textbox = NULL;
}

void drawRect(SDL_Renderer *renderer, SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}

void drawClear(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Couleur noire
    SDL_RenderClear(renderer);
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, char *text, SDL_Rect rect, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

int isMouseClickInRect(SDL_Event event, SDL_Rect rect, int button, int type)
{

    if (type == event.type && event.button.button == button)
    {
        if (event.button.x >= rect.x && event.button.x <= rect.x + rect.w && event.button.y >= rect.y && event.button.y <= rect.y + rect.h)
        {
            return 1;
        }
    }
    return 0;
}

SDL_Point getMousePosition()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    // printf("Mouse position : (%d, %d)\n", mouseX, mouseY);
    return (SDL_Point){mouseX, mouseY};
}