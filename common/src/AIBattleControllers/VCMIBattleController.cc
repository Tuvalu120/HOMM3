/**
 * @file VCMIBattleController.cc
 * @author Bartosz Han
 * @brief This file contains definition of class VCMIBattleController
 *
 */

#include <cmath>
#include <limits>

#include "AIBattleControllers/VCMIBattleController.h"
#include "AIBattleControllers/BattleSimulator.h"
#include "Managers/BattleManager.h"
#include "GameObjects/Army.h"

using namespace std;

VCMIBattleController::VCMIBattleController(int n_evaluation_turns):n_evaluation_turns_(n_evaluation_turns){};


std::pair<int, int> VCMIBattleController::selectBattleAction(){

    Stack* best_attack_possibility = nullptr;
    float max_score = -1.0f * numeric_limits<float>::infinity();

    Stack& current_stack = getBattle().getCurrentStack();
    for(const auto& enemy_stack: getBattle().getOppositeArmy().getArmyStacks()){

        if(current_stack.canAttackEnemyStackThisTurn(*enemy_stack)){
            BattleSimulator simulator(getBattle());
            pair<float, float> current_attack_state = simulator.calculateSimulationSumOfAttacks();
            float current_score = current_attack_state.first - current_attack_state.second;

            simulator.performMockBattleAction(enemy_stack->getXPos(), enemy_stack->getYPos());
            simulator.simulate(n_evaluation_turns_-1); //One Battle action was already performed.

            pair<float, float> predicted_attack_state = simulator.calculateSimulationSumOfAttacks();
            float predicted_score = predicted_attack_state.first - predicted_attack_state.second;

            simulator.restoreBattleState();
            float total_score = predicted_score - current_score;

            if(total_score > max_score){
                max_score = total_score;
                best_attack_possibility = enemy_stack.get();
            }
        }
    }

    if(best_attack_possibility != nullptr){
        return {best_attack_possibility->getXPos(), best_attack_possibility->getYPos()};
    }

    return attackEnemyForBiggestAttackLoss();
}
