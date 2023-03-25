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

extern button_t *createButton(window_t *window, TTF_Font *font, char *text, SDL_Color color, SDL_Color selected_color)
{
    button_t *button = malloc(sizeof(button_t));

    button->sprite = createTextSprite(window, font, text, color);
    button->font = font;
    button->color = color;
    button->is_selected = 0;
    button->selected_color = selected_color;
    button->rect.w = button->sprite->width;
    button->rect.h = button->sprite->height;
    button->rect.x = 0;
    button->rect.y = 0;
    button->text = strdup(text);

    return button;
}

extern void renderButton(window_t *window, button_t *button)
{
    destroySprite(&button->sprite);
    button->sprite = createTextSprite(window, button->font, button->text, button->is_selected ? button->selected_color : button->color);

    SDL_RenderCopy(window->renderer, button->sprite->texture, NULL, &button->rect);
}

extern void destroyButton(button_t **button)
{
    destroySprite(&(*button)->sprite);
    free((*button)->text);
    free(*button);

    *button = NULL;
}

extern textbox_t *createTextbox(window_t *window, TTF_Font *font, char *text, SDL_Color color, int character_limit)
{
    textbox_t *textbox = malloc(sizeof(textbox_t));

    textbox->sprite = text[0] != 0 ? createTextSprite(window, font, text, color) : NULL;
    textbox->font = font;
    textbox->color = color;
    textbox->character_limit = character_limit;
    textbox->character_diplay_count = strlen(text);
    TTF_SizeUTF8(font, text, &textbox->rect.w, &textbox->rect.h);
    textbox->rect.x = 0;
    textbox->rect.y = 0;
    TTF_SizeUTF8(font, text, &textbox->text_rect.w, &textbox->text_rect.h);
    textbox->text_rect.x = 0;
    textbox->text_rect.y = 0;
    textbox->max_width = textbox->rect.w;
    textbox->text = malloc(character_limit);
    strcpy(textbox->text, text);

    return textbox;
}

extern void renderTextbox(window_t *window, textbox_t *textbox)
{
    destroySprite(&textbox->sprite);

    TTF_MeasureUTF8(textbox->font, textbox->text, textbox->max_width, &textbox->text_rect.w, &textbox->character_diplay_count);

    if (textbox->text[0] != 0)
    {
        char *temp_text = malloc(textbox->character_diplay_count + 2);
        int offset = strlen(textbox->text) - textbox->character_diplay_count - 1;

        if (offset < 0)
            offset = 0;

        strcpy(temp_text, textbox->text + offset);

        textbox->sprite = createTextSprite(window, textbox->font, temp_text, textbox->color);

        free(temp_text);
    }
    else
        textbox->sprite = NULL;

    if (textbox->sprite != NULL)
        SDL_RenderCopy(window->renderer, textbox->sprite->texture, NULL, &textbox->rect);
}

extern void updateTextbox(SDL_Event *event, textbox_t *textbox)
{
    int length = strlen(textbox->text);

    switch (event->type)
    {
    case SDL_TEXTINPUT:
        if (length < textbox->character_limit - 1)
            strcat(textbox->text, event->text.text);
        break;
    case SDL_KEYDOWN:
        if (event->key.keysym.sym == SDLK_BACKSPACE && length > 0)
            textbox->text[length - 1] = '\0';
        break;
    default:
        break;
    }

    TTF_MeasureUTF8(textbox->font, textbox->text, textbox->max_width, &textbox->text_rect.w, &textbox->character_diplay_count);
}

extern void destroyTextbox(textbox_t **textbox)
{
    destroySprite(&(*textbox)->sprite);
    free((*textbox)->text);
    free(*textbox);

    *textbox = NULL;
}

extern void drawRect(SDL_Renderer *renderer, SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}

extern int isMouseClickInRect(SDL_Event event, SDL_Rect rect, int button, int type)
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

extern SDL_Point getMousePosition()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return (SDL_Point){mouseX, mouseY};
}