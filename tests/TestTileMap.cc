/**
 * @file TestTileMap.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class TileMap.
 *
 */

#include "CatchConfig.hpp"

#include "GameObjects/TileMaps/TileMap.hpp"
#include "GameObjects/TileMaps/Tile.h"
#include "GameObjects/WorldArmy.h"
#include "Miscellaneous/GameExceptions.hpp"

using namespace std;
using namespace game_data;
using namespace constants;

class TestTileMap : public TileMap<Tile>{

private:
    string file_name_;

public:
    TestTileMap(string file_name):file_name_(executable_path_ + "/../../tests/" + file_name){
        initializeMap();
    };

    void initializeMap() override {
        readMapDataFromFile(file_name_);
    }

    void addTileToMap(char tile_type, int x_pos, int y_pos) override {
        unique_ptr<Tile> new_tile;
        switch (tile_type){

        case 'G':
            new_tile = make_unique<Tile>(x_pos, y_pos, GRASS_TILE, 1, false);
            tile_map_.push_back(std::move(new_tile));
            break;

        case 'X':
            new_tile = make_unique<Tile>(x_pos, y_pos, GRASS_BORDER_TILE, 1, true);
            tile_map_.push_back(std::move(new_tile));
            break;

        default:
            break;

        }
    }
};

TEST_CASE("TileMap Initialization Test", "[TileMap]"){
    TestTileMap tile_map("data/TestMap.txt");

    REQUIRE(tile_map.getTileMapWidth() == 5);
    REQUIRE(tile_map.getTileMapHeight() == 5);
    REQUIRE(tile_map.getTileMap().size() == 25);
}

TEST_CASE("TileMap Get Tile Test", "[TileMap]"){
    TestTileMap tile_map("data/TestMap.txt");

    REQUIRE(tile_map.getTileAt(0, 0).getXPos() == 0);
    REQUIRE(tile_map.getTileAt(0, 0).getYPos() == 0);
    REQUIRE(tile_map.getTileAt(1, 1).getXPos() == 1);
    REQUIRE(tile_map.getTileAt(1, 1).getYPos() == 1);
}

TEST_CASE("TileMap Move Object Test", "[TileMap]"){
    TestTileMap tile_map("data/TestMap.txt");

    int init_x_pos = 1;
    int init_y_pos = 1;
    std::shared_ptr<WorldArmy> army = WorldArmy::generateArmy(1, LOYNIS, init_x_pos, init_y_pos);

    tile_map.getTileAt(init_x_pos, init_y_pos).addObjectOnTile(army);

    REQUIRE(tile_map.getTileAt(init_x_pos, init_y_pos).hasInteractiveObject() == true);
    REQUIRE(tile_map.getTileAt(1, 2).hasInteractiveObject() == false);

    tile_map.moveObject(*army, 1, 2);

    REQUIRE(tile_map.getTileAt(init_x_pos, init_y_pos).hasInteractiveObject() == false);
    REQUIRE(tile_map.getTileAt(1, 2).hasInteractiveObject() == true);
}

TEST_CASE("TileMap Find Path Test", "[TileMap]"){
    TestTileMap tile_map("data/TestMap.txt");

    auto path = tile_map.findPathToTile(1, 1, 3, 3);
    REQUIRE(path.second == 3);
    REQUIRE(path.first.size() == 3);
    REQUIRE(path.first.at(0) == DOWN);
    REQUIRE(path.first.at(1) == DOWN_RIGHT);
    REQUIRE(path.first.at(2) == RIGHT);
}

TEST_CASE("TileMap Failed To Find Path Test", "[TileMap]"){
    TestTileMap tile_map("data/NoPathMap.txt");

    auto path = tile_map.findPathToTile(1, 1, 3, 3);
    REQUIRE(path.first.size() == 0);
    REQUIRE(path.second == -1);

}

TEST_CASE("TileMap Find Path To Not Walkable Tile Test", "[TileMap]"){
    TestTileMap tile_map("data/NoPathMap.txt");

    auto path = tile_map.findPathToTile(1, 1, 2, 2);
    REQUIRE(path.first.size() == 0);
    REQUIRE(path.second == -1);

}

TEST_CASE("TileMap Invalid Maps Test", "[TileMap]"){
    REQUIRE_THROWS_AS(new TestTileMap("data/DifferentSizesMap.txt"), MapStructureError);
    REQUIRE_THROWS_AS(new TestTileMap("data/InvalidCharMap.txt"), MapStructureError);
}
