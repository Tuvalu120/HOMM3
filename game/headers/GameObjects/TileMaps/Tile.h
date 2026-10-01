/**
 * @file Tile.h
 * @author Bartosz Han
 * @brief This file contains Tile class declaration, which is a class, whose objects are making up the TileMap.
 */

#ifndef TILE_H
#define TILE_H

#include <memory>

#include "GameObjects/BaseObjects/DrawableObject.h"
#include "GameObjects/BaseObjects/InteractiveObject.h"
#include "Data/ObjectDataEnums.hpp"

/**
 * @class Tile
 * @brief This class is used by the objects, which make up TileMap.
 * Each tile represents one tile on the tile map.
 *
 * This contains various data related to it state as well the tile map it is associated with.
 * This class also contains pointer to one InteractiveObject, as contains various methods related
 * to managing and controlling it.
 */
class Tile: public DrawableObject{
private:
    int x_pos_;
    int y_pos_;
    TileType tile_type_;
    std::shared_ptr<InteractiveObject> object_on_tile_;
    std::string additional_texture_name_; //Used for additional elements of environments
    int move_cost_;
    bool is_wall_;

    /**
     * @brief Generate file path to the texture associated with the provided tile type.
     *
     * This method will randomly select one texture from all textures available for given tile type.
     *
     * @param tile_type Tile tile of which the texture tile path wil be generated.
     * @return std::string File path to texture associated to given tile path.
     */
    static std::string generateTextureName(TileType tile_type);

public:
    /**
     * @brief Construct a new Tile object.
     *
     * @param x_pos X position of tile on tile map.
     * @param y_pos Y position of tile on tile map.
     * @param tile_type Tile type of this tile.
     * @param move_cost Integer indicating how many move points will be deducted from MOvableObject when it will pass this tile.
     * @param is_wall Flag indicating whether or not this tile is walkable.
     * @param additional_texture File path to an additional texture to be displayed on the tile. This texture is independent from DrawableObject, and
     * texture of object on tile.
     */
    Tile(const int x_pos, const int y_pos, const TileType tile_type, const int move_cost = 1,
        const bool is_wall = false, const std::string additional_texture = "");

    /// @brief Get x position of tile on tile map.
    int getXPos() const;

    /// @brief Get y position of tile on tile map.
    int getYPos() const;

    /// @brief Get position of tile as a pair of coordinates.
    std::pair<int, int> getPos() const;

    /// @brief Get TileType of this object.
    TileType getTileType() const;

    /// @brief Set TileType of this object.
    void setTileType(const TileType tile_type);

    /// @brief Get file path of the additional texture used in drawing this tile.
    const std::string getAdditionalTextureName() const;

    /// @brief Returns true this this object has additional texture used in drawing this tile.
    bool hasAdditionalTexture() const;

    /// @brief Get integer indicating how many move points will be deducted from MOvableObject when it will pass this tile.
    int getMoveCost() const;

    /// @brief Returns true if this object has currently interactive object on it.
    bool hasInteractiveObject() const;

    /// @brief Returns true is this object is marked as wall and is non-walkable.
    bool isTileWall() const;

    /// @brief Set the flag indicating that this tile is marked as wall.
    void setFlagIsTileWall(const bool is_wall);

    /// @brief Returns true if this tile can be used as a path when travelling within map.
    virtual bool isTileWalkable() const;

    /**
     * @brief Add new object to the tile.
     *
     * This tile must not have another object already on it.
     *
     * @param new_object_on_tile Shared pointer to the object to add into this file.
     */
    void addObjectOnTile(std::shared_ptr<InteractiveObject> new_object_on_tile);

    /**
     * @brief Move object, which is currently in this tile into another provided tile.
     *
     * As a result, this tile will lose its current object, and provided tile will get new object.
     * To make this method work, this tile must have object on it, and another tile must not have any object on it.
     *
     * @param other_tile
     */
    void moveObjectOnTile(Tile& other_tile);

    /// @brief Destroy the pointer to the object which is on tile.
    void removeObjectOnTile();

    /// @brief Get object, which is currently on this tile.
    InteractiveObject& getObjectOnTile() const;

    /// @brief Returns true, if given tile_type is marked as Border Tile type. Tiles, which have border tile type as wal by default.
    static bool isTileTypeBorderType(TileType tile_type);

    /// @brief Get border tile type associated with provided walkable tile type.
    /// For each walkable tile type, there is a border tile type with similar textures.
    static TileType getBorderTileTypeFromWalkableTileType(const TileType tile_type);

    Tile(const Tile&) = delete;
    Tile& operator=(const Tile&) = delete;
};

#endif