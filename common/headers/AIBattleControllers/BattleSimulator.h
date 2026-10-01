/**
 * @file BattleSimulator.h
 * @author Bartosz Han
 * @brief This file contains BattleSimulator class, which is used in VCMIBattleController to calculate score of attacks.
 */

#ifndef BATTLE_SIMULATOR_H
#define BATTLE_SIMULATOR_H

#include "Data/ObjectDataEnums.hpp"
#include "GameObjects/Battle.h"

/**
 * @class BattleSimulator
 * @brief This class is used in VCMIBattleController to calculate score of attacks.
 *
 */
class BattleSimulator{

private:

    Battle& battle_;
    std::map<const Stack*, int> mock_health_;
    std::map<const std::shared_ptr<Stack>, std::pair<int,int>> initial_pos_;
    std::vector<BattleObject*> battle_order_;
    std::vector<Stack*> all_stacks_;

    /**
     * @brief Perform mock attack, in which attacking stack deals mock damage to enemy stack,
     * which will be registered in simulator.
     *
     * @param attacker Attacking Stack
     * @param enemy Attacked Stack
     */
    void dealMockDamage(Stack& attacker, Stack& enemy);

    /**
     * @brief Remove stack from simulation. This method is used, when stack was killed in simulation.
     */
    void removeStackFromSimulation(Stack& stack_to_remove);

    /// @brief Returns true, if battle was finished, when all stack of one army were defeated.
    bool isMockBattleFinished() const;

    /// @brief Rotate battle order, which indicate that next troop will perform attack.
    void rotateBattleOrder();

    /// @brief Make current stack move on provided path.
    void moveCurrentStack(Stack& current_stack, std::vector<WorldMove> path);
    Stack& getCurrentStack() const;

    /// @brief Returns true, is stack is still present in simulation.
    bool isStackActive(const Stack& stack) const;

    /// @brief Get target of next attack of stack currently taking its turn.
    std::pair<int, int> chooseNextMove() const;

    /// @brief Get simulation's health points of stack.
    int getMockStackHealth(const Stack& stack) const;

    /// @brief Get simulation's troop quantity of stack.
    int getMockStackTroopQuantity(const Stack& stack) const;

    /// @brief Calculate, how many damage will stack loss, if it get provided amount of damage.
    int calculatePotentialLossInAttack(const Stack& stack, const int damage) const;


public:

    /**
     * @brief Construct a new Battle Simulator object
     *
     * @param battle Battle object, which represents initial state of simulation.
     */
    BattleSimulator(Battle& battle);
    Battle& getBattle() const;

    ///@brief Calculate sum of attacks of attacking and defending army.
    std::pair<float, float> calculateSimulationSumOfAttacks();

    ///@brief Perform provided number of turns of simulation.
    void simulate(int n_turns);

    ///@brief Set state of simulation to when simulation has started.
    void restoreBattleState();

    /// @Perform mock action, with provided event coordinates.
    void performMockBattleAction(int event_x_pos, int event_y_pos);

};

#endif