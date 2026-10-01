/**
 * @file VCMIBattleController.h
 * @author Bartosz Han
 * @brief This file contains VCMIBattleController, which is type of AI player, which
 * simulates battle in order to perform the action, which gave the best result.
 *
 */

#ifndef VCMI_BATTLE_CONTROLLER_H
#define VCMI_BATTLE_CONTROLLER_H

#include "AIBattleController.h"
#include "AIBattleControllers/AIBattleControllerConstants.hpp"

/**
 * @class VCMIBattleController
 * @brief This class is type of AI player, which
 * simulates battle in order to perform the action, which gave the best result.
 */
class VCMIBattleController : public AIBattleController{

    private:

    int n_evaluation_turns_;

    public:
    VCMIBattleController(int n_evaluation_turns = ai_battle_controller_constants::VCMI_N_EVALUATION_TURNS_HARD);
    std::pair<int, int> selectBattleAction() override;
};

#endif