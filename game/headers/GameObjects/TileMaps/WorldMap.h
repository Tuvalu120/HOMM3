/**
 * @file WorldMap.h
 * @author Bartosz Han
 * @brief This file contains WorldMap class, which is a type of TileMap used for game's main exploration map.
 *
 */

#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <vector>

#include "GameObjects/TileMaps/TileMap.hpp"
#include "GameObjects/TileMaps/WorldTile.hpp"

class WorldMap: public TileMap<WorldTile>{

private:
    void addTileToMap(const char tile_type, const int x_pos, const int y_pos) override;
    std::vector<std::pair<int,int>> town_possible_cords_;


public:

    /**
    * @class WorldMap
    * @brief This class is a version of TileMap, which is used as game's main exploration map.
    *
    * This map is where armies can iteract with each other and other objects, and where armies can enter towns.
    *
    * This class also contains few method related to the world-related behaviours of game mechanics.
    *
    * This class uses WorldTile as tiles.
    *
    */
    WorldMap();
    void initializeMap() override;

    /**
     * @brief Get vector of coordinates, with every possible position, on which town can be spawned.
     * Town possible positions are marked as T in map data text file.
     *
     * @return * const std::vector<std::pair<int,int>>& Vector of coordinates, with every possible position, on which town can be spawned.
     */
    const std::vector<std::pair<int,int>>& getTownPossibleCoords() const;

    /**
     * @brief Count, how many objects of given type are currently present on world map.
     *
     * @param type ObjectType of objects to the the number of.
     *
     * @return Number of objects on world map with provided ObjectType value.
     */
    int countNumberOfObjectOfType(const ObjectType type) const;
};

#endif