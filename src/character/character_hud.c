/**
 * @file character_hud.c
 * @author Lucas Dureau
 * @brief Permet de gérer l'affichage de l'interface de sélection des troupes
 * @version 1.1
 * @date 05/04/2023
 *
 */

#include "character_hud.h"

extern character_hud_t *createCharacterHud(window_t *window, character_renderer_t *character_renderer)
{
    character_hud_t *character_hud = malloc(sizeof(character_hud_t));

    character_hud->text_font = loadFont("asset/font/8-BIT-WONDER.ttf", 15);

    if (character_hud->text_font == NULL)
    {
        free(character_hud);

        return NULL;
    }

    SDL_Color text_color = {255, 255, 255, 0};
    SDL_Color selected_text_color = {52, 36, 20, 0};

    character_hud->character_renderer = character_renderer;

    character_hud->rat_animation = createAnimation(
        character_renderer->animations.rat.tile_size / 2,
        character_renderer->animations.rat.state_frame_count,
        character_renderer->animations.rat.sprite,
        character_renderer->animations.rat.fps);
    character_hud->daemon_animation = createAnimation(
        character_renderer->animations.daemon.tile_size / 2,
        character_renderer->animations.daemon.state_frame_count,
        character_renderer->animations.daemon.sprite,
        character_renderer->animations.daemon.fps);
    character_hud->giant_animation = createAnimation(
        character_renderer->animations.giant.tile_size / 2,
        character_renderer->animations.giant.state_frame_count,
        character_renderer->animations.giant.sprite,
        character_renderer->animations.giant.fps);

    changeAnimationState(character_hud->rat_animation, RAT_IDLE_ANIM);
    changeAnimationState(character_hud->daemon_animation, DAEMON_IDLE_ANIM);
    changeAnimationState(character_hud->giant_animation, GOBLIN_GIANT_IDLE_ANIM);

    char buffer[5] = {0};

    SDL_itoa(getCharacterElixirCost(RAT_CHARACTER), buffer, 10);
    character_hud->rat_button = createButton(window, character_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getCharacterElixirCost(DAEMON_CHARACTER), buffer, 10);
    character_hud->daemon_button = createButton(window, character_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getCharacterElixirCost(GIANT_CHARACTER), buffer, 10);
    character_hud->giant_button = createButton(window, character_hud->text_font, buffer, text_color, selected_text_color);

    return character_hud;
}

extern void characterHudEventHandler(SDL_Event *event, character_hud_t *character_hud, client_game_data_t *game_data)
{
    if (isMouseClickInRect(*event, character_hud->rat_animation->frame_rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, character_hud->rat_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_character_type = RAT_CHARACTER;
    if (isMouseClickInRect(*event, character_hud->daemon_animation->frame_rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, character_hud->daemon_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_character_type = DAEMON_CHARACTER;
    if (isMouseClickInRect(*event, character_hud->giant_animation->frame_rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, character_hud->giant_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_character_type = GIANT_CHARACTER;

    SDL_Point mouse_point = getMousePosition();

    character_hud->rat_button->is_selected = SDL_PointInRect(&mouse_point, &character_hud->rat_animation->frame_rect) || SDL_PointInRect(&mouse_point, &character_hud->rat_button->rect) || game_data->selected_character_type == RAT_CHARACTER;
    character_hud->daemon_button->is_selected = SDL_PointInRect(&mouse_point, &character_hud->daemon_animation->frame_rect) || SDL_PointInRect(&mouse_point, &character_hud->daemon_button->rect) || game_data->selected_character_type == DAEMON_CHARACTER;
    character_hud->giant_button->is_selected = SDL_PointInRect(&mouse_point, &character_hud->giant_animation->frame_rect) || SDL_PointInRect(&mouse_point, &character_hud->giant_button->rect) || game_data->selected_character_type == GIANT_CHARACTER;
}

extern void renderCharacterHud(window_t *window, character_hud_t *character_hud)
{
    float scale_factor = window->height_scale_factor;
    int x_offset = 64 * scale_factor;

    character_hud->rat_button->rect = positionFromCenter(
        window,
        character_hud->rat_button->sprite->width * scale_factor,
        character_hud->rat_button->sprite->height * scale_factor,
        -character_hud->character_renderer->map_renderer->offset_from_center + x_offset,
        character_hud->character_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_Point rat_animation_position = {
        -character_hud->character_renderer->map_renderer->offset_from_center + x_offset,
        character_hud->character_renderer->map_renderer->offset_from_center + 80 * scale_factor,
    };

    updateAnimation(character_hud->rat_animation, character_hud->character_renderer->animations.rat.tile_size / 2 * scale_factor, &rat_animation_position, window, TRANSFORM_ORIGIN_BOTTOM);
    renderButton(window, character_hud->rat_button);

    character_hud->daemon_button->rect = positionFromCenter(
        window,
        character_hud->daemon_button->sprite->width * scale_factor,
        character_hud->daemon_button->sprite->height * scale_factor,
        -character_hud->character_renderer->map_renderer->offset_from_center + 128 * scale_factor + x_offset,
        character_hud->character_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_Point daemon_animation_position = {
        -character_hud->character_renderer->map_renderer->offset_from_center + 128 * scale_factor + x_offset,
        character_hud->character_renderer->map_renderer->offset_from_center + 80 * scale_factor,
    };

    updateAnimation(character_hud->daemon_animation, character_hud->character_renderer->animations.daemon.tile_size / 2 * scale_factor, &daemon_animation_position, window, TRANSFORM_ORIGIN_BOTTOM);
    renderButton(window, character_hud->daemon_button);

    character_hud->giant_button->rect = positionFromCenter(
        window,
        character_hud->giant_button->sprite->width * scale_factor,
        character_hud->giant_button->sprite->height * scale_factor,
        -character_hud->character_renderer->map_renderer->offset_from_center + 256 * scale_factor + x_offset,
        character_hud->character_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_Point giant_animation_position = {
        -character_hud->character_renderer->map_renderer->offset_from_center + 256 * scale_factor + x_offset,
        character_hud->character_renderer->map_renderer->offset_from_center + 80 * scale_factor,
    };

    updateAnimation(character_hud->giant_animation, character_hud->character_renderer->animations.giant.tile_size / 2 * scale_factor, &giant_animation_position, window, TRANSFORM_ORIGIN_BOTTOM);
    renderButton(window, character_hud->giant_button);
}

extern int deleteCharacterHud(character_hud_t **character_hud)
{
    if (character_hud == NULL || *character_hud == NULL)
        return -1;

    TTF_CloseFont((*character_hud)->text_font);
    destroyButton(&(*character_hud)->rat_button);
    destroyButton(&(*character_hud)->daemon_button);
    destroyButton(&(*character_hud)->giant_button);
    destroyAnimationWithoutSprite(&(*character_hud)->rat_animation);
    destroyAnimationWithoutSprite(&(*character_hud)->daemon_animation);
    destroyAnimationWithoutSprite(&(*character_hud)->giant_animation);

    free(*character_hud);
    *character_hud = NULL;

    return 0;
}