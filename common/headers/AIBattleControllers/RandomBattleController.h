/**
 * @file RandomBattleController.h
 * @author Bartosz Han
 * @brief This file contains RandomBattleController class, which is a type of AI player,
 * which always picks random heuristic to perform.
 *
 */
#ifndef RANDOM_BATTLE_CONTROLLER_H
#define RANDOM_BATTLE_CONTROLLER_H

#include "AIBattleController.h"

/**
 * @class RandomBattleController
 * @brief This is a type of AI player,
 * which always picks random heuristic to perform.
 *
 */
class RandomBattleController: public AIBattleController{
    private:
    std::vector<BattleAction> available_actions_;
    public:
    RandomBattleController(const std::vector<BattleAction> available_actions);
    std::pair<int, int> selectBattleAction() override;
};

#endif