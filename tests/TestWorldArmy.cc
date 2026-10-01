/**
 * @file TestWorldArmy.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class WorldArmy.
 *
 */

#include "CatchConfig.hpp"

#include "GameObjects/WorldArmy.h"
#include "GameObjects/Hero.h"
#include "Managers/BattleManager.h"

using namespace std;
using namespace constants;


TEST_CASE("WorldArmy Constructor and Getters Test", "[WorldArmy]") {

    HeroType hero_id = LOYNIS;
    int player_id = 1;
    int init_pos_x = 10;
    int init_pos_y = 20;
    auto hero = make_shared<Hero>(hero_id, player_id);
    auto world_army = make_shared<WorldArmy>(hero, init_pos_x, init_pos_y);

    REQUIRE(world_army->getArmyHero().getHeroType() == hero_id);
    REQUIRE(world_army->getXPos() == init_pos_x);
    REQUIRE(world_army->getYPos() == init_pos_y);
    REQUIRE(world_army->getArmyHeroPtr() == hero);
    REQUIRE(world_army->getArmyHero().getStatistic(MAX_MOVES) == hero->getStatistic(MAX_MOVES));
    REQUIRE(world_army->getArmyHero().getCurrentTextureName() == hero->getCurrentTextureName());

}

TEST_CASE("WorldArmy generateArmy Test", "[WorldArmy]"){
    auto world_army = WorldArmy::generateArmy(1, LOYNIS, 0, 0);
    REQUIRE(world_army->getArmyHero().getHeroType() == LOYNIS);
    REQUIRE(world_army->getArmyStacks().size() == 3);
    REQUIRE_NOTHROW(world_army->getArmyHero().getSpellAt(0));
    REQUIRE_THROWS_AS(world_army->getArmyHero().getSpellAt(1), UninitializedObjectError);
}

TEST_CASE("WorldArmy Priority Test", "[WorldArmy]"){

    auto world_army =   WorldArmy::generateArmy(1, LOYNIS, 0, 0);
    auto world_army_1 = WorldArmy::generateArmy(1, XARFAX, 0, 0);
    auto world_army_2 = WorldArmy::generateArmy(2, LOYNIS, 0, 0);

    REQUIRE(world_army->getPriority(*world_army_1) == -1);
    REQUIRE(world_army->getPriority(*world_army_2) == ARMY_PRIORITY);

}

TEST_CASE("WorldArmy getBattleObjectsOfArmy Test", "[WorldArmy]"){
    auto world_army =   WorldArmy::generateArmy(1, LOYNIS, 0, 0);
    vector<shared_ptr<BattleObject>> battle_objects = world_army->getBattleObjectsOfArmy();
    REQUIRE(battle_objects.size() == 4);
    REQUIRE(battle_objects.at(0) == world_army->getArmyStacks().at(0));
    REQUIRE(battle_objects.at(3) == world_army->getArmyHeroPtr());
}
