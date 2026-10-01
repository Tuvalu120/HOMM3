/**
 * @file TestBattlefield.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Battlefield.
 *
 */

#include "Data/ObjectDataEnums.hpp"
#include "GameObjects/TileMaps/Battlefield.h"
#include "GameObjects/TileMaps/BattlefieldTile.hpp"
#include "Managers/PlayerManager.h"
#include "TestUtilities.hpp"

using namespace std;
using namespace game_data;
using namespace constants;

class TestBattlefield : public Battlefield{
    public:
        TestBattlefield(): Battlefield(
            PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0),
            PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0),
            GRASS_TILE
        ){
            initializeMap();
        };
};

TestBattlefield initializeBattlefield(){
    initializeMockGame();
    return TestBattlefield();
}

TEST_CASE("Battlefield Initialization Test", "[Battlefield]"){

    TestBattlefield battlefield = initializeBattlefield();

    REQUIRE(battlefield.getTileMapWidth() == 20);
    REQUIRE(battlefield.getTileMapHeight() == 10);
    REQUIRE(battlefield.getBattlefieldStyle() >= GRASS_TILE);
    REQUIRE(battlefield.getBattlefieldStyle() <= SWAMP_TILE);
    REQUIRE(battlefield.getInitAttackerStacksSpawnPos().size() == MAX_NUMBER_OF_STACKS);
    REQUIRE(battlefield.getInitDefenderStacksSpawnPos().size() == MAX_NUMBER_OF_STACKS);
}

TEST_CASE("Battlefield Tile Map Data Test", "[Battlefield]"){

    TestBattlefield battlefield = initializeBattlefield();

    REQUIRE(battlefield.getTileAt(0, 0).isTileWall() == true);
    REQUIRE(battlefield.getTileAt(0, BATTLEFIELD_HEIGHT-1).isTileWall() == true);
    REQUIRE(battlefield.getTileAt(BATTLEFIELD_WIDTH-1, 0).isTileWall() == true);
    REQUIRE(battlefield.getTileAt(BATTLEFIELD_WIDTH-1, BATTLEFIELD_HEIGHT-1).isTileWall() == true);

    REQUIRE(battlefield.getTileAt(1, 1).isTileWall() == false);
    REQUIRE(battlefield.getInitAttackerStacksSpawnPos().size() == MAX_NUMBER_OF_STACKS);
    REQUIRE(battlefield.getInitDefenderStacksSpawnPos().size() == MAX_NUMBER_OF_STACKS);

    REQUIRE(battlefield.getInitAttackerStacksSpawnPos().at(0) == make_pair(1, 2));
}
