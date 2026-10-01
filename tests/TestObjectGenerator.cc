/**
 * @file TestObjectGenerator.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class ObjectGenerator.
 *
 */

#include "CatchConfig.hpp"
#include "TestUtilities.hpp"

#include "Miscellaneous/ObjectGenerator.hpp"
#include "Data/Constants.hpp"
#include "Data/GameData.hpp"

using namespace std;
using namespace constants;
using namespace game_data;

TEST_CASE("ObjectGenerator Initialization And Getters Test", "[ObjectGenerator]"){
    WorldManager::getInstance().createWorldMap();
    WorldResourcesGenerator generator(50);
    REQUIRE(generator.getObjectQuantity() == 50);
    REQUIRE(generator.getObjectType() == RESOURCE_OBJECT_TYPE);
    REQUIRE(generator.getWorldObjectsOnMap().size() == 0); // No objects generated yet
}

TEST_CASE("ObjectGenerator setObjectQuantity Test", "[ObjectGenerator]"){
    WorldManager::getInstance().createWorldMap();

    size_t object_quantity = 50;
    WorldResourcesGenerator generator(object_quantity);

    generator.setObjectQuantity(object_quantity);
    REQUIRE(generator.getWorldObjectsOnMap().size() == object_quantity);
    REQUIRE(countObjectsOnMap(RESOURCE_OBJECT_TYPE) == static_cast<int>(object_quantity));

    object_quantity = 100;
    generator.setObjectQuantity(object_quantity);
    REQUIRE(generator.getWorldObjectsOnMap().size() == object_quantity);
    REQUIRE(countObjectsOnMap(RESOURCE_OBJECT_TYPE) == static_cast<int>(object_quantity));

    object_quantity = 20;
    generator.setObjectQuantity(object_quantity);
    REQUIRE(generator.getWorldObjectsOnMap().size() == object_quantity);
    REQUIRE(countObjectsOnMap(RESOURCE_OBJECT_TYPE) == static_cast<int>(object_quantity));

    ///
    object_quantity = 50;
    MinesGenerator mine_generator(object_quantity);

    generator.setObjectQuantity(object_quantity);
    REQUIRE(generator.getWorldObjectsOnMap().size() == object_quantity);
    REQUIRE(countObjectsOnMap(RESOURCE_OBJECT_TYPE) == static_cast<int>(object_quantity));

    object_quantity = 100;
    generator.setObjectQuantity(object_quantity);
    REQUIRE(generator.getWorldObjectsOnMap().size() == object_quantity);
    REQUIRE(countObjectsOnMap(RESOURCE_OBJECT_TYPE) == static_cast<int>(object_quantity));

    object_quantity = 20;
    generator.setObjectQuantity(object_quantity);
    REQUIRE(generator.getWorldObjectsOnMap().size() == object_quantity);
    REQUIRE(countObjectsOnMap(RESOURCE_OBJECT_TYPE) == static_cast<int>(object_quantity));
}

TEST_CASE("Object Generator UpdateObjects Test", "[ObjectGenerator][WorldMap]"){
    size_t object_quantity = 50;
    WorldManager::getInstance().createWorldMap();
    WorldResourcesGenerator generator(object_quantity);
    generator.updateObjects();
    REQUIRE(generator.getWorldObjectsOnMap().size() == object_quantity);
    REQUIRE(generator.getWorldObjectsOnMap().at(0).get().getInteractiveObjectType() == RESOURCE_OBJECT_TYPE);

    int object_x_pos = generator.getWorldObjectsOnMap().at(0).get().getXPos();
    int object_y_pos = generator.getWorldObjectsOnMap().at(0).get().getYPos();

    generator.updateObjects();
    REQUIRE(generator.getWorldObjectsOnMap().size() == object_quantity);

    WorldManager::getInstance().getWorldMap().getTileAt(object_x_pos, object_y_pos).removeObjectOnTile();
    REQUIRE(generator.getWorldObjectsOnMap().size() == object_quantity - 1);
    auto old_valid_object = generator.getWorldObjectsOnMap().at(0);
    generator.updateObjects();
    REQUIRE(generator.getWorldObjectsOnMap().size() == object_quantity);
    auto all_objects = generator.getWorldObjectsOnMap();

    REQUIRE(std::any_of(all_objects.begin(), all_objects.end(),
    [&old_valid_object](const std::reference_wrapper<WorldResource>& obj) {
        return &obj.get() == &old_valid_object.get(); // Compare the underlying objects
    }));

    ///

    int map_size = 128;
    REQUIRE_THROWS_AS(generator.setObjectQuantity(map_size * map_size), MapStructureError);
}

TEST_CASE("WorldNeutralTroopsGenerator Test", "[WorldNeutralTroopsGenerator]"){
    int object_quantity = 50;
    WorldManager::getInstance().createWorldMap();
    WorldNeutralTroopsGenerator generator(object_quantity);
    generator.updateObjects();
    WorldNeutralTroops& neutral_troops = generator.getWorldObjectsOnMap().at(0).get();

    REQUIRE(neutral_troops.getInteractiveObjectType() == NEUTRAL_TROOPS_OBJECT_TYPE);
    REQUIRE(neutral_troops.getObjectArmy().getArmyStacks().size() == 1);
    REQUIRE(neutral_troops.getObjectArmy().getArmyPower() > 0);
    REQUIRE(calculateArmyLevel(neutral_troops.getObjectArmy()) == 1);

}

TEST_CASE("MinesGenerator Initialization Test", "[MinesGenerator]"){
    int object_quantity = 50;
    WorldManager::getInstance().createWorldMap();
    MinesGenerator generator(object_quantity);

    generator.updateObjects();
    Mine& mine = generator.getWorldObjectsOnMap().at(0).get();
    REQUIRE(mine.getInteractiveObjectType() == MINES_OBJECT_TYPE);
    REQUIRE(mine.getPlayerID() == -1);
    REQUIRE(calculateArmyLevel(mine.getMineArmy()) == 2);

}

TEST_CASE("ObjectGenerator Game Integration Test", "[ObjectGenerator]"){

    thread game_thread = playMockGame();

    BaseObjectGenerator& generator = WorldManager::getInstance().getObjectGenerator(RESOURCE_OBJECT_TYPE);
    REQUIRE(generator.getObjectQuantity() == NUMBER_OF_WORLD_RESOURCES_ON_MAP);

    int n_of_resources = countObjectsOnMap(RESOURCE_OBJECT_TYPE);
    REQUIRE(n_of_resources == NUMBER_OF_WORLD_RESOURCES_ON_MAP);

    pair<int, int> resource_pos = findAnyObjectOfType(RESOURCE_OBJECT_TYPE);
    WorldArmy& current_army = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy();
    WorldManager::getInstance().getWorldMap().moveObject(current_army, resource_pos.first+1, resource_pos.second+1);

    setWorldDecisionVars(resource_pos.first+1, resource_pos.second);
    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(resource_pos.first, resource_pos.second).hasInteractiveObject() == false);

    n_of_resources = countObjectsOnMap(RESOURCE_OBJECT_TYPE);
    REQUIRE(n_of_resources == NUMBER_OF_WORLD_RESOURCES_ON_MAP - 1);

    skipWeek();

    n_of_resources = countObjectsOnMap(RESOURCE_OBJECT_TYPE);
    REQUIRE(n_of_resources == NUMBER_OF_WORLD_RESOURCES_ON_MAP);

    endGame(game_thread);
}

TEST_CASE("MineGenerator Game Integration Test", "[MineGenerator]"){

    thread game_thread = playMockGame();

    REQUIRE(countObjectsOnMap(RESOURCE_OBJECT_TYPE) == NUMBER_OF_WORLD_RESOURCES_ON_MAP);

    pair<int, int> mine_pos = findAnyObjectOfType(MINES_OBJECT_TYPE);
    WorldArmy& current_army = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy();

    WorldManager::getInstance().getWorldMap().moveObject(current_army, mine_pos.first+1, mine_pos.second+1);
    setWorldDecisionVars(mine_pos.first+1, mine_pos.second);
    Mine& mine = static_cast<Mine&>(WorldManager::getInstance().getWorldMap().getTileAt(mine_pos.first, mine_pos.second).getObjectOnTile());
    REQUIRE(mine.getPlayerID() == -1);

    REQUIRE(BattleManager::getInstance().isBattleInProgress() == true);
    endBattle(WorldManager::getInstance().getCurrentPlayerID());
    REQUIRE(BattleManager::getInstance().isBattleInProgress() == false);
    REQUIRE(mine.getPlayerID() == 0);

    Resource resource = mine.getResourceType();
    int quantity = mine.getResourceQuantity();

    int init_resource = PlayerManager::getInstance().getPlayerByID(0).getPlayerResources().at(resource);

    skipDay();

    unsigned int expected_resources = init_resource + quantity;
    if(resource == GOLD){
        expected_resources += GOLD_PROVIDED_BY_HALL.at(0);
    }

    REQUIRE(PlayerManager::getInstance().getPlayerByID(0).getPlayerResources().at(resource) == expected_resources);

    ///
    mockEndTurnOfCurrentWorldArmy();
    PlayerManager::getInstance().removePlayerFromGame(0);
    REQUIRE(mine.getPlayerID() == -1);

    endGame(game_thread);
}