/**
 * @file WorldMap.cc
 * @author Bartosz Han
 * @brief This file contains definition of class WorldMap.
 *
 */


#include <memory>

#include "GameObjects/TileMaps/WorldMap.h"
#include "GameObjects/TileMaps/WorldTile.hpp"
#include "Data/Constants.hpp"
#include "Data/GameData.hpp"
#include "Miscellaneous/GameExceptions.hpp"


using namespace std;
using namespace game_data;
using namespace constants;

WorldMap::WorldMap():
    TileMap(){};

void WorldMap::initializeMap(){
    readMapDataFromFile(ASSETS_PATH + WORLD_MAP_FILE_PATH);
}

void WorldMap::addTileToMap(const char tile_type, const int x_pos, const int y_pos){
    unique_ptr<WorldTile> new_tile;
    switch (tile_type)
    {

    case 'g': //Grass
        new_tile = make_unique<WorldTile>(x_pos, y_pos, GRASS_TILE);
        tile_map_.push_back(std::move(new_tile));
        break;

    case 'r': //Rough
        new_tile = make_unique<WorldTile>(x_pos, y_pos, ROUGH_TILE);
        tile_map_.push_back(std::move(new_tile));
        break;

    case 's': //Snow
        new_tile = make_unique<WorldTile>(x_pos, y_pos, SNOW_TILE);
        tile_map_.push_back(std::move(new_tile));
        break;

    case 'w': //sWamp
        new_tile = make_unique<WorldTile>(x_pos, y_pos, SWAMP_TILE);
        tile_map_.push_back(std::move(new_tile));
        break;

    case 'G': //Grass
        new_tile = make_unique<WorldTile>(x_pos, y_pos, GRASS_BORDER_TILE, 1, true, WORLD_MAP_BORDERS.at(GRASS_TILE));
        tile_map_.push_back(std::move(new_tile));
        break;

    case 'R': //Rough
        new_tile = make_unique<WorldTile>(x_pos, y_pos, ROUGH_BORDER_TILE, 1, true, WORLD_MAP_BORDERS.at(ROUGH_TILE));
        tile_map_.push_back(std::move(new_tile));
        break;

    case 'S': //Snow
        new_tile = make_unique<WorldTile>(x_pos, y_pos, SNOW_BORDER_TILE, 1, true, WORLD_MAP_BORDERS.at(SNOW_TILE));
        tile_map_.push_back(std::move(new_tile));
        break;

    case 'W': //sWamp
        new_tile = make_unique<WorldTile>(x_pos, y_pos, SWAMP_BORDER_TILE, 1, true, WORLD_MAP_BORDERS.at(SWAMP_TILE));
        tile_map_.push_back(std::move(new_tile));
        break;

    case 'B': //Border
        new_tile = make_unique<WorldTile>(x_pos, y_pos, WORLD_BORDER_TILE, 1, true);
        tile_map_.push_back(std::move(new_tile));
        break;

    case 'T': //Town
        new_tile = make_unique<WorldTile>(x_pos, y_pos, getTypeOfPreviousTile());
        town_possible_cords_.push_back({x_pos, y_pos});
        tile_map_.push_back(std::move(new_tile));
        break;

    default:
        throw UnknownMapCharError(tile_type, WORLD_MAP_FILE_PATH);
        break;
    }
}

const vector<pair<int,int>>& WorldMap::getTownPossibleCoords() const{
    return town_possible_cords_;
}

int WorldMap::countNumberOfObjectOfType(const ObjectType type) const{
    int counter = 0;
    for(const unique_ptr<WorldTile>& tile: getTileMap()){
        if(tile->hasInteractiveObject() && tile->getObjectOnTile().getInteractiveObjectType() == type){
            ++counter;
        }
    }
    return counter;
}
