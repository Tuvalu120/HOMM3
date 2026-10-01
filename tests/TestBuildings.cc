/**
 * @file TestBuildings.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Building and classes, which inherits from this class.
 *
 */

#include "CatchConfig.hpp"
#include "TestUtilities.hpp"

#include "GameObjects/Buildings/Hall.h"
#include "GameObjects/Buildings/Tavern.h"
#include "GameObjects/Buildings/Fortifications.h"
#include "GameObjects/Buildings/Dwellings.h"
#include "Data/GameData.hpp"
#include "Managers/WorldManager.h"
#include "Managers/PlayerManager.h"

using namespace std;
using namespace constants;
using namespace game_data;


TEST_CASE("Building Constructor and Getters Test", "[Building][Hall]"){
    WorldManager::getInstance().createWorldMap();
    int player_id = 0;
    Player player(player_id, "");

    auto town = generateMockTown(player_id, CASTLE, 5, 5);

    Hall hall(*town, getMockBuildingSprites(MAX_LEVEL_OF_HALL));

    REQUIRE(hall.getBuildingLevel() == 0);
    REQUIRE(hall.getMaxBuildingLevel() == MAX_LEVEL_OF_HALL);
    REQUIRE(hall.getCurrentTextureName() == "sprite_0");

}

TEST_CASE("Tavern And Building Level Up Test", "[Building][Tavern]"){
    initializeMockGame();
    int player_id = 0;
    Player& player = PlayerManager::getInstance().getPlayerByID(player_id);
    auto town = generateMockTown(player_id, CASTLE, 15, 15);

    for(size_t i=0; i<town->getTownResources().heroes_in_tavern_.size(); ++i){
        REQUIRE(town->getTownResources().heroes_in_tavern_.at(i) == INVALID_HERO);
    }

    Tavern tavern(*town, getMockBuildingSprites(MAX_LEVEL_OF_TAVERN));
    REQUIRE(tavern.getBuildingDescription() == "Karczma (Niewybudowana)");

    REQUIRE(tavern.attemptToLevelUp() == false);
    player.addResources(COST_OF_TAVERN_UPGRADE.at(0));
    REQUIRE(tavern.attemptToLevelUp() == true);

    const ResourceArray& player_resources = player.getPlayerResources();
    REQUIRE(accumulate(player_resources.begin(), player_resources.end(), 0) == 0);

    REQUIRE(tavern.getBuildingLevel() == 1);
    REQUIRE(tavern.getCurrentTextureName() == "sprite_1");
    REQUIRE(tavern.getBuildingDescription() == "Karczma");

    for(size_t i=0; i<town->getTownResources().heroes_in_tavern_.size(); ++i){
        REQUIRE(town->getTownResources().heroes_in_tavern_.at(i) != INVALID_HERO);
    }

    REQUIRE(tavern.attemptToLevelUp() == false);
    givePlayerMaxResources(player);
    const ResourceArray old_resource_array = player.getPlayerResources();
    REQUIRE(tavern.attemptToLevelUp() == false); //Tavern is already at max level
    REQUIRE(player.getPlayerResources() == old_resource_array); //Resources should not change

}


TEST_CASE("Hall Level Up Test", "[Building][Hall]"){
    initializeMockGame();
    int player_id = 0;
    Player& player = PlayerManager::getInstance().getPlayerByID(player_id);
    auto town = generateMockTown(player_id, CASTLE, 15, 15);

    Hall hall(*town, getMockBuildingSprites(MAX_LEVEL_OF_HALL));

    player.addResources(COST_OF_HALL_UPGRADE.at(0));
    ResourceArray old_resource_array = player.getPlayerResources();
    REQUIRE(hall.attemptToLevelUp() == false); //Need other buildings first
    REQUIRE(player.getPlayerResources() == old_resource_array);

    givePlayerMaxResources(player);
    REQUIRE(town->getBuilding(TAVERN).attemptToLevelUp() == true);

    REQUIRE(hall.attemptToLevelUp() == true);
    REQUIRE(hall.getBuildingLevel() == 1);
    REQUIRE(town->getStatistic(GOLD_GENERATED) == GOLD_PROVIDED_BY_HALL.at(1));

    REQUIRE(hall.attemptToLevelUp() == false); //Need other buildings first
    REQUIRE(town->getBuilding(MAGE_GUILD).attemptToLevelUp() == true);
    REQUIRE(hall.attemptToLevelUp() == true);

    REQUIRE(hall.getBuildingLevel() == 2);

    REQUIRE(town->getBuilding(FORTIFICATIONS).attemptToLevelUp() == true);
    REQUIRE(hall.attemptToLevelUp() == false); //Need Fortifications to be level 3

    for (int i=1; i<3; ++i){
        REQUIRE(town->getBuilding(FORTIFICATIONS).attemptToLevelUp() == true);
    }

    REQUIRE(hall.attemptToLevelUp() == true);
    REQUIRE(town->getStatistic(GOLD_GENERATED) == GOLD_PROVIDED_BY_HALL.at(3));

}

TEST_CASE("Fortifications Level Up Test", "[Building][Fortifications]"){
    initializeMockGame();
    int player_id = 0;
    Player& player = PlayerManager::getInstance().getPlayerByID(player_id);
    auto town = generateMockTown(player_id, CASTLE, 15, 15);

    Fortifications fortifications(*town, getMockBuildingSprites(MAX_LEVEL_OF_FORTIFICATIONS));
    REQUIRE(calculateArmyLevel(town->getArmy()) == TOWN_ARMY_LEVELS.at(0));

    givePlayerMaxResources(player);
    for(int i=1; i<=MAX_LEVEL_OF_FORTIFICATIONS; ++i){
        REQUIRE(town->getBuilding(FORTIFICATIONS).attemptToLevelUp() == true);
        REQUIRE(calculateArmyLevel(town->getArmy()) == TOWN_ARMY_LEVELS.at(i));
        REQUIRE(town->getStatistic(TROOP_GROWTH_MULTIPLIER) == CREATURE_GROWTH_MULTIPLIERS.at(i));
    }

}

TEST_CASE("Dwellings Level Up Test", "[Building][Dwellings]"){
    initializeMockGame();
    int player_id = 0;
    Player& player = PlayerManager::getInstance().getPlayerByID(player_id);
    auto town = generateMockTown(player_id, CASTLE, 15, 15);

    Dwellings dwellings(*town, getMockBuildingSprites(MAX_LEVEL_OF_DWELLINGS));
    givePlayerMaxResources(player);

    REQUIRE(dwellings.attemptToLevelUp() == true);
    int expected_growth = game_data::getTroopData(town->getTownData().town_troops.at(0)).troop_battle_data.growth;
    REQUIRE(town->getTownResources().troops_available_to_buy_.at(0).second == expected_growth);

    REQUIRE(town->getBuilding(FORTIFICATIONS).attemptToLevelUp() == true);
    REQUIRE(dwellings.attemptToLevelUp() == true);
    expected_growth = game_data::getTroopData(town->getTownData().town_troops.at(1)).troop_battle_data.growth * CREATURE_GROWTH_MULTIPLIERS.at(1) / 100;
    REQUIRE(town->getTownResources().troops_available_to_buy_.at(1).second == expected_growth);

    for(int i=2; i<MAX_LEVEL_OF_DWELLINGS; ++i){
        REQUIRE(dwellings.attemptToLevelUp() == true);
        REQUIRE(dwellings.getBuildingLevel() == i+1);
        REQUIRE(town->getTownResources().troops_available_to_buy_.at(i).second != 0);
        REQUIRE(dwellings.getBuildingDescription() == "Koszary (Poziom " + std::to_string(i+1) + ")");
    }

}
