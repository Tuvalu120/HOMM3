/**
 * @file TestPlayer.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Player.
 *
 */

#include "CatchConfig.hpp"

#include "GameObjects/Player.h"
#include "GameObjects/WorldArmy.h"
#include "Data/Constants.hpp"
#include "TestUtilities.hpp"

using namespace std;
using namespace constants;

TEST_CASE("Player Constructor and Getters Test", "[Player]"){
    int player_id = 0;
    Player player(player_id);
    REQUIRE(player.getPlayerID() == player_id);
    REQUIRE(player.getCurrentArmyID() == -1);
    REQUIRE(player.getNMaxArmies() == MAX_NUMBER_OF_ARMIES);
    REQUIRE(player.getNArmies() == 0);
    REQUIRE(player.getArmies().at(0) == nullptr);
    REQUIRE(player.getPlayerResources() == ResourceArray{0, 0, 0, 0, 0});
    REQUIRE(player.getExploredWorldTiles().empty());
    REQUIRE_NOTHROW(player.getPlayerController());
    REQUIRE(player.canAddNewArmy() == true);
    REQUIRE(player.isPlayerActive() == false);
    REQUIRE_THROWS_AS(player.getCurrentArmy(), invalid_argument);
}

TEST_CASE("Player Armies Management Test", "[Player]"){
    int player_id = 0;
    Player player(player_id);
    REQUIRE(player.canAddNewArmy() == true);
    REQUIRE(player.doesArmyExist(0) == false);

    auto army = player.addArmyToPlayer(LOYNIS, 0, 0);
    REQUIRE(army != nullptr);
    REQUIRE(player.getCurrentArmyID() == 0);
    REQUIRE(player.getNArmies() == 1);
    REQUIRE(player.doesArmyExist(0) == true);

    auto invalid_army = player.addArmyToPlayer(LOYNIS, 0, 0);
    REQUIRE(invalid_army == nullptr);
    REQUIRE_THROWS_AS(player.getArmyByID(1), invalid_argument);

    player.addArmyToPlayer(SANYA, 0, 0);
    player.addArmyToPlayer(XARFAX, 0, 0);
    player.addArmyToPlayer(HALON, 0, 0);

    REQUIRE(player.getNArmies() == 4);

    auto invalid_army_2 = player.addArmyToPlayer(ELLESHAR, 0, 0);
    REQUIRE(invalid_army_2 == nullptr);
    REQUIRE(player.getNArmies() == 4);
}

TEST_CASE("Player Resources Management Test", "[Player]") {
    int player_id = 0;
    Player player(player_id);
    ResourceArray resources = {100, 100, 100, 100, 100};

    player.addResources(resources);
    REQUIRE(player.getPlayerResources() == resources);

    ResourceArray cost = {50, 40, 30, 20, 10};
    REQUIRE(player.canAfford(cost) == true);
    REQUIRE(player.spendResources(cost) == true);

    ResourceArray remaining_resources = {50, 60, 70, 80, 90};
    REQUIRE(player.getPlayerResources() == remaining_resources);

    player.addResources(GOLD, 50);
    REQUIRE(player.getPlayerResources().at(GOLD) == 100);

    REQUIRE(player.spendResources(GOLD, 50) == true);
    REQUIRE(player.getPlayerResources().at(GOLD) == 50);

    REQUIRE(player.spendResources(GOLD, 100) == false);
    REQUIRE(player.getPlayerResources().at(GOLD) == 50);

    REQUIRE(player.spendResources(GOLD, 50) == true);
    REQUIRE(player.getPlayerResources().at(GOLD) == 0);
}

TEST_CASE("Player WorldArmy Management Test", "[Player][Army]"){

    thread game_thread = playMockGame();

    int player_id = 0;
    WorldArmy* first_army = &PlayerManager::getInstance().getPlayerByID(player_id).getCurrentArmy();
    Player& event_player = PlayerManager::getInstance().getPlayerByID(player_id);

    REQUIRE(event_player.getCurrentArmyID() == 0);
    REQUIRE(&event_player.getArmyByID(0) == first_army);
    REQUIRE(event_player.getArmyID(*first_army) == 0);

    HeroType new_army_hero_type = first_army->getArmyHero().getHeroType() == LOYNIS ? SANYA : LOYNIS;
    WorldArmy* second_army = PlayerManager::getInstance().getPlayerByID(player_id).addArmyToPlayer(new_army_hero_type, 0, 0).get();

    mockEndTurnOfCurrentWorldArmy();
    REQUIRE(PlayerManager::getInstance().getPlayerByID(player_id).getCurrentArmyID() == 1);

    event_player.removeArmy(0);
    REQUIRE(event_player.getNArmies() == 1);
    REQUIRE(event_player.getArmyID(*first_army) == -1);

    REQUIRE(event_player.getCurrentArmyID() == 0);
    REQUIRE(&event_player.getArmyByID(0) == second_army);
    REQUIRE(event_player.getArmyID(*second_army) == 0);

    mockEndTurnOfCurrentWorldArmy();

    event_player.removeArmy(0);
    REQUIRE(event_player.getNArmies() == 0);
    REQUIRE(event_player.isPlayerActive() == false);

    endGame(game_thread);
}

TEST_CASE("Player Tile Management Test", "[Player]"){

    thread game_thread = playMockGame();

    int player_id = 0;
    WorldArmy& current_army = PlayerManager::getInstance().getPlayerByID(player_id).getCurrentArmy();
    Player& current_player = PlayerManager::getInstance().getPlayerByID(player_id);

    REQUIRE(current_player.getExploredWorldTiles().size() > 0);

    int init_x_pos = current_army.getXPos();
    int init_y_pos = current_army.getYPos();

    int discover_distance = current_army.getArmyHero().getStatistic(DISCOVERY_RADIUS);
    int expected_discoverd_new_tile_x_pos = init_x_pos + discover_distance + 1;
    int expected_discoverd_new_tile_y_pos = init_y_pos;

    auto discovered_tiles = current_player.getExploredWorldTiles();
    pair<int, int> new_tile = make_pair(expected_discoverd_new_tile_x_pos, expected_discoverd_new_tile_y_pos);
    REQUIRE(find(discovered_tiles.begin(), discovered_tiles.end(), new_tile) == discovered_tiles.end());
    REQUIRE(current_player.hasTileBeenVisited(new_tile) == false);

    setWorldDecisionVars(init_x_pos+1, init_y_pos);
    discovered_tiles = current_player.getExploredWorldTiles();
    REQUIRE(find(discovered_tiles.begin(), discovered_tiles.end(), new_tile) != discovered_tiles.end());
    REQUIRE(current_player.hasTileBeenVisited(new_tile) == true);

    size_t init_size = discovered_tiles.size();
    setWorldDecisionVars(init_x_pos, init_y_pos);

    discovered_tiles = current_player.getExploredWorldTiles();
    REQUIRE(init_size == discovered_tiles.size());

    REQUIRE(GraphicsManager::getInstance().getCurrentScene() == TOWN);
    REQUIRE(init_size == discovered_tiles.size());

    HeroType hero_type = current_army.getArmyHero().getHeroType() == XARFAX ? LOYNIS : XARFAX;
    PlayerManager::getInstance().addArmyToPlayer(current_player.getPlayerID(), hero_type, 5, 5);

    discovered_tiles = current_player.getExploredWorldTiles();
    REQUIRE(discovered_tiles.size() > init_size);

    setWorldDecisionVars(LEAVE_TOWN);
    endGame(game_thread);

}
