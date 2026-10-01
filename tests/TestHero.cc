/**
 * @file TestHero.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Hero.
 *
 */

#include "CatchConfig.hpp"
#include "TestUtilities.hpp"

#include "GameObjects/Hero.h"
#include "Data/GameData.hpp"
#include "GameObjects/Spell.h"
#include "Data/ObjectDataEnums.hpp"

using namespace std;
using namespace game_data;
using namespace constants;

TEST_CASE("Hero Constructor Test", "[Hero]") {
    Hero hero(LOYNIS, 1);

    REQUIRE(hero.getHeroType() == HeroType::LOYNIS);
    REQUIRE(hero.getHeroName() == getHeroData(HeroType::LOYNIS).hero_name);
    REQUIRE(hero.getHeroLevel() == 1);
    REQUIRE(hero.getHeroExperiencePoints() == 0);
    REQUIRE(hero.getManaPoints() == 0);
    REQUIRE_THROWS_AS(Hero(LOYNIS, -1), std::exception);
}

TEST_CASE("Hero Spell methods Tests", "[Hero][Spell]"){
    Hero hero(HeroType::LOYNIS, 1);

    REQUIRE(hero.hasSpellAt(0) == false);
    REQUIRE(hero.canAddNewSpell() == true);

    REQUIRE(hero.addSpell(SpellType::FIREBALL, 1) == true);
    REQUIRE(hero.hasSpellAt(0) == true);
    REQUIRE(hero.getSpellAt(0).getSpellType() == SpellType::FIREBALL);
    REQUIRE(hero.getSpellAt(0).getLevelOfSpell() == 1);

    REQUIRE(hero.addSpell(SpellType::FIREBALL, 1) == false); // Same level spell
    REQUIRE(hero.addSpell(SpellType::FIREBALL, 2) == true);  // Upgrade spell level
    REQUIRE(hero.getSpellAt(0).getLevelOfSpell() == 2);

    REQUIRE(hero.addSpell(SpellType::ARMAGEDON, 1) == true);
    REQUIRE(hero.addSpell(SpellType::ARMAGEDON, 3) == true);
    REQUIRE(hero.getSpellAt(0).getLevelOfSpell() == 2);
    REQUIRE(hero.getSpellAt(1).getLevelOfSpell() == 3);

    REQUIRE(hero.addSpell(SpellType::BLOODLUST, 1) == true);
    REQUIRE(hero.canAddNewSpell() == true);

    REQUIRE(hero.addSpell(SpellType::AREA_BLOODLUST, 1) == true);
    REQUIRE(hero.canAddNewSpell() == false);

    REQUIRE(hero.addSpell(SpellType::STONE_SKIN, 1) == false);
    REQUIRE(hero.canAddNewSpell() == false);

}

TEST_CASE("Hero Attack and Mana Management Test", "[Hero]"){
    Hero hero(HeroType::LOYNIS, 1);
    Tile tile(0, 0, GRASS_TILE, 1, false);

    hero.addSpell(SpellType::FIREBALL, 1);
    hero.chooseSpell(0);
    hero.resetManaPoints();
    REQUIRE(hero.getManaPoints() != false);

    REQUIRE(hero.canCastSpellAt(0) == true);
    REQUIRE(hero.canCastSpellAt(1) == false);

}

TEST_CASE("Hero Statistics Management Test", "[Hero]"){
    Hero hero(HeroType::LOYNIS, 1);

    int initial_attack = hero.getStatistic(HERO_ATTACK);
    hero.addStatistic(HERO_ATTACK, 5);
    REQUIRE(hero.getStatistic(HERO_ATTACK) == initial_attack + 5);
}

TEST_CASE("Hero getDataText Test", "[Hero]") {
    Hero hero(HeroType::LOYNIS, 1);

    hero.resetManaPoints();
    auto dataText = hero.getDataText();
    REQUIRE(dataText == std::make_pair(std::vector<std::string>{"Loynis", "Atak", "Obrona", "Wiedza", "Moc Zaklęć", "Mana"}, std::vector<std::string>{"", "1", "0", "2", "2", "20"}));
}

TEST_CASE("Hero Level Up Test", "[Hero]"){

    thread game_thread = playMockGame();

    Hero& event_hero = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero();
    int current_level = event_hero.getHeroLevel();
    int init_exp_points_to_give = EXPERIENCE_POINTS_TO_LEVEL_UP.at(current_level)-1;

    event_hero.addExperiencePoints(init_exp_points_to_give);
    REQUIRE(event_hero.getHeroLevel() == 1);
    REQUIRE(event_hero.getHeroExperiencePoints() == init_exp_points_to_give);
    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == false);

    thread adding_exp_points_thread([&]{
        event_hero.addExperiencePoints(1);
    });
    pauseGame(100);

    REQUIRE(event_hero.getHeroLevel() == 2);
    REQUIRE(event_hero.getHeroExperiencePoints() == 0);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionType() == LEVEL_UP_DECISION);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getNOfDecisionOptions() == 2);

    makeDecision(ADD_HERO_STAT);

    adding_exp_points_thread.join();
    endGame(game_thread);
}

TEST_CASE("Hero Leveling Up Multiple Levels Test", "[Hero]"){

    thread game_thread = playMockGame();

    Hero& event_hero = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero();
    int init_level = event_hero.getHeroLevel();

    // Level up by 2 levels
    int exp_points_to_give = EXPERIENCE_POINTS_TO_LEVEL_UP.at(init_level) + EXPERIENCE_POINTS_TO_LEVEL_UP.at(init_level+1);


    thread adding_exp_points_thread([&]{
        event_hero.addExperiencePoints(exp_points_to_give);
    });
    pauseGame(100);

    REQUIRE(event_hero.getHeroLevel() == 2); //Levels are added one by one, and another level up will be after selecting decision
    REQUIRE(event_hero.getHeroExperiencePoints() == EXPERIENCE_POINTS_TO_LEVEL_UP.at(2));

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionType() == LEVEL_UP_DECISION);

    makeDecision(ADD_HERO_STAT);

    REQUIRE(event_hero.getHeroLevel() == 3);
    REQUIRE(event_hero.getHeroExperiencePoints() == 0);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionType() == LEVEL_UP_DECISION);

    makeDecision(ADD_HERO_STAT);

    adding_exp_points_thread.join();
    endGame(game_thread);
}

TEST_CASE("Hero Max Level Test", "[Hero]"){

    thread game_thread = playMockGame();

    Hero& event_hero = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero();

    bool flag_is_adding_experience_finished = false;
    thread adding_exp_points_thread([&]{
        while(event_hero.getHeroLevel() < MAX_LEVEL_OF_HERO){
            event_hero.addExperiencePoints(EXPERIENCE_POINTS_TO_LEVEL_UP.at(event_hero.getHeroLevel()));
        }
        flag_is_adding_experience_finished = true;
    });
    pauseGame(100);

    while (!flag_is_adding_experience_finished){
        // Wait for the hero to reach max level
        makeDecision(ADD_HERO_STAT);
    }
    adding_exp_points_thread.join();

    REQUIRE(event_hero.getHeroLevel() == MAX_LEVEL_OF_HERO);
    REQUIRE_NOTHROW(event_hero.addExperiencePoints(99999));
    REQUIRE(event_hero.getHeroLevel() == MAX_LEVEL_OF_HERO);

    endGame(game_thread);
}