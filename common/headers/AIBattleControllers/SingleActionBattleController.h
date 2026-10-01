/**
 * @file SingleActionBattleController.h
 * @author Bartosz Han
 * @brief This file contains SingleActionBattleController class, which is a type of AI player,
 * which always picks one certain action.
 *
 */

#ifndef SINGLE_ACTION_BATTLE_CONTROLLER_H
#define SINGLE_ACTION_BATTLE_CONTROLLER_H

#include "AIBattleController.h"


/**
 * @class RandomBattleController
 * @brief This is a type of AI player,
 * which always picks one certain action.
 *
 */
class SingleActionBattleController : public AIBattleController{
    private:
    const BattleAction action_;
    public:
    /**
     * @brief Construct a new Single Action Battle Controller object
     *
     * @param action BattleAction, which this player will always perform.
     */
    SingleActionBattleController(const BattleAction action);
    std::pair<int, int> selectBattleAction() override;

};

#endif