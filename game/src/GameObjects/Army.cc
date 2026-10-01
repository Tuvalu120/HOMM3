/**
 * @file Army.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Army.
 *
 */


#include "GameObjects/Army.h"
#include "Data/Constants.hpp"
#include "GameObjects/BaseObjects/BattleElement.h"
#include "Managers/PlayerManager.h"
#include "Managers/WorldManager.h"
#include "Managers/BattleManager.h"
#include "Data/GameData.hpp"
#include "Miscellaneous/GameUtilities.hpp"
#include "Managers/WorldManager.h"
#include "Miscellaneous/GameExceptions.hpp"

using namespace std;
using namespace game_data;
using namespace constants;

Army::Army(const string texture_name, const int init_pos_x, const int init_pos_y, const bool is_world_army):
InteractiveObject(texture_name, ARMY, init_pos_x, init_pos_y, ARMY_PRIORITY, false),
army_experience_reward_(0),flag_is_world_army_(is_world_army),bonus_experience_points_(0){}

Army::Army(const int init_pos_x, const int init_pos_y, const bool is_world_army):Army("", init_pos_x, init_pos_y, is_world_army){};

Army::Army():Army("",-1,-1,false){};


void Army::interact(){

    Army& current_army = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy();
    BattleManager::getInstance().battle(current_army, *this);
}

const vector<shared_ptr<Stack>>& Army::getArmyStacks() const{
    return stacks_;
}

vector<shared_ptr<BattleObject>> Army::getBattleObjectsOfArmy() const{

    vector<shared_ptr<BattleObject>> battle_objects;
    for (const shared_ptr<Stack>& stack : getArmyStacks()){
        battle_objects.push_back(stack);
    }

    return battle_objects;
}


bool Army::addTroops(const TroopType troop_type, const unsigned int quantity){

    for(const auto& stack: stacks_){
        if(stack != nullptr && stack->getTroopType() == troop_type){
            stack->addQuantity(quantity);
            return true;
        }
    }
    if (stacks_.size() < MAX_NUMBER_OF_STACKS){
        stacks_.push_back(Army::createStack(troop_type, quantity, getPlayerID()));
        if (getArmyHeroPtr() != nullptr){
            stacks_.back()->setHeroStatistics(getArmyHeroPtr()->getStatisticsPtr());
        }
        return true;
    }
    return false;
}

bool Army::isArmyActive() const{
    return (stacks_.size() != 0);
};

vector<shared_ptr<Stack>>::iterator Army::removeStackFromArmy(Stack& stack_to_remove){
    auto it = find_if(stacks_.begin(), stacks_.end(), [&](const shared_ptr<Stack>& stack){
        return stack.get() == &stack_to_remove;
    });
    if(it != stacks_.end()){
        return stacks_.erase(it);
    }
    throw InvalidStackError(stack_to_remove.toString(), " does not belong to " + this->toString());
}

shared_ptr<Stack> Army::createStack(const TroopType troop_type, const unsigned int quantity, const int player_id){

    return make_shared<Stack>(troop_type, player_id, quantity);
}

shared_ptr<Hero> Army::getArmyHeroPtr() const{
    return nullptr;
}

int Army::getArmyPower() const{
    int sum = 0;
    for(const auto& stack: stacks_){
        sum += stack->getStatistic(TROOP_POWER) * stack->getQuantity();
    }
    return sum;
}

shared_ptr<Army> Army::generatePatternArmy(const unsigned int army_level,
    const vector<TroopType> troop_type_array, vector<int> probabilities){

    if (probabilities.empty()){
        probabilities = generateDescendingIntVector(army_level, army_level);
    };

    if (probabilities.size() > troop_type_array.size()){
        throw invalid_argument("Probabilities vector cannot be longer than troop_type_array");
    };

    while(probabilities.size() < troop_type_array.size()){
        probabilities.push_back(0);
    };
    shared_ptr<Army> army = make_shared<Army>();
    while (army->getArmyPower() < PATTERN_ARMY_POWER[army_level - 1]){
        TroopType troop_chosen = chooseFromContainerWithDistribution(troop_type_array, probabilities);
        army->addTroops(troop_chosen , 1);
    }
    return army;
}

shared_ptr<Army> Army::generatePatternArmy(const unsigned int army_level, const TroopType troop_type){
    unsigned int total_army_power = PATTERN_ARMY_POWER[army_level - 1];
    unsigned int single_troop_power = getTroopData(troop_type).troop_battle_data.troop_power;
    unsigned int quantity = ceil(static_cast<double>(total_army_power) / single_troop_power);

    shared_ptr<Army> army = make_shared<Army>();
    army->addTroops(troop_type, quantity);
    return army;
}

bool Army::isWorldArmy() const{
    return flag_is_world_army_;
}

void Army::setArmyExperienceReward(){
    army_experience_reward_ = 0;
    for(const auto& stack: stacks_){
        army_experience_reward_ += stack->getCurrentKnownHealth();
    }
}

void Army::setBonusExperiencePoints(const unsigned int value){
    bonus_experience_points_ = value;
}

int Army::getArmyExperienceReward() const{
    return army_experience_reward_ + bonus_experience_points_;
}

void Army::removeAllStacksFromArmy(){
    stacks_ = {};
}

void Army::drawAdditionalData(IPainter& painter) const{
    painter.drawSquareOnPixmap(playerIDToQColor(getPlayerID()));
}

void Army::resetStacksData(){
    for(const auto& stack: stacks_){
        stack->resetTroopData();
    }
}

const string Army::toString() const{
    int player_id = getPlayerID();
    string description = "Army of player: " + to_string(player_id);
    if (player_id != -1){
        description += " with ID: " + to_string(PlayerManager::getInstance().getPlayerByID(getPlayerID()).getArmyID(*this));
    }
    return description;
}

void Army::registerBattle(Battle* battle){
    BattleElement::registerBattle(battle);

    if(getArmyHeroPtr() != nullptr){
        Hero* hero = getArmyHeroPtr().get();
        hero->registerBattle(battle);
        for(int i=0; i<MAX_NUMBER_OF_SPELLS; ++i){
            if(hero->hasSpellAt(i)){
                hero->getSpellAt(i).registerBattle(battle);
            }
        }
    }

    for(auto& stack: getArmyStacks()){
        stack->registerBattle(battle);
    }
}

void Army::deregisterBattle(){
    BattleElement::deregisterBattle();

    if(getArmyHeroPtr() != nullptr){
        Hero* hero = getArmyHeroPtr().get();
        hero->deregisterBattle();
        for(int i=0; i<MAX_NUMBER_OF_SPELLS; ++i){
            if(hero->hasSpellAt(i)){
                hero->getSpellAt(i).deregisterBattle();
            }
        }
    }

    for(auto& stack: getArmyStacks()){
        stack->deregisterBattle();
    }

}