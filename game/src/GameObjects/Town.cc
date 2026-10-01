/**
 * @file Town.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Town.
 *
 */


#include "GameObjects/Buildings/Hall.h"
#include "GameObjects/Buildings/Fortifications.h"
#include "GameObjects/Buildings/Tavern.h"
#include "GameObjects/Buildings/MageGuild.h"
#include "GameObjects/Buildings/Dwellings.h"
#include "GameObjects/Town.h"
#include "GameObjects/TownInteractiveObject.h"
#include "Data/Constants.hpp"
#include "Data/GameData.hpp"
#include "Data/ObjectDataEnums.hpp"
#include "Managers/TownManager.h"
#include "Managers/WorldManager.h"
#include "Managers/PlayerManager.h"
#include "Managers/BattleManager.h"
#include "Managers/GraphicsManager.h"

using namespace std;
using namespace game_data;
using namespace constants;

Town::Town(TownData town_data, const TownType town_type,
    const int town_id, const int init_x_pos, const int init_y_pos):
    OwnedByPlayer(),town_data_(town_data),town_id_(town_id),
    town_type_(town_type),init_x_pos_(init_x_pos),init_y_pos_(init_y_pos),
    flag_was_town_visited_this_turn_(false){

        generateBuildings();
        generateInteractiveObjects();
        resetArmy();
        initializeTownResources();
        town_resources_.heroes_in_tavern_ = NO_HEROES_IN_TAVERN;
    }

void Town::interactWithTown(){
    int event_player_id = WorldManager::getInstance().getCurrentPlayerID();
    if (event_player_id == getPlayerID()){
        // Player is owner of this town and will be visiting it.
        if (!flag_was_town_visited_this_turn_){
            flag_was_town_visited_this_turn_ = true;
            visitTown();
        }
        //else Player already visited the town this turn.
    }
    else{ //Player is not the owner of the town. Battle is initiated.
        int result = BattleManager::getInstance().battle(WorldManager::getInstance().getCurrentPlayer().getCurrentArmy(), getArmy());
        if (result == 1){
            setPlayerID(event_player_id);
        }
    }
}

void Town::visitTown(){
    onEnteringTown();
    flag_leaving_town_ = false;
    while (!flag_leaving_town_){
        PlayerManager::getInstance().getPlayerByID(getPlayerID()).getPlayerController().makeTownDecision();
    }
}

int Town::getTownID() const{
    return town_id_;
}

const pair<int, int> Town::getInitPosOfTown() const{
    return {init_x_pos_, init_y_pos_};
}

void Town::generateInteractiveObjects(){

    WorldManager::getInstance().getWorldMap().getTileAt(init_x_pos_  , init_y_pos_  ).addObjectOnTile(
        make_shared<TownInteractiveObject>(*this, getTownData().town_textures.at(0), init_x_pos_  , init_y_pos_  , 0));

    WorldManager::getInstance().getWorldMap().getTileAt(init_x_pos_+1, init_y_pos_  ).
        addObjectOnTile(make_shared<TownInteractiveObject>(*this, getTownData().town_textures.at(1), init_x_pos_+1, init_y_pos_  , 1));

    WorldManager::getInstance().getWorldMap().getTileAt(init_x_pos_  , init_y_pos_+1).
        addObjectOnTile(make_shared<TownInteractiveObject>(*this, getTownData().town_textures.at(2), init_x_pos_  , init_y_pos_+1, 3));

    WorldManager::getInstance().getWorldMap().getTileAt(init_x_pos_+1, init_y_pos_+1).
        addObjectOnTile(make_shared<TownInteractiveObject>(*this, getTownData().town_textures.at(3), init_x_pos_+1, init_y_pos_+1, 2));

}


pair<int, int> Town::getArmyStartingPos() const{
    pair<int, int> offset = {ARMY_STARTING_POS_OFFSET_X, ARMY_STARTING_POS_OFFSET_Y};
    if (!WorldManager::getInstance().getWorldMap().getTileAt(init_x_pos_ + offset.first, init_y_pos_ + offset.second).hasInteractiveObject()){
        return {init_x_pos_ + offset.first, init_y_pos_ + offset.second};
    }

    vector<pair<int, int>> directions = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    vector<int> n_steps_vec = {3, 3, 3, 2};
    offset.second += 1;
    int current_direction = 0;

    for (const int& n_steps: n_steps_vec){
        for(int i=0; i<n_steps; ++i){
            if (!WorldManager::getInstance().getWorldMap().getTileAt(init_x_pos_ + offset.first, init_y_pos_ + offset.second).hasInteractiveObject()){
                return {init_x_pos_ + offset.first, init_y_pos_ + offset.second};
            }
            offset.first += directions.at(current_direction).first;
            offset.second += directions.at(current_direction).second;
        }
        ++current_direction;
    }
    return {-1,-1};
}

void Town::addGoldToPlayer(){
    if(getPlayerID() != -1){
        PlayerManager::getInstance().getPlayerByID(getPlayerID()).addResources(GOLD, getStatistic(GOLD_GENERATED));
    }
}

void Town::onNewDay(){
    addGoldToPlayer();
}

void Town::onNewWeek(){
    resetHeroesInTavern();
    increaseTroopsAvailableToBuy();
}

Building& Town::getBuilding(BuildingType building) const{
    return *buildings_.at(building);
}

int Town::getStatistic(const TownStat stat) const{
    return statistics_.at(stat);
}
void Town::setStatistic(const TownStat stat, const int new_value){
    statistics_.at(stat) = new_value;
}

Army& Town::getArmy() const{
    return *town_army_;
}

void Town::resetArmy(){
    if (getPlayerID() == -1){
        town_army_ = Army::generatePatternArmy(NEUTRAL_TOWN_ARMY_LEVEL, getTownData().town_troops);
    }
    else{
        town_army_ = Army::generatePatternArmy(getStatistic(TOWN_ARMY_LEVEL), getTownData().town_troops);
    }
    town_army_->setBonusExperiencePoints(EXPERIENCE_BONUS_FROM_CAPTURING_TOWN);
}

void Town::setPlayerID(const int new_player_id){
    int old_player_id = getPlayerID();
    OwnedByPlayer::replacePlayerID(new_player_id);

    resetFlagWasTownVisitedThisTurn();
    resetArmy();

    if (old_player_id != -1 && new_player_id != -1){
        PlayerManager::getInstance().checkForEliminatedPlayer(old_player_id, false);
    }
}

const TownData& Town::getTownData() const{
    return town_data_;
}

void Town::resetHeroesInTavern(){
    if(getStatistic(HERO_BUYING_ENABLED)){

        std::vector<HeroType> hero_list(getHeroList().begin(), getHeroList().end());

        for(const auto& army: PlayerManager::getInstance().getPlayerByID(getPlayerID()).getArmies()){
            if(army != nullptr){
                auto it = std::find(hero_list.begin(), hero_list.end(), army->getArmyHero().getHeroType());
                hero_list.erase(it);
            }
        }

        std::vector<int> probabilities;
        for(const auto hero_type: hero_list){
            probabilities.push_back(getTownData().hero_probabilities.at(hero_type));
        }

        auto new_heroes = chooseNUniqueElementsFromContainerWithDistribution(hero_list,
        MAX_NUMER_OF_HEROES_IN_TAVERN, probabilities);

        for (int i=0; i<MAX_NUMER_OF_HEROES_IN_TAVERN; ++i){
            town_resources_.heroes_in_tavern_.at(i) = new_heroes.at(i);
        }
    }
}

void Town::increaseTroopsAvailableToBuy(){
    for(int i=0; i<getBuilding(DWELLINGS).getBuildingLevel(); ++i){
        increaseSingleTroopAvailableToBuy(i);
    }
}

void Town::increaseSingleTroopAvailableToBuy(const int index){
    int init_growth = getTroopData((getTownData().town_troops.at(index))).troop_battle_data.growth;
    int total_growth = init_growth * getStatistic(TROOP_GROWTH_MULTIPLIER) / 100;
    town_resources_.troops_available_to_buy_.at(index).second += total_growth;
}

const TownResources& Town::getTownResources() const{
    return town_resources_;
}

void Town::initializeTownResources(){
    for (const auto& troop_type : getTownData().town_troops){
        town_resources_.troops_available_to_buy_.push_back({troop_type, 0});
    }
}

void Town::generateBuildings(){

    buildings_.at(HALL)              = make_unique<Hall>            (*this, getTownData().town_building_textures[HALL]);
    buildings_.at(FORTIFICATIONS)    = make_unique<Fortifications>  (*this, getTownData().town_building_textures[FORTIFICATIONS]);
    buildings_.at(TAVERN)            = make_unique<Tavern>          (*this, getTownData().town_building_textures[TAVERN]);
    buildings_.at(MAGE_GUILD)        = make_unique<MageGuild>       (*this, getTownData().town_building_textures[MAGE_GUILD]);
    buildings_.at(DWELLINGS)         = make_unique<Dwellings>       (*this, getTownData().town_building_textures[DWELLINGS]);
}

void Town::onEnteringTown(){
    TownManager::getInstance().setCurrentTownID(town_id_);
    GraphicsManager::getInstance().setToScene(TOWN);
}
void Town::leaveTown(){
    TownManager::getInstance().resetCurrentTownID();
    GraphicsManager::getInstance().setToScene(WORLD);
    flag_leaving_town_ = true;
}

bool Town::canBuyTroopAtIndex(const int index) const{
    return index < getBuilding(DWELLINGS).getBuildingLevel() && canPlayerAfford(getTownData().cost_of_buying_troop.at(index)) &&
        getTownResources().troops_available_to_buy_.at(index).second > 0;
}

void Town::addSpellToResources(const pair<SpellType, int> new_spell){
    town_resources_.spells_in_mage_guild_.push_back(new_spell);
}

bool Town::canPlayerAfford(const ResourceArray& resources) const{
    return WorldManager::getInstance().getCurrentPlayer().canAfford(resources);
}
bool Town::spendPlayerResources(const ResourceArray& resources){
    return WorldManager::getInstance().getCurrentPlayer().spendResources(resources);
}

bool Town::buyTroop(const int index){

    if(canBuyTroopAtIndex(index) &&
        WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().addTroops(getTownData().town_troops.at(index), 1)){

        spendPlayerResources((getTownData().cost_of_buying_troop.at(index)));
        --town_resources_.troops_available_to_buy_.at(index).second;
        return true;
    }
    return false;
}

bool Town::buyHero(const int index){
    const HeroType hero_type = getTownResources().heroes_in_tavern_.at(index);

    if(hero_type == INVALID_HERO || !canPlayerAfford(COST_OF_BUYING_HERO)){
        return false;
    }

    pair<int, int> starting_pos = getArmyStartingPos();

    if (starting_pos.first == 0 && starting_pos.second == 0){
        return false;
    }

    if(PlayerManager::getInstance().addArmyToPlayer(getPlayerID(), hero_type, starting_pos.first, starting_pos.second)){
        spendPlayerResources(COST_OF_BUYING_HERO);
        town_resources_.heroes_in_tavern_.at(index) = INVALID_HERO;
        return true;
    }
    return false;

}

bool Town::buySpell(const int index){
    if(static_cast<size_t>(index) >= getTownResources().spells_in_mage_guild_.size()){
        return false;
    }
    pair<SpellType, int> spell_to_buy = getTownResources().spells_in_mage_guild_.at(index);
    if(canPlayerAfford(COST_OF_BUYING_SPELL) &&
        WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().addSpell(spell_to_buy.first, spell_to_buy.second)){
        spendPlayerResources(COST_OF_BUYING_SPELL);

        town_resources_.spells_in_mage_guild_.erase(find(town_resources_.spells_in_mage_guild_.begin(),
            town_resources_.spells_in_mage_guild_.end(), spell_to_buy));

        return true;
    }
    return false;
}

bool Town::upgradeBuilding(const BuildingType building){
    return getBuilding(building).attemptToLevelUp();
}

bool Town::attemptToDo(TownDecision decision){

    if(decision >= BUY_TROOP_1 && decision <= BUY_TROOP_7){
        return buyTroop(decision - BUY_TROOP_1);
    }
    else if(decision >= BUY_HERO_1 && decision <= BUY_HERO_3){
        return buyHero(decision - BUY_HERO_1);
    }
    else if(decision >= BUY_SPELL_1 && decision <= BUY_SPELL_9){
        return buySpell(decision - BUY_SPELL_1);
    }
    else if(decision >= UPGRADE_HALL && decision <= UPGRADE_DWELLINGS){
        return upgradeBuilding(static_cast<BuildingType>(decision - UPGRADE_HALL));
    }
    else if(decision == LEAVE_TOWN){
        leaveTown();
        return true;
    }
    else{
        throw InvalidIDError("Town Decision", decision);
        return false;
    }

}

void Town::resetFlagWasTownVisitedThisTurn(){
    flag_was_town_visited_this_turn_ = false;
}
