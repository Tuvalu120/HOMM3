/**
 * @file PlayerController.cc
 * @author Bartosz Han
 * @brief This file contains definition of class PlayerController.
 *
 */


#include "PlayerControllers/PlayerController.h"
#include "Managers/WorldManager.h"
#include "Managers/BattleManager.h"
#include "Managers/TownManager.h"
#include "Managers/DecisionManager.h"
#include "Data/Constants.hpp"

using namespace std;
using namespace constants;

Controller::Controller(): decision_var_1_(-1), decision_var_2_(-1){};


void Controller::setDecisionVars(const int new_decision_var_1, const int new_decision_var_2){
    decision_var_1_ = new_decision_var_1;
    decision_var_2_ = new_decision_var_2;
}

void Controller::setDecisionVars(const pair<int, int> new_decision_vars){
    decision_var_1_ = new_decision_vars.first;
    decision_var_2_ = new_decision_vars.second;
}

void Controller::resetDecisionVars(){
    setDecisionVars(-1, -1);
}

bool Controller::checkDecisionVars(int n_of_vars) const{
    if (n_of_vars == 1){
        return (decision_var_1_ != -1) && (decision_var_2_ <= -1);
    }
    else if (n_of_vars == 2){
        return (decision_var_1_ != -1 && decision_var_2_ != -1);
    }
    else{
        throw invalid_argument("Invalid number of variables to check in PlayerController. Only 1 or 2 variables can be checked.");
    }
}

void WorldMoveController::makeWorldMoveDecision(){
    const pair<int, int> end_turn_indicator = END_TURN_INDICATOR;
    if(decision_var_1_ == end_turn_indicator.first && decision_var_2_ == end_turn_indicator.second){
        WorldManager::getInstance().endTurnOfArmy();
    }
    else{
        WorldManager::getInstance().moveCurrentArmyToTile(decision_var_1_, decision_var_2_);
    }
};


void WorldDecisionController::makeWorldDecision(){
    DecisionManager::getInstance().decide(decision_var_1_);
};

void BattleController::makeBattleDecision(){
    const pair<int, int> end_turn_indicator = END_TURN_INDICATOR;
    if(decision_var_1_ == end_turn_indicator.first && decision_var_2_ == end_turn_indicator.second){
        getBattle().endTurnPrematurely();
        return;
    }
    else{
        getBattle().performBattleAction(
        decision_var_1_, decision_var_2_);
    }
}

void BattleController::makeSpellDecision(){
    const pair<int, int> end_turn_indicator = END_TURN_INDICATOR;
    if(decision_var_1_ == end_turn_indicator.first && decision_var_2_ == end_turn_indicator.second){
        getBattle().endTurnPrematurely();
        resetDecisionVars();
        return;
    }

    auto army_hero = getBattle().getCurrentArmy().getArmyHeroPtr();
    if (army_hero != nullptr){
        army_hero->chooseSpell(decision_var_1_);
    }
    else{
        throw UninitializedObjectError("Hero");
    }
};

void TownController::makeTownDecision(){

    TownManager::getInstance().getCurrentTown().attemptToDo(static_cast<TownDecision>(decision_var_1_));
};

PlayerController::PlayerController(const ControlType control):
    Controller(),control_type_(control){};

ControlType PlayerController::getControlType() const {
    return control_type_;
};