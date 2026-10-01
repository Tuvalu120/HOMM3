/**
 * @file TestTown.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Town.
 *
 */

#include "CatchConfig.hpp"
#include "TestUtilities.hpp"

#include "GameObjects/Town.h"
#include "GameObjects/Army.h"
#include "GameObjects/TownInteractiveObject.h"
#include "Managers/WorldManager.h"
#include "Managers/BattleManager.h"
#include "Managers/PlayerManager.h"
#include "Data/Constants.hpp"
#include "Data/GameData.hpp"

using namespace std;
using namespace constants;
using namespace game_data;

TEST_CASE("Town Constructor and Getters Test", "[Town]"){

    WorldManager::getInstance().createWorldMap();

    TownData town_data = getTownData(CASTLE);
    int init_x_pos = 5;
    int init_y_pos = 5;
    Town town(town_data, CASTLE, 1, 5, 5);
    REQUIRE(town.getTownID() == 1);
    REQUIRE(town.getInitPosOfTown() == make_pair(init_x_pos, init_y_pos));
    vector<pair<int, int>> expected_pos ={{0,0}, {0,1}, {1,0}, {1,1}};
    for(const auto& pos : expected_pos){
        REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(init_x_pos + pos.first, init_y_pos + pos.second).hasInteractiveObject());
        REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(init_x_pos + pos.first, init_y_pos + pos.second).getObjectOnTile().getInteractiveObjectType() == PART_OF_TOWN);
    }
    REQUIRE_NOTHROW(town.getBuilding(DWELLINGS));
    REQUIRE_NOTHROW(town.getArmy().getPlayerID());

    REQUIRE(town.getPlayerID() == -1);
    REQUIRE(calculateArmyLevel(town.getArmy()) == NEUTRAL_TOWN_ARMY_LEVEL);
    REQUIRE(town.getArmy().getArmyHeroPtr() == nullptr);
    REQUIRE(town.getArmy().getPlayerID() == -1);

    REQUIRE(town.getStatistic(GOLD_GENERATED) == GOLD_PROVIDED_BY_HALL.at(0));
    town.setStatistic(GOLD_GENERATED, 750);
    REQUIRE(town.getStatistic(GOLD_GENERATED) == 750);

    auto troops_available_to_buy = town.getTownResources().troops_available_to_buy_;
    for(const auto& troop_data: troops_available_to_buy){
        REQUIRE(troop_data.second == 0);
    }
    REQUIRE(town.getTownResources().spells_in_mage_guild_.size() == 0);
    REQUIRE(town.getTownResources().heroes_in_tavern_ == NO_HEROES_IN_TAVERN);

    ///////

}

TEST_CASE("Town Interaction Test", "[Town]"){

    thread game_thread = playMockGame();

    pair<int, int> first_player_town_pos = enterFriendlyTown();

    REQUIRE(GraphicsManager::getInstance().getCurrentScene() == TOWN);
    REQUIRE_NOTHROW(TownManager::getInstance().getCurrentTown());

    setWorldDecisionVars(LEAVE_TOWN);
    mockEndTurnOfCurrentWorldArmy();

    WorldArmy& current_world_army = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy();
    WorldManager::getInstance().getWorldMap().moveObject(current_world_army, first_player_town_pos.first + 1, first_player_town_pos.second + 1);

    setWorldDecisionVars(first_player_town_pos.first + 1, first_player_town_pos.second);

    REQUIRE(GraphicsManager::getInstance().getCurrentScene() == BATTLE);
    REQUIRE(BattleManager::getInstance().getCurrentBattle().isBattlefieldInitialized() == true);
    REQUIRE(BattleManager::getInstance().getCurrentBattle().isBattleInProgress() == true);

    endBattle(-1);
    endGame(game_thread);
}

TEST_CASE("Town Ownership Test", "[Town]"){

    thread game_thread = playMockGame();

    WorldArmy& current_world_army = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy();

    int enemy_player_x_pos = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0).getXPos();
    int enemy_player_y_pos = PlayerManager::getInstance().getPlayerByID(1).getArmyByID(0).getYPos();

    WorldManager::getInstance().getWorldMap().moveObject(current_world_army, enemy_player_x_pos - 1, enemy_player_y_pos + 2);

    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(enemy_player_x_pos - 1, enemy_player_y_pos).hasInteractiveObject());
    InteractiveObject* object_to_check = &WorldManager::getInstance().getWorldMap().getTileAt(enemy_player_x_pos - 1, enemy_player_y_pos).getObjectOnTile();
    const Town& event_town = static_cast<TownInteractiveObject*>(object_to_check)->getTownOfObject();

    REQUIRE(event_town.getPlayerID() == 1);
    setWorldDecisionVars(enemy_player_x_pos - 2, enemy_player_y_pos + 1);

    REQUIRE(GraphicsManager::getInstance().getCurrentScene() == BATTLE);
    REQUIRE(BattleManager::getInstance().getCurrentBattle().isBattleInProgress() == true);
    REQUIRE(BattleManager::getInstance().getCurrentBattle().isBattlefieldInitialized() == true);

    endBattle(current_world_army.getPlayerID());

    REQUIRE(GraphicsManager::getInstance().getCurrentScene() == WORLD);
    REQUIRE(PlayerManager::getInstance().isPlayerActive(1) == false);
    REQUIRE(TownManager::getInstance().getNOfTownsOfPlayer(0) == 2);
    REQUIRE(event_town.getPlayerID() == 0);

    endGame(game_thread);
}

TEST_CASE("Town onNewDay and onNewWeek Test", "[Town]"){
    initializeMockGame();

    int player_id = 0;
    int player_town_id = -1;
    ResourceArray old_player_resources, new_player_resources;;

    for(int i=0; i<NUMBER_OF_TOWNS; ++i){
        if(TownManager::getInstance().getTownWithID(i).getPlayerID() == player_id){
            player_town_id = i;
            break;
        }
    }

    REQUIRE(player_town_id != -1);
    old_player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();

    skipDay();

    new_player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();

    REQUIRE(new_player_resources.at(GOLD) == old_player_resources.at(GOLD) + GOLD_PROVIDED_BY_HALL.at(0));

    for(int i=1; i<N_RESOURCES; ++i){
        REQUIRE(new_player_resources.at(i) == old_player_resources.at(i));
    }

    Town& player_town = TownManager::getInstance().getTownWithID(player_town_id);
    givePlayerMaxResources(PlayerManager::getInstance().getPlayerByID(player_id));

    REQUIRE(player_town.getBuilding(TAVERN).attemptToLevelUp() == true);
    REQUIRE(player_town.getBuilding(HALL).attemptToLevelUp() == true);
    REQUIRE(player_town.getBuilding(DWELLINGS).attemptToLevelUp() == true);

    old_player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();

    skipDay();

    new_player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();

    REQUIRE(new_player_resources.at(GOLD) == old_player_resources.at(GOLD) + GOLD_PROVIDED_BY_HALL.at(1));

    auto heroes_in_tavern = player_town.getTownResources().heroes_in_tavern_;
    int troops_available_to_buy = player_town.getTownResources().troops_available_to_buy_.at(0).second;

    skipWeek();

    REQUIRE(player_town.getTownResources().heroes_in_tavern_ != heroes_in_tavern);
    REQUIRE(troops_available_to_buy + getTroopData(player_town.getTownResources().troops_available_to_buy_.at(0).first).troop_battle_data.growth == player_town.getTownResources().troops_available_to_buy_.at(0).second);

}

TEST_CASE("Player Upgrading Town Decision Test", "[Town]"){

    thread game_thread = playMockGame();

    int player_id = 0;

    enterFriendlyTown();
    REQUIRE(GraphicsManager::getInstance().getCurrentScene() == TOWN);

    setWorldDecisionVars(UPGRADE_DWELLINGS);
    REQUIRE(TownManager::getInstance().getCurrentTown().getBuilding(DWELLINGS).getBuildingLevel() == 0);

    givePlayerMaxResources(PlayerManager::getInstance().getPlayerByID(player_id));
    setWorldDecisionVars(UPGRADE_DWELLINGS);
    REQUIRE(TownManager::getInstance().getCurrentTown().getBuilding(DWELLINGS).getBuildingLevel() == 1);

    setWorldDecisionVars(UPGRADE_TAVERN);
    REQUIRE(TownManager::getInstance().getCurrentTown().getBuilding(TAVERN).getBuildingLevel() == 1);

    setWorldDecisionVars(UPGRADE_TAVERN);
    REQUIRE(TownManager::getInstance().getCurrentTown().getBuilding(TAVERN).getBuildingLevel() == 1);

    setWorldDecisionVars(LEAVE_TOWN);
    endGame(game_thread);

}

TEST_CASE("Player Buying Troops In Town Decision Test", "[Town]"){

    thread game_thread = playMockGame();

    int player_id = 0;

    enterFriendlyTown();

    WorldArmy& current_army = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy();
    int different_troop_types = current_army.getArmyStacks().size();
    Town& event_town = TownManager::getInstance().getCurrentTown();

    setWorldDecisionVars(BUY_TROOP_1);
    ResourceArray player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();

    REQUIRE(accumulate(player_resources.begin(), player_resources.end(), 0) == 0);

    givePlayerMaxResources(PlayerManager::getInstance().getPlayerByID(player_id));
    ResourceArray old_player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();

    setWorldDecisionVars(BUY_TROOP_1);
    REQUIRE(PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources() == old_player_resources);

    REQUIRE(event_town.getBuilding(DWELLINGS).attemptToLevelUp() == true);
    REQUIRE(event_town.getBuilding(DWELLINGS).attemptToLevelUp() == true);

    old_player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();


    auto army_troops = current_army.getArmyStacks();
    vector<int> quantities;
    vector<TroopType> troop_types;
    for(const auto& troop : army_troops){
        troop_types.push_back(troop->getTroopType());
        quantities.push_back(troop->getQuantity());
    }

    int troop_to_buy_index = 1;

    int init_troop_available_to_buy = event_town.getTownResources().troops_available_to_buy_.at(troop_to_buy_index).second;

    setWorldDecisionVars(BUY_TROOP_2);

    REQUIRE(PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources().at(GOLD) ==
        old_player_resources.at(GOLD) - event_town.getTownData().cost_of_buying_troop.at(troop_to_buy_index).at(GOLD));

    REQUIRE(event_town.getTownResources().troops_available_to_buy_.at(troop_to_buy_index).second == init_troop_available_to_buy - 1);

    TroopType event_troop_type = event_town.getTownResources().troops_available_to_buy_.at(troop_to_buy_index).first;

    bool was_troop_in_army_before = find(troop_types.begin(), troop_types.end(), event_troop_type) != troop_types.end();

    if(was_troop_in_army_before){
        int troop_index = find(troop_types.begin(), troop_types.end(), event_troop_type) - troop_types.begin();
        REQUIRE(troop_index != static_cast<int>(troop_types.size()));
        REQUIRE(quantities.at(troop_index) == army_troops.at(troop_index)->getQuantity() + 1);
        REQUIRE(static_cast<int>(current_army.getArmyStacks().size()) == different_troop_types);
    }
    else{
        REQUIRE(static_cast<int>(current_army.getArmyStacks().size()) == different_troop_types + 1);
        REQUIRE(current_army.getArmyStacks().back()->getTroopType() == event_troop_type);
        REQUIRE(current_army.getArmyStacks().back()->getQuantity() == 1);
    }

    old_player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();
    setWorldDecisionVars(BUY_TROOP_3);
    REQUIRE(old_player_resources == PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources());

    setWorldDecisionVars(LEAVE_TOWN);

    endGame(game_thread);
}

TEST_CASE("Player Buying Hero In Town Decision Test", "[Town]"){

    thread game_thread = playMockGame();

    int player_id = 0;

    pauseGame(100);

    enterFriendlyTown();

    Town& event_town = TownManager::getInstance().getCurrentTown();

    setWorldDecisionVars(BUY_HERO_1);
    ResourceArray player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();

    REQUIRE(accumulate(player_resources.begin(), player_resources.end(), 0) == 0);

    givePlayerMaxResources(PlayerManager::getInstance().getPlayerByID(player_id));
    ResourceArray old_player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();

    setWorldDecisionVars(BUY_HERO_1);
    REQUIRE(PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources() == old_player_resources);

    REQUIRE(event_town.getBuilding(TAVERN).attemptToLevelUp() == true);

    old_player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();

    int hero_to_buy_index = PlayerManager::getInstance().getPlayerByID(player_id).getArmyByID(0).getArmyHero().getHeroType() == event_town.getTownResources().heroes_in_tavern_.at(0) ? 0 : 1;
    HeroType expected_hero_type = event_town.getTownResources().heroes_in_tavern_.at(hero_to_buy_index);
    TownDecision decision = hero_to_buy_index == 0 ? BUY_HERO_1 : BUY_HERO_2;

    setWorldDecisionVars(decision);

    REQUIRE(PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources().at(GOLD) ==
        old_player_resources.at(GOLD) - COST_OF_BUYING_HERO.at(GOLD));

    REQUIRE(event_town.getTownResources().heroes_in_tavern_.at(hero_to_buy_index == INVALID_HERO));
    REQUIRE(PlayerManager::getInstance().getPlayerByID(player_id).getArmies().at(1) != nullptr);
    REQUIRE(PlayerManager::getInstance().getPlayerByID(player_id).getArmyByID(1).getArmyHero().getHeroType() == expected_hero_type);

    old_player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();

    setWorldDecisionVars(decision);

    REQUIRE(PlayerManager::getInstance().getPlayerByID(player_id).getArmies().at(2) == nullptr);
    auto heroes_in_tavern = event_town.getTownResources().heroes_in_tavern_;
    int n_empty_slots = count(heroes_in_tavern.begin(), heroes_in_tavern.end(), INVALID_HERO);
    REQUIRE(n_empty_slots == 1);
    REQUIRE(PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources() == old_player_resources);

    setWorldDecisionVars(LEAVE_TOWN);

    endGame(game_thread);
}

TEST_CASE("Player Buying Spell In Town Decision Test", "[Town]"){

    thread game_thread = playMockGame();

    int player_id = 0;

    enterFriendlyTown();

    WorldArmy& event_army = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy();
    Town& event_town = TownManager::getInstance().getCurrentTown();
    REQUIRE(event_town.getTownResources().spells_in_mage_guild_.size() == 0);

    givePlayerMaxResources(PlayerManager::getInstance().getPlayerByID(player_id));
    REQUIRE(event_town.getBuilding(MAGE_GUILD).attemptToLevelUp() == true);

    REQUIRE(event_town.getTownResources().spells_in_mage_guild_.size() == static_cast<size_t>(MAGE_GUILD_SPELLS_ADDED_ON_LEVEL_UP.at(0).second));

    int spell_to_buy_index = event_army.getArmyHero().getSpellAt(0).getSpellType() == event_town.getTownResources().spells_in_mage_guild_.at(0).first ? 1 : 0;
    ResourceArray player_resources = PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources();
    SpellType expected_spell_type = event_town.getTownResources().spells_in_mage_guild_.at(spell_to_buy_index).first;

    TownDecision decision = spell_to_buy_index == 0 ? BUY_SPELL_1 : BUY_SPELL_2;
    setWorldDecisionVars(decision);

    REQUIRE(event_town.getTownResources().spells_in_mage_guild_.size() == static_cast<size_t>(MAGE_GUILD_SPELLS_ADDED_ON_LEVEL_UP.at(0).second - 1));
    REQUIRE_NOTHROW(event_army.getArmyHero().getSpellAt(1));
    REQUIRE(event_army.getArmyHero().getSpellAt(1).getSpellType() == expected_spell_type);

    REQUIRE(PlayerManager::getInstance().getPlayerByID(player_id).getPlayerResources().at(GOLD) ==
        player_resources.at(GOLD) - COST_OF_BUYING_SPELL.at(GOLD));

    vector<SpellType> hero_spells;
    hero_spells.push_back(event_army.getArmyHero().getSpellAt(0).getSpellType());
    hero_spells.push_back(event_army.getArmyHero().getSpellAt(1).getSpellType());

    REQUIRE(event_town.getBuilding(MAGE_GUILD).attemptToLevelUp() == true);
    REQUIRE(event_town.getTownResources().spells_in_mage_guild_.size() ==
        static_cast<size_t>(MAGE_GUILD_SPELLS_ADDED_ON_LEVEL_UP.at(0).second + MAGE_GUILD_SPELLS_ADDED_ON_LEVEL_UP.at(1).second - 1));

    bool flag_is_spell_in_both_lists = false; //Hero spell list and Mage Guild spell list

    for(size_t i=MAGE_GUILD_SPELLS_ADDED_ON_LEVEL_UP.at(0).second-1; i<event_town.getTownResources().spells_in_mage_guild_.size(); ++i){
        if(find(hero_spells.begin(), hero_spells.end(), event_town.getTownResources().spells_in_mage_guild_.at(i).first) != hero_spells.end()){
            flag_is_spell_in_both_lists = true;
            spell_to_buy_index = i;
            break;
        }
    }

    if(!flag_is_spell_in_both_lists){
        spell_to_buy_index = MAGE_GUILD_SPELLS_ADDED_ON_LEVEL_UP.at(0).second - 1;
    }

    decision = static_cast<TownDecision>(BUY_SPELL_1 + spell_to_buy_index);
    expected_spell_type = event_town.getTownResources().spells_in_mage_guild_.at(spell_to_buy_index).first;
    setWorldDecisionVars(decision);

    if(flag_is_spell_in_both_lists){
        int expected_number_of_spells = 2;
        bool flag_was_spell_in_army = false;
        REQUIRE(event_army.getArmyHero().getNOfSpells() == expected_number_of_spells);
        for(int i = 0; i<expected_number_of_spells; ++i){
            if(event_army.getArmyHero().getSpellAt(i).getSpellType() == expected_spell_type){
                flag_was_spell_in_army = true;
                REQUIRE(event_army.getArmyHero().getSpellAt(i).getLevelOfSpell() == 2);
                break;
            }
        }
        REQUIRE(flag_was_spell_in_army == true);
    }
    else{
        REQUIRE(event_army.getArmyHero().getSpellAt(2).getSpellType() == expected_spell_type);
        REQUIRE(event_army.getArmyHero().getSpellAt(2).getLevelOfSpell() == 2);
    }

    size_t expected_number_of_spells_in_mage_guild = MAGE_GUILD_SPELLS_ADDED_ON_LEVEL_UP.at(0).second + MAGE_GUILD_SPELLS_ADDED_ON_LEVEL_UP.at(1).second - 2; // Two spells were bought during this test
    REQUIRE(event_town.getTownResources().spells_in_mage_guild_.size() == expected_number_of_spells_in_mage_guild);

    setWorldDecisionVars(LEAVE_TOWN);

    endGame(game_thread);
}
