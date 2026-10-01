/**
 * @file DecisionManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class DecisionManager.
 *
 */


#include "Managers/DecisionManager.h"
#include "Managers/GraphicsManager.h"
#include "Managers/WorldManager.h"
#include "Miscellaneous/GameExceptions.hpp"

using namespace std;

DecisionManager::DecisionManager(): SubManager(), current_decision_(nullptr){};

void DecisionManager::makeDecision(unique_ptr<Decision> decision){

    current_decision_ = std::move(decision);
    GraphicsManager::getInstance().setFlagIsDrawingDecisionSceneEnabled(true);
    GraphicsManager::getInstance().setToScene(WORLD_DECISION_MAKING);

    WorldManager::getInstance().getCurrentPlayer().getPlayerController().makeWorldDecision();
    GraphicsManager::getInstance().setToScene(WORLD);
}

void DecisionManager::decide(const int decision_option_index){
    getCurrentDecision().chooseDecisionOption(decision_option_index);
    current_decision_ = nullptr;
}

Decision& DecisionManager::getCurrentDecision(){
    if(current_decision_ != nullptr){
        return *current_decision_;
    }
    throw UninitializedObjectError("Decision");
}

bool DecisionManager::isDecisionInProgress() const{
    return current_decision_ != nullptr;
}

void DecisionManager::resetManager(){
    current_decision_ = nullptr;
}