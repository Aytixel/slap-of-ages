/**
 * @file window.c
 * @author Lucas Dureau
 * @brief Permets de gérer la fenêtre
 * @version 1.1
 * @date 08/02/2023
 *
 */

#include "window.h"

extern window_t *createWindow(char *title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "(Erreur): Initialisation d'SDL2 impossible : %s\n", SDL_GetError());

        return NULL;
    }

    if (TTF_Init() < 0)
    {
        fprintf(stderr, "(Erreur): Initialisation d'SDL2_ttf impossible : %s\n", TTF_GetError());

        SDL_Quit();

        return NULL;
    }

    if (Mix_Init(MIX_INIT_MP3) < 0 || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
    {
        fprintf(stderr, "(Erreur): Initialisation d'SDL2_mixer impossible : %s\n", SDL_GetError());

        return NULL;
    }

    window_t *window = malloc(sizeof(window_t));

    if ((window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) == NULL)
    {
        fprintf(stderr, "(Erreur): Création de la fenêtre impossible : %s\n", SDL_GetError());

        TTF_Quit();
        SDL_Quit();
        free(window);

        return NULL;
    }

    if ((window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED)) == NULL)
    {
        fprintf(stderr, "(Erreur): Création du renderer impossible : %s\n", SDL_GetError());

        SDL_DestroyWindow(window->window);
        TTF_Quit();
        SDL_Quit();
        free(window);

        return NULL;
    }

    window->width = width;
    window->height = height;
    window->original_width = width;
    window->original_height = height;
    window->width_scale_factor = 1;
    window->height_scale_factor = 1;

    SDL_SetWindowMinimumSize(window->window, width, height);
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
    SDL_RenderClear(window->renderer);
    SDL_RenderPresent(window->renderer);

    return window;
}

extern void updateWindowSize(window_t *window, SDL_Event *event)
{
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        window->width = event->window.data1;
        window->height = event->window.data2;
        window->width_scale_factor = (float)window->width / (float)window->original_width;
        window->height_scale_factor = (float)window->height / (float)window->original_height;
    }
}

extern int destroyWindow(window_t **window)
{
    if (window == NULL || *window == NULL)
        return -1;

    SDL_DestroyRenderer((*window)->renderer);
    SDL_DestroyWindow((*window)->window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    free(*window);
    *window = NULL;

    return 0;
}

extern TTF_Font *loadFont(char *path, int pt_size)
{
    TTF_Font *font = TTF_OpenFont(path, pt_size);

    if (font == NULL)
    {
        fprintf(stderr, "(Erreur): Impossible de charger la police \"%s\" : %s\n", path, TTF_GetError());

        return NULL;
    }

    return font;
}

extern sprite_t *loadSprite(window_t *window, char *path)
{
    sprite_t *sprite = malloc(sizeof(sprite_t));
    SDL_Surface *surface = NULL;

    if ((surface = IMG_Load(path)) == NULL)
    {
        fprintf(stderr, "(Erreur): Impossible de charger le sprite \"%s\" : %s\n", path, SDL_GetError());

        free(sprite);

        return NULL;
    }

    sprite->width = surface->w;
    sprite->height = surface->h;

    if ((sprite->texture = SDL_CreateTextureFromSurface(window->renderer, surface)) == NULL)
    {
        fprintf(stderr, "(Erreur): Création de la texture pour le sprite \"%s\" impossible : %s\n", path, SDL_GetError());

        SDL_FreeSurface(surface);
        free(sprite);

        return NULL;
    }

    SDL_FreeSurface(surface);

    return sprite;
}

extern sprite_t *createTextSprite(window_t *window, TTF_Font *font, char *text, SDL_Color color)
{
    sprite_t *sprite = malloc(sizeof(sprite_t));
    SDL_Surface *surface = NULL;

    if ((surface = TTF_RenderText_Solid(font, text, color)) == NULL)
    {
        fprintf(stderr, "(Erreur): Impossible de créer le sprite du texte \"%s\" : %s\n", text, TTF_GetError());

        free(sprite);

        return NULL;
    }

    sprite->width = surface->w;
    sprite->height = surface->h;

    if ((sprite->texture = SDL_CreateTextureFromSurface(window->renderer, surface)) == NULL)
    {
        fprintf(stderr, "(Erreur): Création de la texture pour le sprite du texte \"%s\" impossible : %s\n", text, SDL_GetError());

        SDL_FreeSurface(surface);
        free(sprite);

        return NULL;
    }

    SDL_FreeSurface(surface);

    return sprite;
}

extern int destroySprite(sprite_t **sprite)
{
    if (sprite == NULL || *sprite == NULL)
        return -1;

    SDL_DestroyTexture((*sprite)->texture);
    free(*sprite);
    *sprite = NULL;

    return 0;
}

extern Mix_Music *loadMusic(char *path)
{
    Mix_Music *audio = Mix_LoadMUS(path);

    if (audio == NULL)
    {
        fprintf(stderr, "(Erreur): Impossible de charger l'audio \"%s\" : %s\n", path, SDL_GetError());

        return NULL;
    }

    return audio;
}

extern SDL_Rect positionFromCenter(window_t *window, int width, int height, int x, int y, transform_origin_e origin)
{
    int offset_x = 0, offset_y = 0;

    switch (origin)
    {
    case TRANSFORM_ORIGIN_TOP_LEFT:
        break;
    case TRANSFORM_ORIGIN_TOP_RIGHT:
        offset_x = -width;
        break;
    case TRANSFORM_ORIGIN_BOTTOM_RIGHT:
        offset_x = -width;
    case TRANSFORM_ORIGIN_BOTTOM_LEFT:
        offset_y = -height;
        break;
    case TRANSFORM_ORIGIN_BOTTOM:
        offset_y = -height;
    case TRANSFORM_ORIGIN_TOP:
        offset_x = -width / 2;
        break;
    case TRANSFORM_ORIGIN_RIGHT:
        offset_x = -width;
    case TRANSFORM_ORIGIN_LEFT:
        offset_y = -height / 2;
        break;
    case TRANSFORM_ORIGIN_CENTER:
    default:
        offset_x = -width / 2;
        offset_y = -height / 2;
        break;
    }

    SDL_Rect position = {window->width / 2 + offset_x + x, window->height / 2 + offset_y + y, width, height};

    return position;
}

extern SDL_Rect positionToCenter(window_t *window, int width, int height)
{
    return positionFromCenter(window, width, height, 0, 0, TRANSFORM_ORIGIN_CENTER);
}