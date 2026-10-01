/**
 * @file TestArmy.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Army.
 *
 */

#include "CatchConfig.hpp"

#include "GameObjects/Army.h"
#include "GameObjects/Stack.h"
#include "Managers/BattleManager.h"
#include "Data/GameData.hpp"

using namespace std;
using namespace game_data;

TEST_CASE("Army Constructors and Getters Tests", "[Army]"){
    Army army("texture", 10, 20, true);
    REQUIRE(army.getXPos() == 10);
    REQUIRE(army.getYPos() == 20);
    REQUIRE(army.isWorldArmy() == true);

    Army army_2(10, 20, false);
    REQUIRE(army_2.getCurrentTextureName() == "");
    REQUIRE(army_2.getXPos() == 10);
    REQUIRE(army_2.getYPos() == 20);
    REQUIRE(army_2.isWorldArmy() == false);
    REQUIRE(army_2.getArmyHeroPtr() == nullptr);
}

TEST_CASE("Army addStacks Test", "[Army]") {
    Army army(10, 20, false);
    REQUIRE(army.addTroops(PIKEMAN, 10) == true);
    REQUIRE(army.getArmyStacks().size() == 1);
    REQUIRE(army.getArmyStacks()[0]->getQuantity() == 10);

    REQUIRE(army.addTroops(PIKEMAN, 10) == true);
    REQUIRE(army.getArmyStacks().size() == 1);
    REQUIRE(army.getArmyStacks()[0]->getQuantity() == 20);

    REQUIRE(army.addTroops(ANGEL, 10) == true);
    REQUIRE(army.getArmyStacks().size() == 2);
    REQUIRE(army.getArmyStacks()[0]->getQuantity() == 20);
    REQUIRE(army.getArmyStacks()[0]->getTroopType() == PIKEMAN);
    REQUIRE(army.getArmyStacks()[1]->getQuantity() == 10);
    REQUIRE(army.getArmyStacks()[1]->getTroopType() == ANGEL);


    REQUIRE(army.addTroops(ARCHER, 10) == true);
    REQUIRE(army.addTroops(BEHOLDER, 10) == true);
    REQUIRE(army.addTroops(CAVALIER, 10) == true);
    REQUIRE(army.addTroops(CENTAUR, 10) == true);
    REQUIRE(army.addTroops(DENDROID_GUARD, 10) == true);
    REQUIRE(army.addTroops(DWARF, 10) == true);

    REQUIRE(army.addTroops(GREEN_DRAGON, 10) == false);

    REQUIRE(army.getArmyStacks()[0]->getQuantity() == 20);
    REQUIRE(army.getArmyStacks().size() == 8);
    for(int i=1; i<8; ++i){
        REQUIRE(army.getArmyStacks()[i]->getQuantity() == 10);
    };

}

TEST_CASE("Army removeStackFromArmy and isArmyActive Tests", "[Army]"){
    Army army(10, 20, false);
    army.addTroops(PIKEMAN, 10);
    army.addTroops(ANGEL, 10);
    Stack& stack = *army.getArmyStacks()[0];
    REQUIRE_NOTHROW(army.removeStackFromArmy(stack));
    REQUIRE(army.getArmyStacks().size() == 1);
    REQUIRE(army.getArmyStacks()[0]->getTroopType() == ANGEL);

    Stack& stack_2 = *army.getArmyStacks()[0];
    REQUIRE_NOTHROW(army.removeStackFromArmy(stack_2));
    REQUIRE(army.getArmyStacks().size() == 0);
    REQUIRE(army.isArmyActive() == false);

}

TEST_CASE("Army getArmyPower Test", "[Army]"){
    int quantity = 10;
    Army army(10, 20, false);
    army.addTroops(PIKEMAN, quantity);
    int power = army.getArmyPower();
    REQUIRE(power == getTroopData(PIKEMAN).troop_battle_data.troop_power * quantity);
}

TEST_CASE("Army generatePatternArmy with multiple stack types Test", "[Army]"){
    vector<TroopType> stack_types = {PIKEMAN, ARCHER};
    vector<int> probabilities = {50, 50};
    auto army = Army::generatePatternArmy(2, stack_types, probabilities);
    REQUIRE(army->getArmyPower() >= PATTERN_ARMY_POWER[1]);
    REQUIRE(army->getArmyStacks().size() > 0);
}

TEST_CASE("Army generatePatternArmy with single stack type Test", "[Army]"){
    auto army = Army::generatePatternArmy(1, PIKEMAN);
    REQUIRE(army->getArmyPower() > PATTERN_ARMY_POWER[0]);
}

TEST_CASE("Army setArmyExperienceReward Test", "[Army]"){
    Army army(10, 20, false);
    int quantity = 10;
    army.addTroops(PIKEMAN, quantity);
    army.setArmyExperienceReward();
    REQUIRE(army.getArmyExperienceReward() == quantity * getTroopData(PIKEMAN).troop_battle_data.base_health);
}

TEST_CASE("Army removeAllStacksFromArmy", "[Army]"){
    Army army(10, 20, false);
    army.addTroops(PIKEMAN, 10);
    army.addTroops(ANGEL, 10);
    army.removeAllStacksFromArmy();
    REQUIRE(army.getArmyStacks().size() == 0);
}

TEST_CASE("Army resetStacksData", "[Army][Stack]"){
    Army army(10, 20, false);
    army.addTroops(PIKEMAN, 10);
    Stack& stack = *army.getArmyStacks()[0];
    int baseAttack = stack.getStatistic(TROOP_ATTACK);
    stack.addStatistic(TROOP_ATTACK, 10);
    army.resetStacksData();
    REQUIRE(stack.getStatistic(TROOP_ATTACK) == baseAttack);
}

TEST_CASE("Army setBonusExperiencePoints", "[Army]") {
    Army army(10, 20, false);
    army.setBonusExperiencePoints(100);
    REQUIRE(army.getArmyExperienceReward() == 100);
    int quantity = 10;
    army.addTroops(PIKEMAN, quantity);
    army.setArmyExperienceReward();
    REQUIRE(army.getArmyExperienceReward() == 200);
}

TEST_CASE("Army toString Test", "[Army]") {
    Army army;
    REQUIRE(army.toString() == "Army of player: -1");
}
