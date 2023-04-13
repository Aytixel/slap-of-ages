/**
 * @file building_hud.c
 * @author Lucas Dureau
 * @brief Permet de gérer l'affichage de l'interface de sélection des bâtiments
 * @version 1.1
 * @date 30/03/2023
 *
 */

#include "building_renderer.h"
#include "building_hud.h"

extern building_hud_t *createBuildingHud(window_t *window)
{
    building_hud_t *building_hud = malloc(sizeof(building_hud_t));

    building_hud->text_font = loadFont("asset/font/8-BIT-WONDER.ttf", 15);

    if (building_hud->text_font == NULL)
    {
        free(building_hud);

        return NULL;
    }

    SDL_Color text_color = {255, 255, 255, 0};
    SDL_Color selected_text_color = {52, 36, 20, 0};

    char buffer[5] = {0};

    SDL_itoa(getBuildingGoldCost(HOUSE_1_BUILDING), buffer, 10);
    building_hud->house_1_button = createButton(window, building_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getBuildingGoldCost(HOUSE_2_BUILDING), buffer, 10);
    building_hud->house_2_button = createButton(window, building_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getBuildingGoldCost(HOUSE_3_BUILDING), buffer, 10);
    building_hud->house_3_button = createButton(window, building_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getBuildingGoldCost(MILL_BUILDING), buffer, 10);
    building_hud->mill_button = createButton(window, building_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getBuildingGoldCost(MINE_BUILDING), buffer, 10);
    building_hud->mine_button = createButton(window, building_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getBuildingGoldCost(FIELD_BUILDING), buffer, 10);
    building_hud->field_button = createButton(window, building_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getBuildingGoldCost(WELL_BUILDING), buffer, 10);
    building_hud->well_button = createButton(window, building_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getBuildingGoldCost(CORNER_WALL_BUILDING), buffer, 10);
    building_hud->corner_wall_button = createButton(window, building_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getBuildingGoldCost(VERTICAL_WALL_BUILDING), buffer, 10);
    building_hud->vertical_wall_button = createButton(window, building_hud->text_font, buffer, text_color, selected_text_color);
    SDL_itoa(getBuildingGoldCost(HORIZONTAL_WALL_BUILDING), buffer, 10);
    building_hud->horizontal_wall_button = createButton(window, building_hud->text_font, buffer, text_color, selected_text_color);

    memset(&building_hud->rects, 0, sizeof(building_sprite_rects_t));

    return building_hud;
}

extern void buildingHudEventHandler(SDL_Event *event, building_hud_t *building_hud, client_game_data_t *game_data)
{
    if (isMouseClickInRect(*event, building_hud->rects.house_1, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, building_hud->house_1_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_building_type = HOUSE_1_BUILDING;
    if (isMouseClickInRect(*event, building_hud->rects.house_2, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, building_hud->house_2_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_building_type = HOUSE_2_BUILDING;
    if (isMouseClickInRect(*event, building_hud->rects.house_3, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, building_hud->house_3_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_building_type = HOUSE_3_BUILDING;
    if (isMouseClickInRect(*event, building_hud->rects.mill, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, building_hud->mill_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_building_type = MILL_BUILDING;
    if (isMouseClickInRect(*event, building_hud->rects.mine, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, building_hud->mine_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_building_type = MINE_BUILDING;
    if (isMouseClickInRect(*event, building_hud->rects.field, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, building_hud->field_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_building_type = FIELD_BUILDING;
    if (isMouseClickInRect(*event, building_hud->rects.well, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, building_hud->well_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_building_type = WELL_BUILDING;
    if (isMouseClickInRect(*event, building_hud->rects.corner_wall, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, building_hud->corner_wall_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_building_type = CORNER_WALL_BUILDING;
    if (isMouseClickInRect(*event, building_hud->rects.vertical_wall, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, building_hud->vertical_wall_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_building_type = VERTICAL_WALL_BUILDING;
    if (isMouseClickInRect(*event, building_hud->rects.horizontal_wall, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN) || isMouseClickInRect(*event, building_hud->horizontal_wall_button->rect, SDL_BUTTON_LEFT, SDL_MOUSEBUTTONDOWN))
        game_data->selected_building_type = HORIZONTAL_WALL_BUILDING;

    SDL_Point mouse_point = getMousePosition();

    building_hud->house_1_button->is_selected = SDL_PointInRect(&mouse_point, &building_hud->rects.house_1) || SDL_PointInRect(&mouse_point, &building_hud->house_1_button->rect) || game_data->selected_building_type == HOUSE_1_BUILDING;
    building_hud->house_2_button->is_selected = SDL_PointInRect(&mouse_point, &building_hud->rects.house_2) || SDL_PointInRect(&mouse_point, &building_hud->house_2_button->rect) || game_data->selected_building_type == HOUSE_2_BUILDING;
    building_hud->house_3_button->is_selected = SDL_PointInRect(&mouse_point, &building_hud->rects.house_3) || SDL_PointInRect(&mouse_point, &building_hud->house_3_button->rect) || game_data->selected_building_type == HOUSE_3_BUILDING;
    building_hud->mill_button->is_selected = SDL_PointInRect(&mouse_point, &building_hud->rects.mill) || SDL_PointInRect(&mouse_point, &building_hud->mill_button->rect) || game_data->selected_building_type == MILL_BUILDING;
    building_hud->mine_button->is_selected = SDL_PointInRect(&mouse_point, &building_hud->rects.mine) || SDL_PointInRect(&mouse_point, &building_hud->mine_button->rect) || game_data->selected_building_type == MINE_BUILDING;
    building_hud->field_button->is_selected = SDL_PointInRect(&mouse_point, &building_hud->rects.field) || SDL_PointInRect(&mouse_point, &building_hud->field_button->rect) || game_data->selected_building_type == FIELD_BUILDING;
    building_hud->well_button->is_selected = SDL_PointInRect(&mouse_point, &building_hud->rects.well) || SDL_PointInRect(&mouse_point, &building_hud->well_button->rect) || game_data->selected_building_type == WELL_BUILDING;
    building_hud->corner_wall_button->is_selected = SDL_PointInRect(&mouse_point, &building_hud->rects.corner_wall) || SDL_PointInRect(&mouse_point, &building_hud->corner_wall_button->rect) || game_data->selected_building_type == CORNER_WALL_BUILDING;
    building_hud->vertical_wall_button->is_selected = SDL_PointInRect(&mouse_point, &building_hud->rects.vertical_wall) || SDL_PointInRect(&mouse_point, &building_hud->vertical_wall_button->rect) || game_data->selected_building_type == VERTICAL_WALL_BUILDING;
    building_hud->horizontal_wall_button->is_selected = SDL_PointInRect(&mouse_point, &building_hud->rects.horizontal_wall) || SDL_PointInRect(&mouse_point, &building_hud->horizontal_wall_button->rect) || game_data->selected_building_type == HORIZONTAL_WALL_BUILDING;
}

extern void renderBuildingHud(window_t *window, building_hud_t *building_hud, building_renderer_t *building_renderer)
{
    float scale_factor = window->height_scale_factor;
    int x_offset = -96 * scale_factor;

    building_hud->rects.house_1 = positionFromCenter(
        window,
        building_renderer->sprite_rects.house_1.w * 1.5 * scale_factor,
        building_renderer->sprite_rects.house_1.h * 1.5 * scale_factor,
        -building_renderer->map_renderer->offset_from_center + x_offset,
        building_renderer->map_renderer->offset_from_center + 80 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);
    building_hud->house_1_button->rect = positionFromCenter(
        window,
        building_hud->house_1_button->sprite->width * scale_factor,
        building_hud->house_1_button->sprite->height * scale_factor,
        -building_renderer->map_renderer->offset_from_center + x_offset,
        building_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        &building_renderer->sprite_rects.house_1,
        &building_hud->rects.house_1);
    renderButton(window, building_hud->house_1_button);

    building_hud->rects.house_2 = positionFromCenter(
        window,
        building_renderer->sprite_rects.house_2.w * 1.5 * scale_factor,
        building_renderer->sprite_rects.house_2.h * 1.5 * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 64 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 80 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);
    building_hud->house_2_button->rect = positionFromCenter(
        window,
        building_hud->house_2_button->sprite->width * scale_factor,
        building_hud->house_2_button->sprite->height * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 64 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        &building_renderer->sprite_rects.house_2,
        &building_hud->rects.house_2);
    renderButton(window, building_hud->house_2_button);

    building_hud->rects.house_3 = positionFromCenter(
        window,
        building_renderer->sprite_rects.house_3.w * 1.5 * scale_factor,
        building_renderer->sprite_rects.house_3.h * 1.5 * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 128 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 80 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);
    building_hud->house_3_button->rect = positionFromCenter(
        window,
        building_hud->house_3_button->sprite->width * scale_factor,
        building_hud->house_3_button->sprite->height * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 128 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        &building_renderer->sprite_rects.house_3,
        &building_hud->rects.house_3);
    renderButton(window, building_hud->house_3_button);

    building_hud->rects.mill = positionFromCenter(
        window,
        building_renderer->sprite_rects.mill.w * 1.5 * scale_factor,
        building_renderer->sprite_rects.mill.h * 1.5 * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 192 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 80 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);
    building_hud->mill_button->rect = positionFromCenter(
        window,
        building_hud->mill_button->sprite->width * scale_factor,
        building_hud->mill_button->sprite->height * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 192 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        &building_renderer->sprite_rects.mill,
        &building_hud->rects.mill);
    renderButton(window, building_hud->mill_button);

    building_hud->rects.mine = positionFromCenter(
        window,
        building_renderer->sprite_rects.mine.w * 1.5 * scale_factor,
        building_renderer->sprite_rects.mine.h * 1.5 * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 256 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 80 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);
    building_hud->mine_button->rect = positionFromCenter(
        window,
        building_hud->mine_button->sprite->width * scale_factor,
        building_hud->mine_button->sprite->height * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 256 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        &building_renderer->sprite_rects.mine,
        &building_hud->rects.mine);
    renderButton(window, building_hud->mine_button);

    building_hud->rects.field = positionFromCenter(
        window,
        building_renderer->sprite_rects.field.w * 1.5 * scale_factor,
        building_renderer->sprite_rects.field.h * 1.5 * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 320 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 80 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);
    building_hud->field_button->rect = positionFromCenter(
        window,
        building_hud->field_button->sprite->width * scale_factor,
        building_hud->field_button->sprite->height * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 320 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        &building_renderer->sprite_rects.field,
        &building_hud->rects.field);
    renderButton(window, building_hud->field_button);

    building_hud->rects.well = positionFromCenter(
        window,
        building_renderer->sprite_rects.well.w * 1.5 * scale_factor,
        building_renderer->sprite_rects.well.h * 1.5 * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 384 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 80 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);
    building_hud->well_button->rect = positionFromCenter(
        window,
        building_hud->well_button->sprite->width * scale_factor,
        building_hud->well_button->sprite->height * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 384 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        &building_renderer->sprite_rects.well,
        &building_hud->rects.well);
    renderButton(window, building_hud->well_button);

    building_hud->rects.corner_wall = positionFromCenter(
        window,
        building_renderer->sprite_rects.corner_wall.w * 1.5 * scale_factor,
        building_renderer->sprite_rects.corner_wall.h * 1.5 * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 448 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 80 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);
    building_hud->corner_wall_button->rect = positionFromCenter(
        window,
        building_hud->corner_wall_button->sprite->width * scale_factor,
        building_hud->corner_wall_button->sprite->height * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 448 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        &building_renderer->sprite_rects.corner_wall,
        &building_hud->rects.corner_wall);
    renderButton(window, building_hud->corner_wall_button);

    building_hud->rects.vertical_wall = positionFromCenter(
        window,
        building_renderer->sprite_rects.vertical_wall.w * 1.5 * scale_factor,
        building_renderer->sprite_rects.vertical_wall.h * 1.5 * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 512 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 80 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);
    building_hud->vertical_wall_button->rect = positionFromCenter(
        window,
        building_hud->vertical_wall_button->sprite->width * scale_factor,
        building_hud->vertical_wall_button->sprite->height * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 512 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        &building_renderer->sprite_rects.vertical_wall,
        &building_hud->rects.vertical_wall);
    renderButton(window, building_hud->vertical_wall_button);

    building_hud->rects.horizontal_wall = positionFromCenter(
        window,
        building_renderer->sprite_rects.horizontal_wall.w * 1.5 * scale_factor,
        building_renderer->sprite_rects.horizontal_wall.h * 1.5 * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 576 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 80 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);
    building_hud->horizontal_wall_button->rect = positionFromCenter(
        window,
        building_hud->horizontal_wall_button->sprite->width * scale_factor,
        building_hud->horizontal_wall_button->sprite->height * scale_factor,
        -building_renderer->map_renderer->offset_from_center + 576 * scale_factor + x_offset,
        building_renderer->map_renderer->offset_from_center + 96 * scale_factor,
        TRANSFORM_ORIGIN_BOTTOM);

    SDL_RenderCopy(
        window->renderer,
        building_renderer->sprite->texture,
        &building_renderer->sprite_rects.horizontal_wall,
        &building_hud->rects.horizontal_wall);
    renderButton(window, building_hud->horizontal_wall_button);
}

extern int deleteBuildingHud(building_hud_t **building_hud)
{
    if (building_hud == NULL || *building_hud == NULL)
        return -1;

    TTF_CloseFont((*building_hud)->text_font);
    destroyButton(&(*building_hud)->house_1_button);
    destroyButton(&(*building_hud)->house_2_button);
    destroyButton(&(*building_hud)->house_3_button);
    destroyButton(&(*building_hud)->mill_button);
    destroyButton(&(*building_hud)->mine_button);
    destroyButton(&(*building_hud)->field_button);
    destroyButton(&(*building_hud)->well_button);
    destroyButton(&(*building_hud)->corner_wall_button);
    destroyButton(&(*building_hud)->vertical_wall_button);
    destroyButton(&(*building_hud)->horizontal_wall_button);

    free(*building_hud);
    *building_hud = NULL;

    return 0;
}