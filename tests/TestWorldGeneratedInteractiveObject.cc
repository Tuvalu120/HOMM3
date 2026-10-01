/**
 * @file TestWorldGeneratedInteractiveObject.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class WorldGeneratedInteractiveObject and classes, which inherits from this class.
 *
 */

#include "CatchConfig.hpp"
#include "Data/ObjectDataEnums.hpp"
#include "TestUtilities.hpp"

#include "GameObjects/WorldObjects/Mine.h"
#include "GameObjects/WorldObjects/WorldResource.h"
#include "GameObjects/WorldObjects/WorldNeutralTroops.h"
#include "GameObjects/WorldObjects/Scholar.h"
#include "GameObjects/WorldObjects/TreasureChest.h"

using namespace std;
using namespace game_data;
using namespace constants;

TEST_CASE("Mine Resources Test", "[Mine][WorldGeneratedInteractiveObject]"){

    initializeMockGame();

    thread game_thread([&]{
        GameManager::getInstance().play();
    });
    pauseGame(100);

    int player_id = 0;

    WorldArmy& army = PlayerManager::getInstance().getPlayerByID(player_id).getArmyByID(0);
    Player& current_player = PlayerManager::getInstance().getPlayerByID(player_id);

    pair<int, int> mine_pos = findAnyObjectOfType(MINE);
    Mine& event_object = static_cast<Mine&>(WorldManager::getInstance().getWorldMap().getTileAt(mine_pos.first, mine_pos.second).getObjectOnTile());
    REQUIRE(event_object.getPlayerID() == -1);

    WorldManager::getInstance().getWorldMap().moveObject(army, mine_pos.first+1, mine_pos.second+1);
    setWorldDecisionVars(mine_pos.first+1, mine_pos.second);

    REQUIRE(BattleManager::getInstance().isBattleInProgress() == true);
    REQUIRE(BattleManager::getInstance().getDefendingArmy().getPlayerID() == -1);
    REQUIRE(calculateArmyLevel(BattleManager::getInstance().getDefendingArmy()) == MINE_ARMY_LEVEL);

    endBattle(player_id);

    REQUIRE(BattleManager::getInstance().isBattleInProgress() == false);
    REQUIRE(event_object.getPlayerID() == 0);

    Resource mine_resource_type = event_object.getResourceType();
    REQUIRE(current_player.getPlayerResources().at(mine_resource_type) == 0);

    skipDay();
    ResourceArray player_resource_array = current_player.getPlayerResources();
    int extra_resources = mine_resource_type == GOLD ? GOLD_PROVIDED_BY_HALL.at(0) : 0;
    REQUIRE(player_resource_array.at(mine_resource_type) == MINE_RESOURCE_QUANTITY.at(mine_resource_type) + extra_resources);
    REQUIRE(accumulate(player_resource_array.begin(), player_resource_array.end(),0U) == MINE_RESOURCE_QUANTITY.at(mine_resource_type) + GOLD_PROVIDED_BY_HALL.at(0));

    endGame(game_thread);

}

TEST_CASE("Mine Player Management Test", "[Mine][PlayerManager]"){
    initializeMockGame();

    thread game_thread([&]{
        GameManager::getInstance().play();
    });
    pauseGame(100);

    WorldArmy& army_0 = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);

    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 0);

    pair<int, int> mine_pos = findAnyObjectOfType(MINE);
    Mine& event_object = static_cast<Mine&>(WorldManager::getInstance().getWorldMap().getTileAt(mine_pos.first, mine_pos.second).getObjectOnTile());
    REQUIRE(event_object.getPlayerID() == -1);

    WorldManager::getInstance().getWorldMap().moveObject(army_0, mine_pos.first+1, mine_pos.second+1);
    setWorldDecisionVars(mine_pos.first+1, mine_pos.second);

    endBattle(0);

    REQUIRE(event_object.getPlayerID() == 0);

    setWorldDecisionVars(mine_pos.first+1, mine_pos.second+1);

    REQUIRE(BattleManager::getInstance().isBattleInProgress() == false);
    REQUIRE(event_object.getPlayerID() == 0);

    mockEndTurnOfCurrentWorldArmy();

    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 1);
    WorldArmy& army_1 = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0);

    TownManager& manager_t = TownManager::getInstance();

    WorldManager::getInstance().getWorldMap().moveObject(army_1, mine_pos.first-1, mine_pos.second+1);

    setWorldDecisionVars(mine_pos.first-1, mine_pos.second);

    manager_t.getNOfTownsOfPlayer(0);

    REQUIRE(BattleManager::getInstance().getCurrentBattle().isBattleInProgress() == true);
    REQUIRE(BattleManager::getInstance().getAttackingArmy().getPlayerID() == 1);
    REQUIRE(BattleManager::getInstance().getDefendingArmy().getPlayerID() == -1);
    REQUIRE(calculateArmyLevel(BattleManager::getInstance().getDefendingArmy()) == MINE_ARMY_LEVEL);

    endBattle(-1);
    REQUIRE(event_object.getPlayerID() == 0);
    REQUIRE(PlayerManager::getInstance().getNOfActivePlayers() == 3);
    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 2);

    PlayerManager::getInstance().removePlayerFromGame(0);
    REQUIRE(event_object.getPlayerID() == -1);

    WorldArmy& army_2 = PlayerManager::getInstance().getPlayerByID(2).getArmyByID(0);
    WorldManager::getInstance().getWorldMap().moveObject(army_2, mine_pos.first-1, mine_pos.second+1);

    setWorldDecisionVars(mine_pos.first-1, mine_pos.second);
    REQUIRE(calculateArmyLevel(BattleManager::getInstance().getDefendingArmy()) == MINE_ARMY_LEVEL);

    endBattle(2);
    auto& manager = WorldManager::getInstance();
    manager.getCurrentWeek();
    endGame(game_thread);
}

TEST_CASE("WorldResource Test", "[WorldResource]"){

    initializeMockGame();

    thread game_thread([&]{
        GameManager::getInstance().play();
    });
    pauseGame(100);

    WorldArmy& army = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    Player& player = PlayerManager::getInstance().getPlayerByID(0);

    int event_x_pos = 15;
    int event_y_pos = 15;
    Resource resource_type = GOLD;
    unsigned int quantity = 250;

    shared_ptr<WorldResource> resource = make_shared<WorldResource>("Texture", resource_type, quantity);

    setPositionOfWorldGeneratedInteractiveObject(resource, event_x_pos, event_y_pos);
    REQUIRE(resource->getResourceType() == resource_type);
    REQUIRE(resource->getQuantity() == quantity);
    REQUIRE(resource.use_count() == 2);

    WorldManager::getInstance().getWorldMap().moveObject(army, event_x_pos-1, event_y_pos-1);
    setWorldDecisionVars(event_x_pos-1, event_y_pos);

    REQUIRE(player.getPlayerResources().at(resource_type) == quantity);
    ResourceArray player_resource_array = player.getPlayerResources();
    REQUIRE(accumulate(player_resource_array.begin(), player_resource_array.end(), 0U) == quantity);
    REQUIRE(resource.use_count() == 1);
    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(event_x_pos, event_y_pos).hasInteractiveObject() == false);

    REQUIRE_NOTHROW(setWorldDecisionVars(event_x_pos, event_y_pos));
    endGame(game_thread);

    WorldResource default_resource;
    unsigned int expected_min_quantity = WORLD_RESOURCE_QUANTITY_PROBABILITIES.at(default_resource.getResourceType()).first;
    unsigned int expected_max_quantity = WORLD_RESOURCE_QUANTITY_PROBABILITIES.at(default_resource.getResourceType()).second;
    REQUIRE_NOTHROW(default_resource.getResourceType());
    REQUIRE(default_resource.getQuantity() >= expected_min_quantity);
    REQUIRE(default_resource.getQuantity() <= expected_max_quantity);
}

TEST_CASE("WorldNeutralTroops Initialization Test", "[WorldNeutralTroops]"){

    initializeMockGame();
    WorldNeutralTroops neutral_troops;
    int troops_level = 3;
    neutral_troops.setUpArmy(troops_level);
    REQUIRE(calculateArmyLevel(neutral_troops.getObjectArmy()) == troops_level);
    REQUIRE(neutral_troops.getObjectArmy().getPlayerID() == -1);
    REQUIRE(neutral_troops.getInteractiveObjectType() == NEUTRAL_TROOPS);
    auto& troops = neutral_troops.getObjectArmy().getArmyStacks();
    Stack& troop = *troops.at(0);
    REQUIRE(static_cast<int>(troops.size()) == 1);
    REQUIRE(troop.getQuantity() == (PATTERN_ARMY_POWER.at(troops_level-1) / troop.getStatistic(TROOP_POWER))+1);

    // World initialization

    pair<int, int> world_generated_neutral_troops_pos = findAnyObjectOfType(NEUTRAL_TROOPS);
    WorldNeutralTroops& world_generated_neutral_troops = static_cast<WorldNeutralTroops&>(WorldManager::getInstance().getWorldMap().getTileAt(
        world_generated_neutral_troops_pos.first, world_generated_neutral_troops_pos.second).getObjectOnTile());
    REQUIRE(world_generated_neutral_troops.getObjectArmy().getPlayerID() == -1);
    REQUIRE(calculateArmyLevel(world_generated_neutral_troops.getObjectArmy()) == 1);

}

TEST_CASE("WorldNeutralTroops Interaction Do Nothing Test", "[WorldNeutralTroops][Decision][DecisionManager]"){

    initializeMockGame();

    thread game_thread([&]{
        GameManager::getInstance().play();
    });
    pauseGame(100);

    WorldArmy& army = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    int init_army_power = army.getArmyPower();

    int troops_level = 1;
    int event_x_pos = 15;
    int event_y_pos = 15;

    auto neutral_troops = setUpWorldNeutralTroops(troops_level, event_x_pos, event_y_pos);
    WorldManager::getInstance().getWorldMap().moveObject(army, event_x_pos, event_y_pos+1);

    setWorldDecisionVars(event_x_pos+1, event_y_pos+1);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionType() == NEUTRAL_TROOPS_DECISION);

    makeDecision(DO_NOTHING);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == false);
    REQUIRE(neutral_troops.use_count() == 2);
    REQUIRE(army.getArmyPower() == init_army_power);

    setWorldDecisionVars(event_x_pos, event_y_pos+1);
    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == false);
    REQUIRE(neutral_troops.use_count() == 2);

    endGame(game_thread);
}

TEST_CASE("WorldNeutralTroops Buy Troops Test", "[WorldNeutralTroops][Decision][DecisionManager]"){

    initializeMockGame();

    thread game_thread([&]{
        GameManager::getInstance().play();
    });
    pauseGame(100);

    WorldArmy& army = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);
    Player& player = PlayerManager::getInstance().getPlayerByID(0);
    int init_army_power = army.getArmyPower();

    int troops_level = 1;
    int event_x_pos = 15;
    int event_y_pos = 15;

    shared_ptr<WorldNeutralTroops> neutral_troops = setUpWorldNeutralTroops(troops_level, event_x_pos, event_y_pos);

    WorldManager::getInstance().getWorldMap().moveObject(army, event_x_pos, event_y_pos+1);

    setWorldDecisionVars(event_x_pos+1, event_y_pos+1);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getNOfDecisionOptions() == 2); //As when player does not have enough resources, it cannot buy them

    for(const auto& decision_option: DecisionManager::getInstance().getCurrentDecision().getDecisionOptions()){
        REQUIRE(decision_option->getDecisionOptionType() != ADD_TROOPS);
    }

    makeDecision(DO_NOTHING);
    REQUIRE(army.getArmyPower() == init_army_power);

    skipDay();
    givePlayerMaxResources(player);
    unsigned int init_gold = player.getPlayerResources().at(GOLD);

    setWorldDecisionVars(event_x_pos, event_y_pos+1);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getNOfDecisionOptions() == 3);

    vector<TroopType> init_army_troop_types;
    vector<int> init_army_troop_quantities;

    for(const auto& troop: army.getArmyStacks()){
        init_army_troop_types.push_back(troop->getTroopType());
        init_army_troop_quantities.push_back(troop->getQuantity());
    }

    Stack& event_troop = *neutral_troops->getObjectArmy().getArmyStacks().at(0);
    TroopType event_troop_type = event_troop.getTroopType();
    int event_troop_quantity = event_troop.getQuantity();

    makeDecision(ADD_TROOPS);
    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == false);
    REQUIRE(neutral_troops.use_count() == 1); //Removed from the map
    REQUIRE(player.getPlayerResources().at(GOLD) == init_gold - getTroopData(event_troop_type).troop_battle_data.troop_power * event_troop.getQuantity());

    bool flag_was_troop_already_in_army = find(init_army_troop_types.begin(), init_army_troop_types.end(), event_troop_type) != init_army_troop_types.end();

    if(flag_was_troop_already_in_army){
        for(size_t i=0; i<init_army_troop_types.size(); ++i){
            if(init_army_troop_types.at(i) == event_troop_type){
                REQUIRE(army.getArmyStacks().at(i)->getQuantity() == init_army_troop_quantities.at(i) + event_troop_quantity);
            }
        }
    }
    else{
        size_t troop_vector_size = army.getArmyStacks().size();
        REQUIRE(troop_vector_size == init_army_troop_types.size() + 1);
        REQUIRE(army.getArmyStacks().at(troop_vector_size-1)->getTroopType() == event_troop_type);
        REQUIRE(army.getArmyStacks().at(troop_vector_size-1)->getQuantity() == event_troop.getQuantity());
    }

    endGame(game_thread);
}

TEST_CASE("WorldNeutralTroops Battle Test", "[WorldNeutralTroops][Decision][DecisionManager]"){
    initializeMockGame();

    thread game_thread([&]{
        GameManager::getInstance().play();
    });
    pauseGame(100);

    int player_id = 0;
    WorldArmy& army = PlayerManager::getInstance().getPlayerByID(player_id).getArmyByID(0);
    int init_hero_experience_points = army.getArmyHero().getHeroExperiencePoints();

    int troops_level = 1;
    int event_x_pos = 15;
    int event_y_pos = 15;

    shared_ptr<WorldNeutralTroops> neutral_troops = setUpWorldNeutralTroops(troops_level, event_x_pos, event_y_pos);

    WorldManager::getInstance().getWorldMap().moveObject(army, event_x_pos, event_y_pos+1);

    setWorldDecisionVars(event_x_pos+1, event_y_pos+1);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    makeDecision(PARTICIPATE_IN_BATTLE);
    REQUIRE(BattleManager::getInstance().getCurrentBattle().isBattleInProgress() == true);
    REQUIRE(BattleManager::getInstance().getAttackingArmy().getPlayerID() == 0);
    REQUIRE(BattleManager::getInstance().getDefendingArmy().getPlayerID() == -1);
    REQUIRE(BattleManager::getInstance().getDefendingArmy().getArmyStacks().size() == 1);
    int neutral_army_experience_points = neutral_troops->getObjectArmy().getArmyExperienceReward();

    endBattle(player_id);
    REQUIRE(army.getArmyHero().getHeroExperiencePoints() == init_hero_experience_points + neutral_army_experience_points);
    REQUIRE(neutral_troops.use_count() == 1); //Removed from the map

    neutral_troops = setUpWorldNeutralTroops(troops_level, event_x_pos, event_y_pos);

    setWorldDecisionVars(event_x_pos, event_y_pos+1);
    makeDecision(PARTICIPATE_IN_BATTLE);

    REQUIRE(BattleManager::getInstance().getCurrentBattle().isBattleInProgress() == true);

    endBattle(-1);
    REQUIRE(WorldManager::getInstance().getCurrentPlayerID() == 1);
    REQUIRE(neutral_troops.use_count() == 2); //Troops are still on the map

    endGame(game_thread);

}

TEST_CASE("Scholar Test", "[Scholar][Decision][DecisionManager]"){

    initializeMockGame();

    thread game_thread([&]{
        GameManager::getInstance().play();
    });
    pauseGame(100);

    int object_x_pos = 15;
    int object_y_pos = 15;

    shared_ptr<Scholar> scholar = shared_ptr<Scholar>(new Scholar());
    pair<int, int> event_pos = setUpWorldGeneratedInteractiveObject(scholar, object_x_pos, object_y_pos);

    REQUIRE(PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().getSpellAt(0).getLevelOfSpell() == 1);

    setWorldDecisionVars(event_pos);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionType() == SCHOLAR_DECISION);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getNOfDecisionOptions() == 3);

    int increase_level_of_spell_decision_option_id = 0;

    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionOptionAt(increase_level_of_spell_decision_option_id).getDecisionOptionType() == ADD_SPELL);

    setWorldDecisionVars(increase_level_of_spell_decision_option_id);
    REQUIRE(PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().getSpellAt(0).getLevelOfSpell() == 2);
    REQUIRE(PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().getNOfSpells() == 1);
    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(object_x_pos, object_y_pos).hasInteractiveObject() == false);

    //////

    scholar = shared_ptr<Scholar>(new Scholar());
    event_pos = setUpWorldGeneratedInteractiveObject(scholar, object_x_pos, object_y_pos);

    setWorldDecisionVars(event_pos);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionType() == SCHOLAR_DECISION);

    int add_new_spell_decision_option_id = 1;
    setWorldDecisionVars(add_new_spell_decision_option_id);

    REQUIRE(PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().getNOfSpells() == 2);
    REQUIRE(PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().getSpellAt(1).getLevelOfSpell() == 1);
    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(object_x_pos, object_y_pos).hasInteractiveObject() == false);

    /////

    scholar = shared_ptr<Scholar>(new Scholar());
    event_pos = setUpWorldGeneratedInteractiveObject(scholar, object_x_pos, object_y_pos);

    setWorldDecisionVars(event_pos);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    int add_statistic_decision_option_id = 2;

    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionType() == SCHOLAR_DECISION);
    AddHeroStatisticDecisionOption& add_statistic_decision_option = static_cast<AddHeroStatisticDecisionOption&>(DecisionManager::getInstance().getCurrentDecision().getDecisionOptionAt(add_statistic_decision_option_id));

    HeroStat stat_to_increase = add_statistic_decision_option.getHeroStat();
    int quantity_to_increase = add_statistic_decision_option.getQuantity();

    int init_stat_value = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().getStatistic(stat_to_increase);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getNOfDecisionOptions() == 3);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionOptionAt(add_statistic_decision_option_id).getDecisionOptionType() == ADD_HERO_STAT);
    setWorldDecisionVars(add_statistic_decision_option_id);

    REQUIRE(PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero().getStatistic(stat_to_increase) == init_stat_value + quantity_to_increase);
    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(object_x_pos, object_y_pos).hasInteractiveObject() == false);

    endGame(game_thread);
}

TEST_CASE("Scholar Edge Case Test", "[Scholar]"){
    initializeMockGame();

    thread game_thread([&]{
        GameManager::getInstance().play();
    });
    pauseGame(100);

    int object_x_pos = 15;
    int object_y_pos = 15;

    shared_ptr<Scholar> scholar = shared_ptr<Scholar>(new Scholar());
    pair<int, int> event_pos = setUpWorldGeneratedInteractiveObject(scholar, object_x_pos, object_y_pos);

    WorldArmy& army = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);

    army.getArmyHero().addSpell(STONE_SKIN, 1);
    army.getArmyHero().addSpell(BLOODLUST, 1);
    army.getArmyHero().addSpell(AREA_STONE_SKIN, 1);
    army.getArmyHero().addSpell(AREA_BLOODLUST, 1);

    REQUIRE(army.getArmyHero().getNOfSpells() == MAX_NUMBER_OF_SPELLS);

    WorldManager::getInstance().getWorldMap().moveObject(army, object_x_pos+1, object_y_pos+1);
    setWorldDecisionVars(event_pos);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionType() == SCHOLAR_DECISION);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getNOfDecisionOptions() == 2); //No new spell can be added

    makeDecision(ADD_HERO_STAT);

    ///

    scholar = shared_ptr<Scholar>(new Scholar());
    setPositionOfWorldGeneratedInteractiveObject(scholar, object_x_pos, object_y_pos);

    for(auto& spell: army.getArmyHero().getHeroSpells()){
        spell->setLevelOfSpell(MAX_LEVEL_OF_SPELL);
    }

    WorldManager::getInstance().getWorldMap().moveObject(army, object_x_pos+1, object_y_pos+1);
    setWorldDecisionVars(object_x_pos+1, object_y_pos);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionType() == SCHOLAR_DECISION);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getNOfDecisionOptions() == 1); //No spell can be upgraded

    makeDecision(ADD_HERO_STAT);

    endGame(game_thread);

}

TEST_CASE("Treasure Chest Test", "[TreasureChest][Decision][DecisionManager]"){

    initializeMockGame();

    thread game_thread([&]{
        GameManager::getInstance().play();
    });
    pauseGame(100);

    int object_x_pos = 15;
    int object_y_pos = 15;

    shared_ptr<TreasureChest> treasure_chest = shared_ptr<TreasureChest>(new TreasureChest());
    pair<int, int> event_pos = setUpWorldGeneratedInteractiveObject(treasure_chest, object_x_pos, object_y_pos);

    int init_gold = PlayerManager::getInstance().getPlayerByID(0).getPlayerResources().at(GOLD);

    setWorldDecisionVars(event_pos);

    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionType() == CHEST_DECISION);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getNOfDecisionOptions() == 2);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionOptionAt(0).getDecisionOptionType() == ADD_RESOURCES);
    REQUIRE(DecisionManager::getInstance().getCurrentDecision().getDecisionOptionAt(1).getDecisionOptionType() == ADD_EXPERIENCE_POINTS);

    int expected_add_resources_decision_option_id = 0;
    AddResourceDecisionOption& decision_option = static_cast<AddResourceDecisionOption&>(DecisionManager::getInstance().getCurrentDecision().getDecisionOptionAt(expected_add_resources_decision_option_id));
    REQUIRE(decision_option.getResourceType() == GOLD);
    unsigned int expected_gold_increase = decision_option.getResourceQuantity();

    makeDecision(ADD_RESOURCES);

    int current_gold = PlayerManager::getInstance().getPlayerByID(0).getPlayerResources().at(GOLD);
    unsigned int gold_increase = current_gold - init_gold;

    REQUIRE(expected_gold_increase == gold_increase);

    ///

    treasure_chest = shared_ptr<TreasureChest>(new TreasureChest());
    event_pos = setUpWorldGeneratedInteractiveObject(treasure_chest, object_x_pos, object_y_pos);

    int add_experience_points_decision_option_id = 1;

    setWorldDecisionVars(event_pos);
    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == true);
    AddExperiencePointsDecisionOption& add_experience_points_decision_option = static_cast<AddExperiencePointsDecisionOption&>(DecisionManager::getInstance().getCurrentDecision().getDecisionOptionAt(add_experience_points_decision_option_id));
    int experience_points = add_experience_points_decision_option.getExperiencePoints();
    REQUIRE(find(EXPERIENCE_AMOUNT_IN_CHESTS.begin(), EXPERIENCE_AMOUNT_IN_CHESTS.end(), experience_points) != EXPERIENCE_AMOUNT_IN_CHESTS.end());

    makeDecision(ADD_EXPERIENCE_POINTS);
    if(experience_points >= EXPERIENCE_POINTS_TO_LEVEL_UP.at(1)){
        makeDecision(ADD_HERO_STAT);
    }

    Hero& event_hero = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0).getArmyHero();
    REQUIRE((event_hero.getHeroExperiencePoints() != 0 || event_hero.getHeroLevel() != 1));

    endGame(game_thread);
}