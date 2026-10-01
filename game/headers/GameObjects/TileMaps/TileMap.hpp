/**
 * @file TileMap.hpp
 * @author Bartosz Han
 * @brief This file contains TileMap class, which is an abstraction of rectangle grid consisting of tiles.
 *
 */

#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <fstream>
#include <memory>
#include <queue>
#include <vector>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <set>

#include "Data/Constants.hpp"
#include "GameObjects/BaseObjects/MovableObject.h"
#include "Data/ObjectDataEnums.hpp"
#include "Miscellaneous/GameExceptions.hpp"
#include "GameObjects/TileMaps/Tile.h"

/**
 * @class TileMap
 * @brief Template class representing a 2D tile map of tiles for game objects.
 *
 * The TileMap class manages a grid of tiles, providing functionality for map initialization,
 * tile access, pathfinding and object movement. It is designed to be inherited
 * and extended for specific tile types.
 *
 * In order to generate map, there must be file containing data about this tile.
 * The way this file is read is dependent on implementation of addTileToMap() method.
 * TileMap is always a rectangle map.
 *
 * @tparam T The type of tile stored in the map. T must inherits from Tile class.
 */
template <typename T>
class TileMap {

protected:
    std::vector<std::unique_ptr<T>> tile_map_;
    int tile_map_width_, tile_map_height_;

    /**
     * @brief Create a map, by adding tile to it, base on data provided in given file.
     *
     * This method will read the provided text file. File must have such structure that each char is representing
     * on tile on this map and position of char in file represents position of tile at the tile map. This method will
     * read each char in order, and then add new tile for each char.
     *
     * Because of how map is being generated, map data file must must have appropiate structure. Each line of this file must have same number
     * of chars in it, and number of rows and columns in this file must match the previously declared in this object height and width of this tile map.
     *
     * @param file_name File path (std::string) to the tile map data.
     */
    void readMapDataFromFile(std::string file_name){
        std::string row_data;
        std::ifstream world_file(file_name);
        int x_pos = 0;
        int y_pos = 0;
        tile_map_width_ = -1;
        while (getline(world_file, row_data)){
            x_pos = 0;
            for(char tile_data : row_data){
                addTileToMap(tile_data, x_pos, y_pos);
                ++x_pos;
            }
            if(tile_map_width_ == -1){
                tile_map_width_ = x_pos;
            }
            if(x_pos != tile_map_width_){
                throw MapStructureError("Mismatch between size of the map and number of tiles");
            }

            ++y_pos;
        }
        tile_map_height_ = y_pos;
        tile_map_width_ = x_pos;

        if (static_cast<size_t>(tile_map_height_ * tile_map_width_) != tile_map_.size()){
            throw MapStructureError("Mismatch between size of the map and number of tiles");
        }
    }

    /**
     * @brief Generate new tile based on provided char and add it to the map.
     *
     * This method will generate new object of class T, representing new Tile. The data about this tile is based on provided
     * char representing tile. After that, new tile will be added to the tile map.
     *
     * @note This method should only add tile at the end of vector representing tile map, to avoid mismatch between tiles.
     * Because of that, x_pos and y_pos arguments of this method are only for tile to get data about its own position.
     * This method should only be used while generating whole tile map.
     *
     * @param tile_type char, representing tile, which should be added.
     * @param x_pos X position of new tile on tile map.
     * @param y_pos Y position of new tile on tile map.
     */
    virtual void addTileToMap(char tile_type, int x_pos, int y_pos) = 0;

    /**
     * @brief Get the TileType of tile, which is currently last non-border tile added while building map.
     *
     * This method, which should be used when building new tile map, is used to give now-generating new tile a TileType,
     * in situation when char representing new tile does not contain information about tile type should be used for this tile.
     * This way, new tile will have same tile type as one of previous tiles.
     *
     * This method check for latest added tile, which does not have border tile type, and returns its tile type. If there wasn't any
     * such tile added, then GRASS_TILE is returned.
     *
     * @return TileType Tile type of last non-border tile added while building map, of GRASS_TILE if there wasn;t any such tile.
     */
    TileType getTypeOfPreviousTile() const{
        int current_tile_map_size = getTileMap().size() - 1;

        while (current_tile_map_size >= 0){
            TileType previous_tile_type = getTileAt(current_tile_map_size).getTileType();
            if (!Tile::isTileTypeBorderType(previous_tile_type)){
                return previous_tile_type;
            }
            --current_tile_map_size;
        }
        return GRASS_TILE; //Grass is default type in this method
    }

public:

    TileMap(){};

    /**
     * @brief Build a tile map, by creating and adding all necessary tile and other resources to it.
     *
     * Definition of this method should use readMapDataFromFile() method with provided path to map data.
     *
     */
    virtual void initializeMap() = 0;

    int getTileMapWidth() const{
        return tile_map_width_;
    }

    int getTileMapHeight() const{
        return tile_map_height_;
    }

    /// @brief Get reference to whole map, as a vector of all tiles.
    /// Tiles are ordered by lower y coordinate first, and then by x coordinate first.
    const std::vector<std::unique_ptr<T>>& getTileMap() const{
        return tile_map_;
    }

    /// @brief Get reference to the Tile, which has coordinates (x, y) on world map.
    T& getTileAt(const int x, const int y) const{
        return *tile_map_[y * tile_map_width_ + x];
    }

    /// @brief Get reference to the Tile, which is at 'id' index of the vector representing whole tile map.
    T& getTileAt(const int id) const{
        return *tile_map_[id];
    }

    /**
     * @brief Calculate the distance between two tiles based on their coordinates on tile map.
     *
     * @param x1 X coordinate of first tile.
     * @param y1 Y coordinate of first tile.
     * @param x2 X coordinate of second tile.
     * @param y2 Y coordinate of second tile.
     * @return float distance between these two tiles.
     */
    float getDistanceBetweenTiles(const int x1, const int y1, const int x2, const int y2) const{
        return std::sqrt(std::pow(x2-x1, 2) + std::pow(y2-y1, 2));
    }


    /**
     * @brief Get the neighboring tiles of provided tile at position (x, y), and get their corresponding move directions.
     *
     * This function examines all adjacent tiles (including diagonals) around the tile at coordinates (x, y),
     * and returns a pair consisting of two vectores: First vector is a vector of references to the neighboring tiles,
     * second is vector of WorldMove values representing the direction to each neighbor with respect to checked tile.
     *
     * The function ensures that only valid neighbors within the bounds of the tile map are included.
     *
     * @param x The x coordinate of the tile whose neighbors are to be found.
     * @param y The y coordinate of the tile whose neighbors are to be found.
     * @return std::pair<std::vector<std::reference_wrapper<T>>, std::vector<WorldMove>>
     * A pair containing vector of references to neighboring tiles and vector of corresponding move directions (WorldMove).
     */
    const std::pair<std::vector<std::reference_wrapper<T>>, std::vector<WorldMove>> getNeighboursOfTile(const int x, const int y) const{
        std::vector<std::reference_wrapper<T>> neighbours;
        std::vector<WorldMove> directions;

        for (int i = -1; i<=1; ++i){
            for(int j = -1; j<=1; ++j){
                if ((i != 0 || j != 0) && x+i >= 0 && x+i < tile_map_width_ && y+j >= 0 && y+j < tile_map_height_){
                    neighbours.push_back(getTileAt(x+i, y+j));
                    int move_dir_int = 3*(j+1)+(i+1);
                    if (move_dir_int > 4 ){
                        --move_dir_int;

                    }

                    directions.push_back(static_cast<WorldMove>(move_dir_int));
                }
            }
        }
        return std::pair<std::vector<std::reference_wrapper<T>>, std::vector<WorldMove>>(neighbours, directions);
    }


    /**
     * @brief Returns a vector of tile coordinates of tiles, which are within a given radius from a specified position.
     *
     * This function computes all valid tile coordinates within a circular area
     * centred at (pos_x, pos_y), with the specified radius. Only tiles within the bounds
     * of the tile map are included in the result.
     *
     * @param pos_x The x coordinate of the center position.
     * @param pos_y The y coordinate of the center position.
     * @param radius The radius within which to search for tiles.
     * @return std::vector<std::pair<int, int>> A vector of pairs (x, y) representing the coordinates of tiles within the radius.
     */
    std::vector<std::pair<int, int>> getTilesWithinRadius(const int pos_x, const int pos_y, const int radius) const{
        std::vector<std::pair<int, int>> result;
        int dx, dy;
        for (int i = pos_x - radius; i <= pos_x + radius ; ++i){
            for (int j = pos_y - radius; j <= pos_y + radius; ++j){
                if(i >= 0 && i < tile_map_width_ && j >= 0 && j < tile_map_height_){
                    dx = pos_x - i;
                    dy = pos_y - j;
                    if(dx * dx + dy * dy <= radius * radius){
                        result.push_back(std::pair<int, int>(i,j));
                    }
                }
            }
        }
        return result;
    }

    /**
     * @brief Move provided object, into new tile, which is at given coordinates.
     *
     * As a result, tile, which had this object will be empty, and tile at provided coordinates will get this object.
     *
     * The object to move must be a MovableObject, which is already present on tile map.
     * Tile at provided coordinates must not have any other object on it.
     *
     * @param object_to_move MovableObject& to be moved.
     * @param new_x_pos X coordinate of tile to add object to.
     * @param new_y_pos Y coordinate of tile to add object to.
     */
    void moveObject(MovableObject& object_to_move, const int new_x_pos, const int new_y_pos){
        if (!(new_x_pos >= 0 && new_x_pos < getTileMapWidth()) && !(new_y_pos >= 0 && new_y_pos < getTileMapHeight())){
            std::cout << "Cannot move object outside the tile map border" << std::endl;
            return;
        }

        if (!getTileAt(new_x_pos, new_y_pos).isTileWalkable()){
            std::cout << "Cannot move objects to tile " << new_x_pos << " " << new_y_pos << std::endl;
            return;
        }

        int old_pos_x = object_to_move.getXPos();
        int old_pos_y = object_to_move.getYPos();

        getTileAt(old_pos_x, old_pos_y).moveObjectOnTile(getTileAt(new_x_pos, new_y_pos));
        object_to_move.setXPos(new_x_pos);
        object_to_move.setYPos(new_y_pos);

    }

    /**
     * @brief Move provided object, into new tile, which is next to tile which currently has this object in provided (MovePos) direction.
     *
     * As a result, tile, which had this object will be empty, and tile at provided coordinates will get this object.
     *
     * The object to move must be a MovableObject, which is already present on tile map.
     * Tile at provided coordinates must not have any other object on it.
     *
     * @param object_to_move MovableObject& to be moved.
     * @param move WorldMove, in which direction this object should be moved.
     */
    void moveObject(MovableObject& object_to_move, const WorldMove move){
        std::pair<int,int> move_pos = worldMoveToInt(move);
        TileMap::moveObject(object_to_move, object_to_move.getXPos()+move_pos.first, object_to_move.getYPos()+move_pos.second);

    }

    /**

     * @brief Move provided object, into new tile, which is next to tile which currently has this object in provided (MovePos) direction.
     *
     * As a result, tile, which had this object will be empty, and tile at provided coordinates will get this object.
     *
     * The object to move must be a MovableObject, which is already present on tile map.
     * Tile at provided coordinates must not have any other object on it.
     *
     * @param move_object shared_ptr of MovableObject to be moved.
     * @param new_x_pos X coordinate of tile to add object to.
     * @param new_y_pos Y coordinate of tile to add object to.
     */
    void moveObject(std::shared_ptr<MovableObject> new_object, const int new_x_pos, const int new_y_pos){

        getTileAt(new_x_pos, new_y_pos).addObjectOnTile(new_object);
        new_object->setXPos(new_x_pos);
        new_object->setYPos(new_y_pos);

    }



    struct TileMapPath{
        T* tile;
        std::vector<WorldMove> path;
        int cost;

        int estimatedTotalCost(int x_end, int y_end, std::function<int(int, int, int, int)> heuristic) const{
            return cost + heuristic(tile->getXPos(), tile->getYPos(), x_end, y_end);
        }

        bool operator>(const TileMapPath& other) const{
            return cost > other.cost;
        }
    };
    /**
     * @brief Finds a path from a starting tile to a target tile using an A* pathfinding algorithm.
     *
     * This function attempts to find a path from the tile at (x_start, y_start) to the tile at (x_end, y_end).
     * It uses a customizable end condition to determine when the target has been reached.
     * The algorithm explores walkable neighboring tiles and accumulates movement costs, returning the sequence
     * of moves and the total cost of the path if any valid path was found.
     *
     * @param x_start The x-coordinate of the starting tile.
     * @param y_start The y-coordinate of the starting tile.
     * @param x_end The x-coordinate of the target tile.
     * @param y_end The y-coordinate of the target tile.
     * @param end_condition A function that determines if the current tile is the target and algorithm can stop.
     *        By default, it checks if the current coordinates match the target coordinates.
     *        Signature of end_condition must be bool(int x1, int y1, int x2, int y2).
     * @param goal_is_not_walkable If true, allows the goal tile to be non-walkable.
     * @return std::pair<std::vector<WorldMove>, int>
     *         - first: the sequence of moves (as WorldMove objects) to reach the target tile,
     *         - second: the total movement cost of the path (or -1 if no path is found).
     *
     */
    const std::pair<std::vector<WorldMove>, int> findPathToTile(const int x_start, const int y_start,
        const int x_end, const int y_end, std::function<bool(int, int, int, int)> end_condition =
        [](int x1, int y1, int x2, int y2){return x1 == x2 && y1 == y2;}, bool goal_is_not_walkable = false) const{

        auto heuristic = [this](int x1, int y1, int x2, int y2){
            return getDistanceBetweenTiles(x1, y1, x2, y2);
        };

        auto comparator = [&](const TileMapPath& a, const TileMapPath& b) {
            return a.estimatedTotalCost(x_end, y_end, heuristic) > b.estimatedTotalCost(x_end, y_end, heuristic);
        };

        std::priority_queue<TileMapPath, std::vector<TileMapPath>, decltype(comparator)> open_set(comparator);
        std::unordered_set<T*> visited;

        T* start_tile = &getTileAt(x_start, y_start);
        open_set.push({start_tile, {}, 0});

        while (!open_set.empty()){
            TileMapPath current = open_set.top();
            open_set.pop();

            T* current_tile = current.tile;

            if (visited.count(current_tile)){
                continue;
            }

            visited.insert(current_tile);

            int x = current_tile->getXPos();
            int y = current_tile->getYPos();

            if (end_condition(x, y, x_end, y_end)){
                return {current.path, current.cost};
            }

            auto neighbours = getNeighboursOfTile(x, y);
            for (size_t i = 0; i < neighbours.first.size(); ++i){
                T& neighbor = neighbours.first.at(i).get();
                int neighbour_x = neighbor.getXPos();
                int neighbour_y = neighbor.getYPos();

                if (!neighbor.isTileWalkable() && !(goal_is_not_walkable && neighbour_x == x_end && neighbour_y == y_end)){
                    continue;
                }

                int move_cost = current.cost + neighbor.getMoveCost();
                std::vector<WorldMove> new_path = current.path;
                new_path.push_back(neighbours.second.at(i));

                open_set.push({&neighbor, new_path, move_cost});
            }
        }

        return {{}, -1}; // no path found
    }


    const std::pair<std::vector<WorldMove>, int> findPathToTile(const int x_start, const int y_start,
        const int x_end, const int y_end, bool goal_is_not_walkable) const{

            return findPathToTile(x_start, y_start, x_end, y_end,
                [](const int x1, const int y1, const int x2, const int y2){return x1 == x2 && y1 == y2;}, goal_is_not_walkable);
        }

    /**
     * @brief Convert a WorldMove enumeration value to a integer pair representing movement direction.
     *
     * This function maps a given WorldMove value to its corresponding (x, y) movement offset.
     * For Example, UP_LEFT move corresponds to offset {-1, -1} as this is how coordinates changes when
     * going to upper left neighbouring tile.
     *
     * @param move The WorldMove enumeration value to convert.
     * @return std::pair<int, int> The (x, y) movement offset corresponding to the given move.
     */
    static std::pair<int, int> worldMoveToInt(const WorldMove move){
        if(move == NO_MOVE){
            return {0,0};
        }
        int move_int = static_cast<int>(move);
        if (move_int >= 4){
            ++move_int;
        }
        int x_move = (move_int % 3) - 1;
        int y_move = (move_int / 3) - 1;
        return std::pair<int,int>(x_move, y_move);
    }

    /**
     * @brief Get list of all tiles, which are currently walkable (they don;t have any object on it, and are not marked as border tiles).
     *
     * @return const std::vector<std::reference_wrapper<T>> Vector of references to currently walkable tiles.
     */
    const std::vector<std::reference_wrapper<T>> getEmptyTiles() const{
        std::vector<std::reference_wrapper<T>> valid_tiles;
        for (const auto& tile : tile_map_){
            if (tile->isTileWalkable()){
                valid_tiles.push_back(*tile);
            }
        }
        return valid_tiles;
    }


    /**
     * @brief Finds the closest object of which as one of specified types from a given starting position.
     *
     * Iterates through the provided list of tile positions, checks if each tile contains an interactive object
     * of any of the specified types, and calculates the distance from the starting position. Returns the position
     * of the closest matching object, or {-1, -1} if none is found.
     *
     * @param map TileMap& to search within.
     * @param x_start X coordinate of the starting position.
     * @param y_start Y coordinate of the starting position.
     * @param object_types Vector of ObjectType values to search for.
     * @param tiles_to_check Vector of a chunk of tiles from tile map  positions to search.
     * @return std::pair<int, int> Position of the closest object of one the specified types, or {-1, -1} if not found.
     */
    const std::pair<int, int> getClosestObjectOfTypes(const TileMap& map, const int x_start, const int y_start,
        const std::vector<ObjectType> object_types, std::vector<std::pair<int, int>> tiles_to_check) const{

        std::pair<int, int> closet_object_pos = {-1, -1};
        float min_distance = std::numeric_limits<float>::max();

        for( const std::pair<int,int>& tile_pos: tiles_to_check){
            T& event_tile = map.getTileAt(tile_pos.first, tile_pos.second);
            if(event_tile.hasInteractiveObject() &&
            (std::find(object_types.begin(), object_types.end(), event_tile.getObjectOnTile().getInteractiveObjectType()) != object_types.end())){
                float distance = map.getDistanceBetweenTiles(tile_pos.first, tile_pos.second, x_start, y_start);
                if(distance < min_distance){
                    min_distance = distance;
                    closet_object_pos = tile_pos;
                }
            }
        }
        return closet_object_pos;
    }

    /**
     * @brief Get list of coordinates (pair<int, int>), where object can go to in current turn,
     * based on number of moves object can go during its turn.
     *
     * @param object Object, for which list is created.
     * @param include_starting_tile If true, tile on which object is currently on
     * is also added to the list of coordinates.
     */
    const std::vector<std::pair<int, int>> getTilesWhereObjectCanGoTo(const MovableObject& object, bool include_starting_tile = true){

        std::queue<std::pair<int,int>> queue;
        std::map<std::pair<int,int>, int> distance;
        std::map<std::pair<int,int>, std::pair<int,int>> parent;
        std::set<std::pair<int,int>> visited;

        auto start = std::make_pair(object.getXPos(), object.getYPos());
        queue.push(start);
        visited.insert(start);
        distance[start] = 0;

        while(!queue.empty()){
            auto current = queue.front();
            queue.pop();

            for(auto & tile : getNeighboursOfTile(current.first, current.second).first){
                if(tile.get().isTileWalkable()){
                    auto neighbour = std::make_pair(tile.get().getXPos(), tile.get().getYPos());
                    if(visited.count(neighbour) == 0){
                        visited.insert(neighbour);
                        parent[neighbour] = current;
                        distance[neighbour] = distance[current] + 1;
                        queue.push(neighbour);
                    }
                }
            }
        }

        std::vector<std::pair<int, int>> valid_tiles;

        for(const auto& distance_data: distance){
            if(distance_data.second <= object.getMovesLeft() && (include_starting_tile || distance_data.second > 0)){
                valid_tiles.push_back((distance_data.first));
            }
        }

        return valid_tiles;

    }

    /// @brief Get number of moves it takes to get from (x_start, y_start) to (x_end, y_end).
    int getWalkingDistanceBetweenTiles(const int x_start, const int y_start, const int x_end, const int y_end) const{
        auto path_data = findPathToTile(x_start, y_start, x_end, y_end, true);
        int distance = path_data.second;
        if(distance == -1){
            distance = constants::BATTLEFIELD_NO_PATH_DISTANCE;
        }
        return distance;
    }


    TileMap(const TileMap&) = delete;
    TileMap& operator=(const TileMap&) = delete;

};

#endif