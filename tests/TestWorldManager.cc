/**
 * @file TestWorldManager.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class WorldManager.
 *
 */

#include <thread>

#include "CatchConfig.hpp"
#include "TestUtilities.hpp"

#include "Managers/WorldManager.h"
#include "GameObjects/TileMaps/WorldMap.h"
#include "GameObjects/Player.h"
#include "GameObjects/WorldObjects/WorldResource.h"

using namespace std;
using namespace constants;
using namespace game_data;

TEST_CASE("WorldManager Initialization Test", "[WorldManager]"){
    initializeMockGame();

    REQUIRE(WorldManager::getInstance().getCurrentDay() == 1);
    REQUIRE(WorldManager::getInstance().getCurrentWeek() == 1);
    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 0);
}

TEST_CASE("WorldManager Turn Progression Test", "[WorldManager][PlayerManager]"){
    initializeMockGame();
    int initial_day = WorldManager::getInstance().getCurrentDay();
    int n_players = PlayerManager::getInstance().getNOfPlayers();


    for(int i = 0; i < n_players; ++i){
        REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == i);
        WorldManager::getInstance().endTurnOfArmy();
    }
    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 0);
    REQUIRE(WorldManager::getInstance().getCurrentDay() == initial_day + 1);

    HeroType hero_to_add = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().getHeroType() == XARFAX ?
        SANYA : XARFAX;
    PlayerManager::getInstance().getPlayerByID(0).addArmyToPlayer(hero_to_add, 20, 20);
    WorldManager::getInstance().endTurnOfArmy();
    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 0);
    REQUIRE(&PlayerManager::getInstance().getPlayerByID(0).getCurrentArmy() ==
        &PlayerManager::getInstance().getPlayerByID(0).getArmyByID(1));

}

TEST_CASE("WorldManager WorldMap Test", "[WorldManager][WorldMap][Tile]"){
    initializeMockGame();

    WorldMap& world_map = WorldManager::getInstance().getWorldMap();
    //Expecting the world map to be initialized with a size of 128x128,
    //as this is size of tile map file (128 lines of text, each with 128 chars).
    int expected_tile_map_size_height = 128, expected_tile_map_size_width = 128;
    REQUIRE(world_map.getTileMapHeight() == expected_tile_map_size_height);
    REQUIRE(world_map.getTileMapWidth() == expected_tile_map_size_width);

    Tile::isTileTypeBorderType(world_map.getTileAt(0, 0).getTileType());
    Tile::isTileTypeBorderType(world_map.getTileAt(0, expected_tile_map_size_height-1).getTileType());
    Tile::isTileTypeBorderType(world_map.getTileAt(expected_tile_map_size_width-1, 0).getTileType());
    Tile::isTileTypeBorderType(world_map.getTileAt(expected_tile_map_size_width-1, expected_tile_map_size_height-1).getTileType());

    REQUIRE(world_map.countNumberOfObjectOfType(MINE) == NUMBER_OF_MINES_ON_MAP);
    REQUIRE(world_map.countNumberOfObjectOfType(NEUTRAL_TROOPS) == NUMBER_OF_NEUTRAL_TROOPS_ON_MAP);
    REQUIRE(world_map.countNumberOfObjectOfType(RESOURCE) == NUMBER_OF_WORLD_RESOURCES_ON_MAP);
    REQUIRE(world_map.countNumberOfObjectOfType(SCHOLAR) == NUMBER_OF_SCHOLARS_ON_MAP);
    REQUIRE(world_map.countNumberOfObjectOfType(TREASURE_CHEST) == NUMBER_OF_CHESTS_ON_MAP);

}

TEST_CASE("WorldManager Player Removing Test", "[WorldManager][PlayerManager][BattleManager]"){

    thread game_thread_1 = playMockGame();

    PlayerManager::getInstance().removePlayerFromGame(1);
    REQUIRE(PlayerManager::getInstance().getNOfActivePlayers() == 3);
    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 0);
    mockEndTurnOfCurrentWorldArmy();

    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 2);
    endGame(game_thread_1);

    //////////////

    thread game_thread_2 = playMockGame();

    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 0);

    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);

    int battle_x_pos = 15;
    int battle_y_pos = 15;

    moveArmiesForTest(attacker, defender, battle_x_pos, battle_y_pos);
    setWorldDecisionVars(battle_x_pos, battle_y_pos);

    REQUIRE(BattleManager::getInstance().getCurrentBattle().isBattleInProgress() == true);
    endBattle(1); //Current player (0) loses battle and should be eliminated from game

    REQUIRE(PlayerManager::getInstance().getNOfActivePlayers() == 3);
    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 1); //current player ID should automatically be 1

    endGame(game_thread_2);

}

TEST_CASE("WorldManager Interact Test", "[WorldManager]"){

    thread game_thread = playMockGame();

    int event_x_pos = 5;
    int event_y_pos = 5;

    WorldArmy& event_army = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldManager::getInstance().getWorldMap().moveObject(event_army, event_x_pos-1, event_y_pos);

    shared_ptr<WorldResource> event_object = make_shared<WorldResource>();
    setPositionOfWorldGeneratedInteractiveObject(event_object, event_x_pos+1, event_y_pos);
    pauseGame(100);
    REQUIRE(event_object.use_count() == 2);

    setWorldDecisionVars(event_x_pos, event_y_pos);
    REQUIRE(event_object.use_count() == 1);

    endGame(game_thread);
}


TEST_CASE("WorldManager Interact With Multiple Objects Test", "[WorldManager]"){

    thread game_thread = playMockGame();

    int currentPlayerID = 0;
    WorldArmy& event_army = PlayerManager::getInstance().getPlayerByID(currentPlayerID).getArmyByID(0);

    int event_x_pos = event_army.getXPos();
    int event_y_pos = event_army.getYPos();

    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(event_x_pos-1, event_y_pos).hasInteractiveObject() == true);
    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(event_x_pos-1, event_y_pos).getObjectOnTile().getInteractiveObjectType() == PART_OF_TOWN);
    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(event_x_pos-1, event_y_pos-1).getObjectOnTile().getInteractiveObjectType() == PART_OF_TOWN);

    shared_ptr<WorldResource> event_object = make_shared<WorldResource>();
    setPositionOfWorldGeneratedInteractiveObject(event_object, event_x_pos+1, event_y_pos-1);
    pauseGame(100);
    REQUIRE(event_object.use_count() == 2);

    setWorldDecisionVars(event_x_pos, event_y_pos-1);
    REQUIRE(event_object.use_count() == 1); //Object should be destroyed after interaction
    REQUIRE_NOTHROW(TownManager::getInstance().getCurrentTown()); //Check if town is being visited - if not, this is error and there should be throw
    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == currentPlayerID);
    setWorldDecisionVars(LEAVE_TOWN);

    endGame(game_thread);
}


TEST_CASE("WorldManager Interact With Multiple Objects Only Low Priority Test", "[WorldManager]"){

    thread game_thread = playMockGame();

    int currentPlayerID = 0;
    WorldArmy& event_army = PlayerManager::getInstance().getPlayerByID(currentPlayerID).getArmyByID(0);

    int event_x_pos = event_army.getXPos();
    int event_y_pos = event_army.getYPos();

    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(event_x_pos-1, event_y_pos).hasInteractiveObject() == true);
    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(event_x_pos-1, event_y_pos).getObjectOnTile().getInteractiveObjectType() == PART_OF_TOWN);
    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(event_x_pos-1, event_y_pos-1).getObjectOnTile().getInteractiveObjectType() == PART_OF_TOWN);

    shared_ptr<WorldResource> event_object = make_shared<WorldResource>();
    setPositionOfWorldGeneratedInteractiveObject(event_object, event_x_pos, event_y_pos-1);
    pauseGame(100);
    REQUIRE(event_object.use_count() == 2);

    mockEndTurnOfCurrentWorldArmy();
    REQUIRE(event_object.use_count() == 1); //Object should still be destroyed.
    REQUIRE_THROWS_AS(TownManager::getInstance().getCurrentTown(), TownLogicError); //No town should be visited right now
    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == currentPlayerID+1);
    endGame(game_thread);
}