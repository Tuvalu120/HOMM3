/**
 * @file TestStack.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Stack.
 *
 */

#include "CatchConfig.hpp"

#include "GameObjects/Stack.h"
#include "Managers/BattleManager.h"

using namespace std;
using namespace game_data;

TEST_CASE("Troop Constructor and Getters Test", "[Troop]"){
    TroopType troop_type = TroopType::PIKEMAN;
    int player_id = 1;
    int quantity = 10;

    Stack troop(troop_type, player_id, quantity);

    REQUIRE(troop.getTroopType() == troop_type);
    REQUIRE(troop.getQuantity() == quantity);
    REQUIRE(troop.getCurrentKnownHealth() == troop.getStatistic(BASE_HEALTH) * quantity);
}

TEST_CASE("Troop Interact Test", "[Troop]"){
    TroopType troop_type = TroopType::PIKEMAN;
    int player_id = 1;
    int quantity = 10;

    Stack troop1(troop_type, player_id, quantity);
    Stack troop2(troop_type, player_id, quantity);

    troop1.interact(troop2);

    REQUIRE(troop2.getCurrentKnownHealth() >= 70);
    REQUIRE(troop2.getCurrentKnownHealth() <= 90);
}


TEST_CASE("Troop Receive Damage Test", "[Troop]"){
    TroopType troop_type = TroopType::PIKEMAN;
    int player_id = 1;
    int quantity = 10;

    Stack troop(troop_type, player_id, quantity);
    int initial_health = troop.getCurrentKnownHealth();
    int damage = 5;

    troop.receiveDamage(damage);

    REQUIRE(troop.getCurrentKnownHealth() == initial_health);
    REQUIRE(troop.getQuantity() == 10);

    damage = 4;
    troop.receiveDamage(damage);
    REQUIRE(troop.getQuantity() == 10);

    damage = 1;
    troop.receiveDamage(damage);
    REQUIRE(troop.getCurrentKnownHealth() == initial_health * (quantity - 1) / quantity);
    REQUIRE(troop.getQuantity() == 9);

    troop.receiveDamage(damage);
    REQUIRE(troop.getCurrentKnownHealth() == initial_health * (quantity - 1) / quantity);
    REQUIRE(troop.getQuantity() == 9);

}

TEST_CASE("Troop Dying Test", "[Troop]"){
    TroopType troop_type = TroopType::PIKEMAN;
    int player_id = 1;
    int quantity = 10;
    Stack troop(troop_type, player_id, quantity);

    int damage = getTroopData(troop_type).troop_battle_data.base_health * quantity;
    troop.receiveDamage(damage);

    REQUIRE(troop.getQuantity() == 0);
    REQUIRE(troop.getFlagDestroyAfterInteraction() == true);

}

TEST_CASE("Troop Statistics Test", "[Troop]"){
    TroopType troop_type = TroopType::PIKEMAN;
    int player_id = 1;
    int quantity = 10;
    Stack troop(troop_type, player_id, quantity);

    TroopBattleData data = getTroopData(troop_type).troop_battle_data;
    REQUIRE(troop.getStatistics() == data.toArray());

    int base_attack = troop.getStatistic(TROOP_ATTACK);
    int attack_to_add = 10;

    troop.addStatistic(TROOP_ATTACK, attack_to_add);
    REQUIRE(troop.getStatistic(TROOP_ATTACK) == base_attack + attack_to_add);

    troop.resetTroopData();
    REQUIRE(troop.getStatistic(TROOP_ATTACK) == base_attack);

}

TEST_CASE("Troop Data Text Test", "[Troop]"){
    TroopType troop_type = TroopType::PIKEMAN;
    int player_id = 1;
    int quantity = 10;
    Stack troop(troop_type, player_id, quantity);

    vector<string> expected_names = {"Pikinier"};
    expected_names.insert(expected_names.end(), TROOP_DISPLAY_STAT_TEXT.begin(), TROOP_DISPLAY_STAT_TEXT.end());

    vector<string> expected_values = {"", "4", "5", "4", "1", "1", "3", "10"};
    pair<vector<string>, vector<string>> expected_pair = {expected_names, expected_values};

    REQUIRE(troop.getDataText() == expected_pair);

}

TEST_CASE("Troop Errors Test", "[Troop]"){

    REQUIRE_THROWS_AS(new Stack(PIKEMAN, 4, 10), invalid_argument);
    REQUIRE_THROWS_AS(new Stack(PIKEMAN, 0, -1), invalid_argument);
    REQUIRE_THROWS_AS(new Stack(PIKEMAN, 0, 0), invalid_argument);

}