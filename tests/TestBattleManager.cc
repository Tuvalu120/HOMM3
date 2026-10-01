/**
 * @file TestBattleManager.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class BattleManager.
 *
 */

#include <memory>
#include <thread>

#include "CatchConfig.hpp"
#include "TestUtilities.hpp"
#include "Managers/PlayerManager.h"
#include "Managers/BattleManager.h"
#include "GameObjects/Army.h"
#include "GameObjects/Stack.h"
#include "GameObjects/Hero.h"
#include "GameObjects/Town.h"
#include "Miscellaneous/GameExceptions.hpp"

using namespace std;

TEST_CASE("BattleManager Initialization And Mock Battle Test", "[BattleManager]"){

    initializeMockGame(1234);
    REQUIRE(BattleManager::getInstance().isBattleInProgress() == false);
    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);
    int result;
    // battle() sometimes awaits input from the player, so we need to run it in a separate thread and mock the input
    thread battle_thread([&](){
        result = BattleManager::getInstance().battle(attacker, defender);
    });
    pauseGame(100);
    REQUIRE(BattleManager::getInstance().getCurrentBattle().isBattlefieldInitialized() == true);
    REQUIRE(BattleManager::getInstance().isBattleInProgress() == true);
    REQUIRE(&BattleManager::getInstance().getAttackingArmy() == &attacker);
    REQUIRE(&BattleManager::getInstance().getDefendingArmy() == &defender);
    int current_player_id = BattleManager::getInstance().getCurrentBattle().getCurrentPlayerID();

    int winning_player_id = current_player_id == 0 ? 0 : 1;
    int losing_player_id = current_player_id == 0 ? 1 : 0;
    int expected_result =  current_player_id == 0 ? 1 : -1;

    endBattleThread(battle_thread, winning_player_id);

    REQUIRE(result == expected_result);
    REQUIRE(PlayerManager::getInstance().isPlayerActive(losing_player_id) == false);
    REQUIRE(BattleManager::getInstance().isBattleInProgress() == false);

}

TEST_CASE("Troop Perform Battle Action Walking Test", "[BattleManager][Troop]"){

    initializeMockGame();
    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);
    int result;
    // battle() sometimes awaits input from the player, so we need to run it in a separate thread and mock the input
    thread battle_thread([&](){
        result = BattleManager::getInstance().battle(attacker, defender);
    });
    pauseGame(100);
    BattleObject* current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    while (current_battle_object->getBattleObjectType() != STACK){
        BattleManager::getInstance().getCurrentBattle().endTurnPrematurely();
        current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    }
    const auto& army_troops = BattleManager::getInstance().getCurrentBattle().getCurrentPlayer().getArmyByID(0).getArmyStacks();
    Stack* current_troop = nullptr;
    for (const auto& troop: army_troops){
        if( troop.get() == current_battle_object){
            current_troop = troop.get();
            break;
        }
    }
    REQUIRE(current_troop != nullptr);


    int old_x_pos = current_troop->getXPos();
    int old_y_pos = current_troop->getYPos();
    setBattleDecisionVars(0, 0); //Cannot move troop to (0,0) - nothing should change
    REQUIRE(current_troop->getXPos() == old_x_pos);
    REQUIRE(current_troop->getYPos() == old_y_pos);
    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() == current_troop);

    int new_x_pos = old_x_pos + 2;
    int new_y_pos = old_y_pos + 3;

    setBattleDecisionVars(new_x_pos, new_y_pos);
    REQUIRE(current_troop->getXPos() == new_x_pos);
    REQUIRE(current_troop->getYPos() == new_y_pos);
    endBattleThread(battle_thread, 0);

}

TEST_CASE("Troop Perform Battle Action Attacking Test", "[BattleManager][Troop]"){

    initializeMockGame();
    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);

    thread battle_thread([&](){
        BattleManager::getInstance().battle(attacker, defender);
    });
    pauseGame(100);

    BattleObject* current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    while (current_battle_object->getBattleObjectType() != STACK){
        BattleManager::getInstance().getCurrentBattle().endTurnPrematurely();
        current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    }

    const auto& army_troops = BattleManager::getInstance().getCurrentBattle().getCurrentPlayer().getArmyByID(0).getArmyStacks();
    Stack* current_troop = nullptr;
    for (const auto& troop: army_troops){
        if( troop.get() == current_battle_object){
            current_troop = troop.get();
            break;
        }
    }
    REQUIRE(current_troop != nullptr);

    Stack* another_friendly_troop = nullptr;
    for (const auto& troop: army_troops){
        if(troop.get() != current_troop){
            another_friendly_troop = troop.get();
            break;
        }
    }
    REQUIRE(another_friendly_troop != nullptr);

    const auto& enemy_troops = BattleManager::getInstance().getCurrentBattle().getOppositeArmy(
        BattleManager::getInstance().getCurrentBattle().getCurrentPlayerID()).getArmyStacks();

    Stack* enemy_troop = enemy_troops.at(0).get();
    REQUIRE(enemy_troop != nullptr);

    int current_troop_x_pos = current_troop->getXPos();
    int current_troop_y_pos = current_troop->getYPos();

    int friendly_troop_x_pos = another_friendly_troop->getXPos();
    int friendly_troop_y_pos = another_friendly_troop->getYPos();

    int enemy_troop_x_pos = enemy_troop->getXPos();
    int enemy_troop_y_pos = enemy_troop->getYPos();
    int enemy_troop_health = enemy_troop->getCurrentKnownHealth();

    setBattleDecisionVars(friendly_troop_x_pos, friendly_troop_y_pos); //Cannot attack friendly troop - nothing should change
    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() == current_troop);

    REQUIRE(current_troop->getXPos() == current_troop_x_pos);
    REQUIRE(current_troop->getYPos() == current_troop_y_pos);
    REQUIRE(another_friendly_troop->getXPos() == friendly_troop_x_pos);
    REQUIRE(another_friendly_troop->getYPos() == friendly_troop_y_pos);

    current_troop->setMaxMoves(100);
    setBattleDecisionVars(enemy_troop_x_pos, enemy_troop_y_pos); //Attack enemy troop
    REQUIRE(enemy_troop_health > enemy_troop->getCurrentKnownHealth());
    REQUIRE(abs(current_troop->getXPos() - enemy_troop_x_pos) <= current_troop->getStatistic(RANGE));
    REQUIRE(abs(current_troop->getYPos() - enemy_troop_y_pos) <= current_troop->getStatistic(RANGE));
    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() != current_troop);
    endBattleThread(battle_thread, 0);

}

TEST_CASE("Troop Perform Battle Action Walking to Enemy Test", "[BattleManager][Troop]"){

    initializeMockGame();
    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);
    int result;

    thread battle_thread([&](){
        result = BattleManager::getInstance().battle(attacker, defender);
    });
    pauseGame(100);

    BattleObject* current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    while (current_battle_object->getBattleObjectType() != STACK){
        BattleManager::getInstance().getCurrentBattle().endTurnPrematurely();
        current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    }

    const auto& army_troops = BattleManager::getInstance().getCurrentBattle().getCurrentPlayer().getArmyByID(0).getArmyStacks();
    Stack* current_troop = nullptr;
    for (const auto& troop: army_troops){
        if( troop.get() == current_battle_object){
            current_troop = troop.get();
            break;
        }
    }
    REQUIRE(current_troop != nullptr);

    const auto& enemy_troops = BattleManager::getInstance().getCurrentBattle().getOppositeArmy(
        BattleManager::getInstance().getCurrentBattle().getCurrentPlayerID()).getArmyStacks();
    Stack* enemy_troop = enemy_troops.at(0).get();
    REQUIRE(enemy_troop != nullptr);

    int current_troop_x_pos = current_troop->getXPos();
    int current_troop_y_pos = current_troop->getYPos();

    int enemy_troop_x_pos = enemy_troop->getXPos();
    int enemy_troop_y_pos = enemy_troop->getYPos();
    int enemy_troop_health = enemy_troop->getCurrentKnownHealth();

    int init_distance = sqrt(pow(current_troop_x_pos - enemy_troop_x_pos, 2) + pow(current_troop_y_pos - enemy_troop_y_pos, 2));

    setBattleDecisionVars(enemy_troop_x_pos, enemy_troop_y_pos); // going (but not attacking) to enemy troop

    bool did_current_troop_move = (current_troop->getXPos() != current_troop_x_pos) || (current_troop->getYPos() != current_troop_y_pos);
    REQUIRE(did_current_troop_move == true);

    int new_distance = sqrt(pow(current_troop->getXPos() - enemy_troop_x_pos, 2) + pow(current_troop->getYPos() - enemy_troop_y_pos, 2));
    REQUIRE(new_distance < init_distance);
    REQUIRE(enemy_troop_health == enemy_troop->getCurrentKnownHealth());
    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() != current_troop);

    endBattleThread(battle_thread, 0);
}

TEST_CASE("Hero Perform Battle Action", "[BattleManager][Hero]"){

    initializeMockGame();
    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);
    int result;

    thread battle_thread([&](){
        result = BattleManager::getInstance().battle(attacker, defender);
    });
    pauseGame(100);

    BattleObject* current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    while (current_battle_object->getBattleObjectType() != HERO){
        setBattleDecisionVars(-2, -2);
        current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    }
    const Hero& army_hero = BattleManager::getInstance().getCurrentBattle().getCurrentPlayer().getArmyByID(0).getArmyHero();
    REQUIRE(&army_hero == current_battle_object);

    int spell_to_choose = 0;
    setBattleDecisionVars(spell_to_choose);
    REQUIRE(&army_hero == &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject()); //Spell chosen, but not casted yet
    REQUIRE(army_hero.getSpellChosenIndex() == spell_to_choose);
    Spell& chosen_spell = army_hero.getSpellAt(spell_to_choose);
    REQUIRE(chosen_spell.getSpellType() == game_data::getHeroData(army_hero.getHeroType()).init_spells.at(0));

    endBattleThread(battle_thread, 0);

}

TEST_CASE("Hero Perform Battle Action Throws Test", "[BattleManager][Hero]"){
    initializeMockGame();
    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);

    thread battle_thread([&](){
        REQUIRE_THROWS_AS(BattleManager::getInstance().battle(attacker, defender), UninitializedObjectError);
    });
    pauseGame(100);

    BattleObject* current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    while (current_battle_object->getBattleObjectType() != HERO){
        setBattleDecisionVars(-2, -2);
        current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    }
    setBattleDecisionVars(1); //Nothing should change - Hero has only one spell at index 0.

    endBattleThread(battle_thread, 0);
}
