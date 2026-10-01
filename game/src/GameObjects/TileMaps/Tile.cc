/**
 * @file Tile.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Tile.
 *
 */


#include "GameObjects/TileMaps/Tile.h"
#include "Data/Constants.hpp"
#include "Data/GameData.hpp"
#include "Miscellaneous/GameExceptions.hpp"

using namespace std;
using namespace game_data;
using namespace constants;

Tile::Tile(int x_pos, int y_pos, const TileType tile_type, int move_cost, bool is_wall,
    string additional_texture):
    DrawableObject(generateTextureName(static_cast<TileType>(tile_type % N_TERRAIN_TYPES))),
    x_pos_(x_pos),y_pos_(y_pos),
    tile_type_(tile_type),additional_texture_name_(additional_texture),
    move_cost_(move_cost),is_wall_(is_wall){}

int Tile::getXPos() const{
    return x_pos_;
}

int Tile::getYPos() const{
    return y_pos_;
}

int Tile::getMoveCost() const{
    return move_cost_;
}

const string Tile::getAdditionalTextureName() const{
    return additional_texture_name_;
}

bool Tile::hasAdditionalTexture() const{
    return !additional_texture_name_.empty();
}

void Tile::addObjectOnTile(shared_ptr<InteractiveObject> new_object_on_tile){
    if(object_on_tile_ == nullptr && !isTileWall()){
        object_on_tile_ = new_object_on_tile;
    }
    else if(object_on_tile_ != nullptr){
        throw TileLogicError("Tried to add object on Tile which already has object");
    }
    else{
        throw TileLogicError("Tried to add object on Tile which is wall");
    }
}

bool Tile::hasInteractiveObject() const{
    return object_on_tile_ != nullptr;
}

void Tile::moveObjectOnTile(Tile& other_tile){
    if (!other_tile.isTileWall()){
        other_tile.addObjectOnTile(std::move(object_on_tile_));
    }
    else{
        throw TileLogicError("Cannot move object into tile marked as wall");
    }
}

void Tile::removeObjectOnTile(){
    object_on_tile_ = nullptr;
}

InteractiveObject& Tile::getObjectOnTile() const{
    if(hasInteractiveObject()){
        return *object_on_tile_;
    }
    throw TileLogicError("Tried to access InteractiveObject of Tile, which does not have it");
}

bool Tile::isTileWall() const{
    return is_wall_;
}

void Tile::setFlagIsTileWall(const bool is_wall){
    is_wall_ = is_wall;
}

bool Tile::isTileWalkable() const{
    return !isTileWall() && !hasInteractiveObject();
}

TileType Tile::getTileType() const{
    return tile_type_;
}

void Tile::setTileType(const TileType tile_type){
    tile_type_ = tile_type;
    setCurrentTextureName(generateTextureName(tile_type));
}

string Tile::generateTextureName(TileType tile_type){

    if (isTileTypeBorderType(tile_type)){
        return EMPTY_IMAGE;
    }
    else{
        vector<string> textures = TILE_TEXTURES.at(tile_type);
        return chooseFromContainer(textures);
    }
}

bool Tile::isTileTypeBorderType(TileType tile_type){
    auto& border_tile_types = BORDER_TILE_TYPES;
    return (find(border_tile_types.begin(), border_tile_types.end(), tile_type) != border_tile_types.end());
};

TileType Tile::getBorderTileTypeFromWalkableTileType(const TileType tile_type){
    return BORDER_TILE_TYPES.at(tile_type);
}

pair<int, int> Tile::getPos() const{
    return {x_pos_, y_pos_};
}
