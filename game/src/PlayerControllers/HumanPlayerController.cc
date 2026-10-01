/**
 * @file HumanPlayerController.cc
 * @author Bartosz Han
 * @brief This file contains definition of class HumanPlayerController.
 *
 */

#include "PlayerControllers/HumanPlayerController.h"
#include "Managers/GraphicsManager.h"
#include "Managers/IGraphicsManager.h"
#include "PlayerControllers/PlayerController.h"


HumanPlayerController::HumanPlayerController():PlayerController(HUMAN){}

void HumanPlayerController::makeWorldMoveDecision(){

    resetDecisionVars();
    waitForDecisionVars(2);
    WorldMoveController::makeWorldMoveDecision();


    // resetDecisionVars();

};

void HumanPlayerController::makeWorldDecision(){

    resetDecisionVars();

    waitForDecisionVars(1);

    PlayerController::makeWorldDecision();

};

void HumanPlayerController::makeBattleDecision(){

    resetDecisionVars();
    GraphicsManager::getInstance().setFlagIsUIActive(true);
    waitForDecisionVars(2);
    GraphicsManager::getInstance().setFlagIsUIActive(false);

    PlayerController::makeBattleDecision();

    resetDecisionVars();
}

void HumanPlayerController::makeSpellDecision(){
    resetDecisionVars();


    if(GraphicsManager::getGraphicsManagerType() == REAL_GRAPHICS_MANAGER){
        GraphicsManager::getInstance().setFlagIsUIActive(true);
        GraphicsManager::getInstance().enableSpellLayout();
    }

    waitForDecisionVars(1);

    if(GraphicsManager::getGraphicsManagerType() == REAL_GRAPHICS_MANAGER){
        GraphicsManager::getInstance().disableSpellLayout();
        GraphicsManager::getInstance().setFlagIsUIActive(false);
    }

    PlayerController::makeSpellDecision();

    resetDecisionVars();
};

void HumanPlayerController::makeTownDecision(){
    resetDecisionVars();
    waitForDecisionVars(1);
    PlayerController::makeTownDecision();
    resetDecisionVars();
};

void HumanPlayerController::waitForDecisionVars(const int n_of_vars) const{
    while (!checkDecisionVars(n_of_vars)){
        GraphicsManager::getInstance().processEvents();
    }
}
