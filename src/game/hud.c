/**
 * @file hud.c
 * @author Lucas Dureau
 * @brief Implémente les fonctions pour gérer l'affichage en jeux
 * @version 0.1
 * @date 27/03/2023
 *
 */
#include "hud.h"

extern hud_t *createHud()
{
    hud_t *hud = malloc(sizeof(hud_t));

    hud->text_font = loadFont("asset/font/8-BIT-WONDER.ttf", 15);

    if (hud->text_font == NULL)
    {
        free(hud);

        return NULL;
    }

    return hud;
}

extern void renderHud(window_t *window, hud_t *hud, map_renderer_t *map_renderer, client_game_data_t *game_data)
{
    float scale_factor = window->height_scale_factor;
    SDL_Color text_color = {255, 255, 255, 0};
    char text[50] = "";
    char number_text[50] = "";

    SDL_itoa(game_data->gold_count, text, 10);
    sprite_t *gold_count_sprite = createTextSprite(window, hud->text_font, strcat(text, " or"), text_color);
    SDL_Rect gold_count_rect = positionFromCenter(
        window,
        gold_count_sprite->width * scale_factor,
        gold_count_sprite->height * scale_factor,
        map_renderer->offset_from_center + 32 * scale_factor,
        -map_renderer->offset_from_center - 64 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM_RIGHT);

    SDL_RenderCopy(window->renderer, gold_count_sprite->texture, NULL, &gold_count_rect);
    destroySprite(&gold_count_sprite);

    strcpy(text, "victoire ");
    SDL_itoa(game_data->win_count, number_text, 10);
    sprite_t *win_count_sprite = createTextSprite(window, hud->text_font, strcat(text, number_text), text_color);
    SDL_Rect win_count_rect = positionFromCenter(
        window,
        win_count_sprite->width * scale_factor,
        win_count_sprite->height * scale_factor,
        -map_renderer->offset_from_center - 32 * scale_factor,
        -map_renderer->offset_from_center - 64 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM_LEFT);

    SDL_RenderCopy(window->renderer, win_count_sprite->texture, NULL, &win_count_rect);
    destroySprite(&win_count_sprite);

    switch (game_data->state)
    {
    case PREPARATION_GAME_STATE:
        // affichage du bouton "Attaquer" pour aller en file d'attente
        break;
    case MATCHMAKING_GAME_STATE:
        // affichage du bouton "Annuler" pour sortir de la file d'attente
        break;
    case COMBAT_GAME_STATE:
        if (game_data->timer != NULL)
        {
            strcpy(text, "temps ");
            SDL_itoa(timeLeft(game_data->timer) / 1000, number_text, 10);
            sprite_t *timer_count_sprite = createTextSprite(window, hud->text_font, strcat(text, number_text), text_color);
            SDL_Rect timer_count_rect = positionFromCenter(
                window,
                timer_count_sprite->width * scale_factor,
                timer_count_sprite->height * scale_factor,
                -map_renderer->offset_from_center - 32 * scale_factor,
                -map_renderer->offset_from_center - 32 * scale_factor,
                TRANSFORM_ORIGIN_BOTTOM_LEFT);

            SDL_RenderCopy(window->renderer, timer_count_sprite->texture, NULL, &timer_count_rect);
            destroySprite(&timer_count_sprite);
        }
        break;
    default:
        break;
    }
}

extern int deleteHud(hud_t **hud)
{
    if (hud == NULL || *hud == NULL)
        return -1;

    TTF_CloseFont((*hud)->text_font);

    free(*hud);
    *hud = NULL;

    return 0;
}