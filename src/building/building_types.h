/**
 * @file building_types.h
 * @author Lucas Dureau
 * @brief Gestion des différents types de bâtiments
 * @version 1.1
 * @date 17/05/2023
 *
 */

#ifndef __BUILDING_TYPES_H
#define __BUILDING_TYPES_H

/**
 * @brief Enumération des différents types de bâtiments
 *
 */
typedef enum building_type_s
{
    HOUSE_1_BUILDING,
    HOUSE_2_BUILDING,
    HOUSE_3_BUILDING,
    MILL_BUILDING,
    MINE_BUILDING,
    FIELD_BUILDING,
    WELL_BUILDING,
    CORNER_WALL_BUILDING,
    VERTICAL_WALL_BUILDING,
    HORIZONTAL_WALL_BUILDING,
    VERTICAL_SPACE_FILLER_WALL_BUILDING,
    HORIZONTAL_SPACE_FILLER_WALL_BUILDING,
} building_type_e;

#endif