/**
 * @file TestTownManager.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class TownManager.
 *
 */

#include "CatchConfig.hpp"
#include "TestUtilities.hpp"
#include "Managers/TownManager.h"
#include "GameObjects/TownInteractiveObject.h"
#include "GameObjects/Town.h"
#include "Data/Constants.hpp"

using namespace std;
using namespace game_data;
using namespace constants;

TEST_CASE("TownManager: generateTowns Test", "[TownManager][Town]"){
    WorldManager::getInstance().createWorldMap();
    int n_towns = 1;
    vector<pair<int, int>> coords = {{5, 5}};
    TownManager::getInstance().generateTowns(coords, n_towns);

    REQUIRE(TownManager::getInstance().getTownWithID(0).getTownID() == 0);

    for(const auto& coord : coords){
        REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(coord.first, coord.second).getObjectOnTile().getInteractiveObjectType() == PART_OF_TOWN);
    }

    int interactive_object_type_count = 0;
    for(const auto& tile: WorldManager::getInstance().getWorldMap().getTileMap()){
        if(tile->hasInteractiveObject() && tile->getObjectOnTile().getInteractiveObjectType() == PART_OF_TOWN){
            ++interactive_object_type_count;
        }
    }
    REQUIRE(interactive_object_type_count == n_towns * TOWN_WORLD_MAP_WIDTH * TOWN_WORLD_MAP_HEIGHT);
}


TEST_CASE("TownManager getCurrentTown Test", "[TownManager][Town]"){

    thread game_thread = playMockGame();

    pair<int, int> army_pos = enterFriendlyTown();

    REQUIRE_NOTHROW(TownManager::getInstance().getCurrentTown());

    int current_town_id = TownManager::getInstance().getCurrentTownID();
    Town& current_town = TownManager::getInstance().getCurrentTown();

    InteractiveObject* town_object = &WorldManager::getInstance().getWorldMap().getTileAt(army_pos.first-1, army_pos.second).getObjectOnTile();
    REQUIRE(town_object->getInteractiveObjectType() == PART_OF_TOWN);

    TownInteractiveObject* part_of_town = static_cast<TownInteractiveObject*>(town_object);

    REQUIRE(&current_town == &TownManager::getInstance().getTownWithID(current_town_id));
    REQUIRE(&part_of_town->getTownOfObject() == &current_town);


    WorldArmy& world_army_1 = WorldManager::getInstance().getCurrentPlayer().getArmyByID(0);

    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == world_army_1.getPlayerID());
    setWorldDecisionVars(LEAVE_TOWN);
    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == world_army_1.getPlayerID());

    REQUIRE_THROWS_AS(TownManager::getInstance().getCurrentTown(), TownLogicError);
    REQUIRE(TownManager::getInstance().getCurrentTownID() == -1);

    setWorldDecisionVars(army_pos.first, army_pos.second);
    REQUIRE(GraphicsManager::getInstance().getCurrentScene() == WORLD);

    mockEndTurnOfCurrentWorldArmy();

    WorldArmy& world_army_2 = WorldManager::getInstance().getCurrentPlayer().getArmyByID(0);
    setWorldDecisionVars(world_army_2.getXPos(), world_army_2.getYPos()-1);
    REQUIRE(GraphicsManager::getInstance().getCurrentScene() == TOWN);
    setWorldDecisionVars(LEAVE_TOWN);

    endGame(game_thread);
}

TEST_CASE("TownManager Town Ownership Test", "[TownManager]"){

    thread game_thread = playMockGame();

    for (int i=0 ; i<MAX_NUMBER_OF_PLAYERS; ++i){
        REQUIRE(TownManager::getInstance().getNOfTownsOfPlayer(i) == 1);
    }

    mockEndTurnOfCurrentWorldArmy();
    PlayerManager::getInstance().removePlayerFromGame(0);
    REQUIRE(TownManager::getInstance().getNOfTownsOfPlayer(0) == 0);
    endGame(game_thread);
}
