/**
 * @file TestSpell.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Spell and classes, which inherits from this class.
 *
 */

#include "CatchConfig.hpp"
#include "TestUtilities.hpp"

#include "GameObjects/Spell.h"
#include "Managers/BattleManager.h"
#include "GameObjects/Army.h"
#include "Data/ObjectDataEnums.hpp"

using namespace std;
using namespace game_data;

TEST_CASE("Spell Factory and Initialization Test", "[Spell]"){

    unique_ptr<Spell> spell = Spell::generateSpell(MAGIC_ARROW);
    REQUIRE(spell->getSpellFamily() == SINGLE_TARGET_DAMAGE_SPELL);
    REQUIRE(dynamic_cast<SingleTargetDamageSpell*>(spell.get()) != nullptr); // Check if the spell is of the correct type

    REQUIRE(spell->getSpellType() == MAGIC_ARROW);
    REQUIRE(spell->getLevelOfSpell() == 1);
    REQUIRE(spell->getStatistic(BASE_DAMAGE) == 10);
    REQUIRE(spell->getStatistic(DAMAGE_PER_SPELL_POWER_POINT) == 10);
    REQUIRE(spell->getSpellName() == getSpellData(MAGIC_ARROW).spell_name);
    REQUIRE(spell->getSpellDescription() == getSpellData(MAGIC_ARROW).spell_description);

    spell = Spell::generateSpell(STONE_SKIN);
    REQUIRE(spell->getSpellFamily() == SINGLE_TARGET_BUFF_SPELL);
    REQUIRE(dynamic_cast<SingleTargetBuffSpell*>(spell.get()) != nullptr);

    spell = Spell::generateSpell(FIREBALL);
    REQUIRE(spell->getSpellFamily() == AREA_OF_EFFECT_DAMAGE_SPELL);
    REQUIRE(dynamic_cast<AreaOfEffectDamageSpell*>(spell.get()) != nullptr);

    spell = Spell::generateSpell(AREA_STONE_SKIN);
    REQUIRE(spell->getSpellFamily() == AREA_OF_EFFECT_BUFF_SPELL);
    REQUIRE(dynamic_cast<AreaOfEffectBuffSpell*>(spell.get()) != nullptr);


}

TEST_CASE("SingleTargetDamageSpell Casting Test", "[Spell][SingleTargetDamageSpell]"){
    initializeMockGame();
    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);

    thread battle_thread = setUpBattleForSpellTesting(attacker, defender, MAGIC_ARROW, 1);
    Hero* attacking_hero = attacker.getArmyHeroPtr().get();
    int spell_id = attacker.getArmyHero().hasSpellAt(1) ? 1 : 0;

    setBattleDecisionVars(spell_id);
    REQUIRE(attacking_hero->getSpellChosenIndex() == spell_id);

    Stack* friendly_troop = attacker.getArmyStacks().at(0).get();
    Stack* enemy_troop = defender.getArmyStacks().at(0).get();
    int init_mana = attacking_hero->getManaPoints();

    int friendly_troop_x_pos = friendly_troop->getXPos();
    int friendly_troop_y_pos = friendly_troop->getYPos();
    int init_friendly_troop_health = friendly_troop->getCurrentKnownHealth();

    int enemy_troop_x_pos = enemy_troop->getXPos();
    int enemy_troop_y_pos = enemy_troop->getYPos();
    int init_enemy_troop_health = enemy_troop->getCurrentKnownHealth();

    int empty_tile_x_pos = 0;
    int empty_tile_y_pos = 0;

    setBattleDecisionVars(friendly_troop_x_pos, friendly_troop_y_pos);
    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() == attacking_hero);
    REQUIRE(friendly_troop->getCurrentKnownHealth() == init_friendly_troop_health);
    REQUIRE(attacking_hero->getManaPoints() == init_mana);

    setBattleDecisionVars(spell_id);
    setBattleDecisionVars(empty_tile_x_pos, empty_tile_y_pos);
    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() == attacking_hero);
    REQUIRE(attacking_hero->getManaPoints() == init_mana);

    setBattleDecisionVars(spell_id);
    setBattleDecisionVars(enemy_troop_x_pos, enemy_troop_y_pos);
    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() != attacking_hero);
    REQUIRE(enemy_troop->getCurrentKnownHealth() < init_enemy_troop_health);
    REQUIRE(attacking_hero->getManaPoints() == static_cast<int>(init_mana - getSpellData(MAGIC_ARROW).spell_battle_data.cost.at(0)));

    endBattleThread(battle_thread, 0);

}

TEST_CASE("SingleTargetBuffSpell Casting Test", "[Spell][SingleTargetBuffSpell]"){
    initializeMockGame();
    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);

    thread battle_thread = setUpBattleForSpellTesting(attacker, defender, STONE_SKIN, 1);
    Hero* attacking_hero = attacker.getArmyHeroPtr().get();
    int spell_id = attacker.getArmyHero().hasSpellAt(1) ? 1 : 0;

    setBattleDecisionVars(spell_id);

    Stack* friendly_troop = attacker.getArmyStacks().at(0).get();
    Stack* enemy_troop = defender.getArmyStacks().at(0).get();

    int friendly_troop_x_pos = friendly_troop->getXPos();
    int friendly_troop_y_pos = friendly_troop->getYPos();
    const auto init_friendly_troop_statistics = friendly_troop->getStatistics();

    int enemy_troop_x_pos = enemy_troop->getXPos();
    int enemy_troop_y_pos = enemy_troop->getYPos();
    const auto init_enemy_troop_statistics = enemy_troop->getStatistics();

    int empty_tile_x_pos = 0;
    int empty_tile_y_pos = 0;

    setBattleDecisionVars(enemy_troop_x_pos, enemy_troop_y_pos);
    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() == attacking_hero);
    REQUIRE(enemy_troop->getStatistics() == init_enemy_troop_statistics);

    setBattleDecisionVars(spell_id);
    setBattleDecisionVars(empty_tile_x_pos, empty_tile_y_pos);
    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() == attacking_hero);

    setBattleDecisionVars(spell_id);
    setBattleDecisionVars(friendly_troop_x_pos, friendly_troop_y_pos);
    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() != attacking_hero);
    REQUIRE(friendly_troop->getStatistics() != init_friendly_troop_statistics);


    endBattleThread(battle_thread, 0);

}

TEST_CASE("AreaOfEffectDamageSpell Casting Test", "[Spell][AreaOfEffectDamageSpell]"){
    initializeMockGame();
    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);

    Hero* attacking_hero = attacker.getArmyHeroPtr().get();
    attacking_hero->addStatistic(KNOWLEDGE, 10);
    thread battle_thread = setUpBattleForSpellTesting(attacker, defender, FIREBALL, 1);

    int spell_id = attacker.getArmyHero().hasSpellAt(1) ? 1 : 0;


    setBattleDecisionVars(spell_id);

    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() == attacking_hero);

    Stack* friendly_troop = attacker.getArmyStacks().at(0).get();
    Stack* enemy_troop = defender.getArmyStacks().at(0).get();

    int target_x_pos = 2;
    int target_y_pos = 3;

    moveTroopsForTest(friendly_troop, enemy_troop, target_x_pos, target_y_pos);

    int friendly_troop_health = friendly_troop->getCurrentKnownHealth();
    int enemy_troop_health = enemy_troop->getCurrentKnownHealth();

    setBattleDecisionVars(target_x_pos, target_y_pos);

    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() != attacking_hero);
    REQUIRE(enemy_troop->getCurrentKnownHealth() < enemy_troop_health);
    REQUIRE(friendly_troop->getCurrentKnownHealth() == friendly_troop_health);

    endBattleThread(battle_thread, 0);

}

TEST_CASE("AreaOfEffectBuffSpell Casting Test", "[Spell][AreaOfEffectBuffSpell]"){
    initializeMockGame();
    WorldArmy& attacker = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    WorldArmy& defender = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);

    thread battle_thread = setUpBattleForSpellTesting(attacker, defender, AREA_STONE_SKIN, 1);
    Hero* attacking_hero = attacker.getArmyHeroPtr().get();
    int spell_id = attacker.getArmyHero().hasSpellAt(1) ? 1 : 0;

    setBattleDecisionVars(spell_id);

    Stack* friendly_troop = attacker.getArmyStacks().at(0).get();
    Stack* enemy_troop = defender.getArmyStacks().at(0).get();

    int target_x_pos = 2;
    int target_y_pos = 3;

    BattleManager::getInstance().getCurrentBattle().getBattlefield().moveObject(*friendly_troop, target_x_pos-1, target_y_pos);
    BattleManager::getInstance().getCurrentBattle().getBattlefield().moveObject(*enemy_troop, target_x_pos+1, target_y_pos);

    const auto friendly_troop_statistics = friendly_troop->getStatistics();
    const auto enemy_troop_statistics = enemy_troop->getStatistics();

    setBattleDecisionVars(target_x_pos, target_y_pos);

    REQUIRE(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() != attacking_hero);
    REQUIRE(enemy_troop->getStatistics() == enemy_troop_statistics);
    REQUIRE(friendly_troop->getStatistics() != friendly_troop_statistics);

    endBattleThread(battle_thread, 0);

}

TEST_CASE("Spell Level Impact Test", "[Spell]") {
    //Test to check if the level of the spell affects its behaviour

    initializeMockGame();

    PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().addSpell(MAGIC_ARROW, 1);
    int spell_id = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().hasSpellAt(1) ? 1 : 0;

    thread battle_thread_1 = setUpBattleForSpellTesting(PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0),
        PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0), MAGIC_ARROW, 1);

    Stack* enemy_troop = BattleManager::getInstance().getDefendingArmy().getArmyStacks().at(0).get();
    int enemy_troop_x_pos = enemy_troop->getXPos();
    int enemy_troop_y_pos = enemy_troop->getYPos();
    int enemy_base_health = enemy_troop->getCurrentKnownHealth();

    setBattleDecisionVars(spell_id);
    setBattleDecisionVars(enemy_troop_x_pos, enemy_troop_y_pos);
    int damage_dealt_at_level_one = enemy_base_health - enemy_troop->getCurrentKnownHealth();

    endBattleThread(battle_thread_1, 0);

    ////////////////////

    initializeMockGame();

    PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().addSpell(MAGIC_ARROW, 3);
    spell_id = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().hasSpellAt(1) ? 1 : 0;

    thread battle_thread_2 = setUpBattleForSpellTesting(PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0),
        PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0), MAGIC_ARROW, 1);

    enemy_troop = BattleManager::getInstance().getDefendingArmy().getArmyStacks().at(0).get();
    enemy_troop_x_pos = enemy_troop->getXPos();
    enemy_troop_y_pos = enemy_troop->getYPos();
    enemy_base_health = enemy_troop->getCurrentKnownHealth();

    setBattleDecisionVars(spell_id);
    setBattleDecisionVars(enemy_troop_x_pos, enemy_troop_y_pos);
    int damage_dealt_at_level_three = enemy_base_health - enemy_troop->getCurrentKnownHealth();

    endBattleThread(battle_thread_2, 0);

    REQUIRE(damage_dealt_at_level_three > damage_dealt_at_level_one);
}
