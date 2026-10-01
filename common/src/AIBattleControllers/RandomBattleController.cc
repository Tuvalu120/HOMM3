/**
 * @file RandomBattleController.cc
 * @author Bartosz Han
 * @brief This file contains definition of class RandomBattleController.
 *
 */

#include "AIBattleControllers/RandomBattleController.h"
#include "Managers/BattleManager.h"
#include "Data/Constants.hpp"
#include "Utilities.hpp"

using namespace std;
using namespace constants;


RandomBattleController::RandomBattleController(const std::vector<BattleAction> available_actions):available_actions_(available_actions){};


pair<int,int> RandomBattleController::selectBattleAction(){

    BattleAction decision = chooseFromContainer(available_actions_);

    pair<int, int> coordinates = battleActionToCoordinates(decision);

    Stack& current_stack = getBattle().getCurrentStack();
    int stack_x_pos = current_stack.getXPos();
    int stack_y_pos = current_stack.getYPos();

    if(coordinates.first == stack_x_pos && coordinates.second == stack_y_pos){
        coordinates = END_TURN_INDICATOR;
    }

    return coordinates;

}
