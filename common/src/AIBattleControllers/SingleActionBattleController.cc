/**
 * @file SingleActionBattleController.cc
 * @author Bartosz Han
 * @brief This file contains definition of class SingleActionBattleController.
 *
 */

#include "AIBattleControllers/SingleActionBattleController.h"

SingleActionBattleController::SingleActionBattleController(const BattleAction action):action_(action){};

std::pair<int, int> SingleActionBattleController::selectBattleAction(){
    return battleActionToCoordinates(action_);
}
