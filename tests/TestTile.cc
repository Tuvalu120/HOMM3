/**
 * @file TestTile.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Tile.
 *
 */

#include "CatchConfig.hpp"

#include "GameObjects/TileMaps/Tile.h"
#include "GameObjects/WorldArmy.h"
#include "Miscellaneous/GameExceptions.hpp"

using namespace std;
using namespace game_data;
using namespace constants;

TEST_CASE("Tile Constructor and Getters Test", "[Tile]") {
    int x_pos = 5;
    int y_pos = 10;
    TileType tile_type = TileType::GRASS_TILE;
    int move_cost = 2;
    bool is_wall = false;
    string additional_texture = "grass_texture";

    Tile tile(x_pos, y_pos, tile_type, move_cost, is_wall, additional_texture);

    REQUIRE(tile.getXPos() == x_pos);
    REQUIRE(tile.getYPos() == y_pos);
    REQUIRE(tile.getTileType() == tile_type);
    REQUIRE(tile.getMoveCost() == move_cost);
    REQUIRE(tile.isTileWall() == is_wall);
    REQUIRE(tile.isTileWalkable() == true);
    REQUIRE(tile.getAdditionalTextureName() == additional_texture);
    REQUIRE(tile.hasAdditionalTexture() == true);
};

TEST_CASE("Tile InteractiveObject Test", "[Tile]"){
    std::shared_ptr<WorldArmy> army = WorldArmy::generateArmy(1, LOYNIS, 0, 0);
    int x_pos = 5;
    int y_pos = 10;
    TileType tile_type = TileType::GRASS_TILE;
    int move_cost = 1;

    Tile tile(x_pos, y_pos, tile_type, move_cost);

    REQUIRE(tile.hasInteractiveObject() == false);
    REQUIRE(tile.isTileWalkable() == true);
    REQUIRE_THROWS_AS(tile.getObjectOnTile(), TileLogicError);

    tile.addObjectOnTile(army);
    REQUIRE(tile.hasInteractiveObject() == true);
    REQUIRE(tile.isTileWalkable() == false);
    REQUIRE(&tile.getObjectOnTile() == army.get());
    REQUIRE_THROWS_AS(tile.addObjectOnTile(army), TileLogicError);

    Tile another_tile(x_pos, y_pos, tile_type, move_cost);
    tile.moveObjectOnTile(another_tile);
    REQUIRE(tile.hasInteractiveObject() == false);
    REQUIRE(another_tile.hasInteractiveObject() == true);
    REQUIRE(&another_tile.getObjectOnTile() == army.get());

    Tile wall_tile(x_pos, y_pos, tile_type, move_cost, true);
    REQUIRE_THROWS_AS(another_tile.moveObjectOnTile(wall_tile), TileLogicError);
    REQUIRE(another_tile.hasInteractiveObject() == true);
    another_tile.removeObjectOnTile();
    REQUIRE(another_tile.hasInteractiveObject() == false);
    REQUIRE(army.use_count() == 1);



}