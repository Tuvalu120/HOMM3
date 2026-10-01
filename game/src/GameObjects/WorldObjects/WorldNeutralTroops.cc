/**
 * @file WorldNeutralTroops.cc
 * @author Bartosz Han
 * @brief This file contains definition of class WorldNeutralTroops.
 *
 */


#include "GameObjects/WorldObjects/WorldNeutralTroops.h"
#include "Decisions/Decision.h"
#include "Data/ObjectDataEnums.hpp"
#include "Data/GameData.hpp"
#include "Managers/DecisionManager.h"

using namespace std;
using namespace game_data;
using namespace constants;

WorldNeutralTroops::WorldNeutralTroops(): WorldGeneratedInteractiveObject(EMPTY_IMAGE, NEUTRAL_TROOPS_OBJECT_TYPE,
    priority_=OTHER_OBJECT_PRIORITY, destroy_after_interaction_=false),
    was_army_visited_this_turn_(false){}

void WorldNeutralTroops::interact(){
    if(was_army_visited_this_turn_){
        return;
    }
    was_army_visited_this_turn_ = true;
    unique_ptr<NeutralTroopsDecision> decision = make_unique<NeutralTroopsDecision>(*object_army_);
    DecisionManager::getInstance().makeDecision(std::move(decision));
    if(!object_army_->isArmyActive()){
        setFlagDestroyAfterInteraction();
    }
}

Army& WorldNeutralTroops::getObjectArmy() const{
    return *object_army_;
}

void WorldNeutralTroops::setUpArmy(const int army_power){
    if(object_army_ == nullptr){
        power_ = army_power;
        vector<TownType> family_types = TROOP_FAMILY_TYPES;
        int army_power_chosen = generateRandomIntInRange(0, army_power-1);
        TownType troop_types = chooseFromContainer(family_types);
        TroopType troop_type_chosen = TROOP_TYPES.at(troop_types).at(army_power_chosen);

        setCurrentTextureName(getTroopTextureName(troop_type_chosen));
        object_army_ = Army::generatePatternArmy(army_power, troop_type_chosen);
    }
}

void WorldNeutralTroops::resetVisitFlag(){
    was_army_visited_this_turn_ = false;
}

void WorldNeutralTroops::drawAdditionalData(IPainter& painter) const{
    painter.drawNumberOnPixmap(power_);
}
