/**
 * @file BattlefieldTile.hpp
 * @author Bartosz Han
 * @brief This file contains BattlefieldTile class, which is used as tiles in Battlefield tile map.
 *
 */

#ifndef BATTLEFIELD_TILE_H
#define BATTLEFIELD_TILE_H

#include "GameObjects/TileMaps/Tile.h"

/**
 * @brief This class is used for objects, which are further used as tiles in Battlefield tile map.
 *
 */
class BattlefieldTile: public Tile{

public:
    /**
     * @brief Construct a new BattlefieldTile object
     *
     * @param x_pos X position of tile on tile map.
     * @param y_pos Y position of tile on tile map.
     * @param tile_type Tile type of this tile.
     * @param move_cost Integer indicating how many move points will be deducted from MOvableObject when it will pass this tile.
     * @param is_wall Flag indicating whether or not this tile is walkable.
     * @param additional_texture File path to an additional texture to be displayed on the tile. This texture is independent from DrawableObject, and
     * texture of object on tile.
     */
    BattlefieldTile(const int x_pos, const int y_pos, TileType tile_type,
        const int move_cost = 1, const bool is_wall = false, const std::string additional_texture = ""):
        Tile(x_pos, y_pos, tile_type, move_cost, is_wall, additional_texture){};

};

#endif