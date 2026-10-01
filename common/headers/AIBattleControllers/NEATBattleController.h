/**
 * @file NEATBattleController.h
 * @author Bartosz Han
 * @brief this file constains NEAtBattleController, which is a type of AI player, which is using neural networks to select actions.
 *
 */
#ifndef NEAT_BATTLE_CONTROLLER_H
#define NEAT_BATTLE_CONTROLLER_H

#include "AIBattleController.h"
#include "GameObjects/Stack.h"
#include "GameObjects/Hero.h"
#include "NeuralNetwork/NeuralNetwork.h"

#include <vector>

/**
 * @class NEATBattleController
 * @brief This class represents a type of AI player, which is using neural networks to select actions
 */
class NEATBattleController: public AIBattleController{

private:

    NeuralNetwork network_;

    /**
     * @brief Shape the vector, in such way that it can be processed by neural network.
     */
    std::vector<float> shapeStateVector(
        std::vector<std::vector<float>>& state, const int state_vector_size, const int n_stacks_to_log) const;

    /// @brief Get state of friendly army.
    std::vector<float> generateFriendlyArmyState(const Army& friendly_army, const Stack& current_stack) const;

    /// @brief Get state of enemy army.
    std::vector<float> generateEnemyArmyState(const Army& enemy_army, const Stack& current_stack) const;

    /// @brief Get state of friendly stack.
    std::vector<float> generateStackStateWithRespectToFriendlyStack(const Stack& current_stack, const Stack& friendly_stack) const;

    /// @brief Get state of enemy stack.
    std::vector<float> generateStackStateWithRespectToEnemyStack(const Stack& current_stack, const Stack& enemy_stack) const;

    /// @brief Get state of hero.
    std::vector<float> generateHeroStateVector(const Hero& hero) const;

    /// @brief Get state of spell.
    std::vector<float> generateSpellStateVector(const Spell& spell, const Hero& owner_of_spell) const;

    /// @brief Normalize state vector, by dividing each value of state by some constant.
    void normalizeStateVector(std::vector<float>& state) const;

    /// Create state vector of the battle.
    std::vector<float> generateStateVector() const;

    /// Get the output of neural network, describing scores of every heuristic.
    const std::vector<float> predict(const std::vector<float>& input);

    int staleness_ = 0;
    std::vector<BattleAction> performed_actions_;

    std::pair<int, int> selectBattleAction() override;

public:

    NEATBattleController(const NeuralNetwork& model);

    /// @brief Get list of all performed actions of this controller.
    const std::vector<BattleAction>& getPerformedActionsVector() const;

};

#endif