/**
 * @file AIBattleController.h
 * @author Bartosz Han
 * @brief This file contains AIBattleController class, which is an abstract used in another battle controllers, which uses some kind of heuristics.
 *
 */
#ifndef AI_BATTLE_CONTROLLER_H
#define AI_BATTLE_CONTROLLER_H


#include <functional>

#include "AIBattleControllers/BattleAction.hpp"
#include "PlayerControllers/PlayerController.h"
#include "GameObjects/Stack.h"

/**
 * @struct SpellCastInfo
 * @brief This class represents a spell cast on given tile.
 */
struct SpellCastInfo{
    int spell_index;
    int target_x_pos;
    int target_y_pos;
};

/**
 * @class AIBattleController
 * @brief This class is an abstract used in another battle controllers, which uses heuristics provided by this class.
 */
class AIBattleController : public BattleController{

private:

    std::pair<int, int> spell_target_;
    constexpr static std::pair<int, int> NO_SPELL_TARGET = {-1, -1};


protected:

    /**
     * @brief Get the list of enemy stack in battlefield, ordered ascending by provided function.
     *
     * @param order_function Function used in ordering stacks.
     */
    std::vector<std::reference_wrapper<Stack>> getEnemyStacksInOrder(
        std::function<float(Stack&)> order_function) const;

    /**
     * @brief Get the list of all tiles in battlefield, which can be reached by stack currently
     performing its turn, ordered ascending by provided function.
     *
     * @param value_function Function used in ordering tiles.
     * @param edge_case_heuristic Function used in situation, when for all tiles, value_function returns 0.
     */
    std::vector<std::pair<int,int>> getWalkableTilesInOrder(
        std::function<float(const std::pair<int,int>&)> value_function, std::function<std::pair<int,int>()> edge_case_heuristic) const;

    /**
     * @brief Returns pair of ints, representing enemy position, which should be attacked.
     * For every enemy, a score_function is used to calculate quality of attack.
     * This method will return the enemy with the best grade of attack, which can be attacked this turn.
     * If none can be attacked, the best currently reachable enemy will be chosen.
     *
     * @param current_stack Stack, which is currently performing its turn.
     * @param score_function Function to grade attacks on enemies.
     */
    std::pair<int, int> attackEnemyWithCondition(const Stack& current_stack,
        std::function<float(Stack&)> score_function) const;

    /**
     * @brief Returns pair of ints, representing enemy position, which should be attacked.
     * For every enemy, a score_function is used to calculate quality of attack.
     * This method will return the enemy with the best grade of attack, which can be attacked this turn.
     * If none can be attacked, the best currently reachable enemy will be chosen. If none is reachable,
     * edge_case_heuristic will be performed.
     *
     * @param current_stack Stack, which is currently performing its turn.
     * @param score_function Function to grade attacks on enemies.
     * @param edge_case_heuristic Function to use, if it is impossible to attack or get closer to enemy.
     */
    std::pair<int, int> attackEnemyWithCondition(const Stack& current_stack,
        std::function<float(Stack&)> score_function, std::function<std::pair<int,int>()> edge_case_heuristic) const;

    /**
     * @brief Select heuristic to perform.
     * Classes implementing this class should choose heuristic to perform and get the coordinates of target.
     * @return std::pair<int, int>
     */
    virtual std::pair<int, int> selectBattleAction() = 0;

    /**
     * @brief Translate BattleAction into coordinates.
     *
     * @param action Action to translate into coordinates.
     */
    std::pair<int, int> battleActionToCoordinates(BattleAction action) const;

    /**
     * @brief Get target of following heuristic:
     * Attack enemy which is closest to stack taking turn.
     */
    std::pair<int, int> attackClosestEnemy() const;

    /**
     * @brief Get target of following heuristic:
     * Attack enemy which is furthest to stack taking turn.
     */
    std::pair<int, int> attackFurthestEnemy() const;

    /**
     * @brief Get target of following heuristic:
     * Attack enemy which has lowest amount of health.
     */
    std::pair<int, int> attackEnemyWithLowsetHealth() const;

    /**
     * @brief Get target of following heuristic:
     * Attack enemy which has biggest amount of health.
     */
    std::pair<int, int> attackEnemyWithBiggestHealth() const;

    /**
     * @brief Get target of following heuristic:
     * Attack enemy which has lowest average damage done.
     */
    std::pair<int, int> attackEnemyWithLowsetAttack() const;

    /**
     * @brief Get target of following heuristic:
     * Attack enemy which has biggest average damage done.
     */
    std::pair<int, int> attackEnemyWithBiggestAttack() const;

    /**
     * @brief Get target of following heuristic:
     * For each enemy stack W that can be attacked this turn,
     calculate the attack loss W if it were attacked by a
     stack moving at that moment. The attack point loss is
     calculated as the product of the average number of
     damage points dealt by one unit in W and the number
     of units it would lose as a result of the attack.
     Attack the enemy stack for which the attack
     loss W would be the greatest.
     */
    std::pair<int, int> attackEnemyForBiggestAttackLoss() const;

    /**
     * @brief Get target of following heuristic:
     * For each enemy stack W that can be attacked
     this turn, check if the stack performing
     the attack now would result in the
     loss of all W troops. Attack the enemy
     stack that meets this condition and
     can deal the greatest average damage.
     */
    std::pair<int, int> executeEnemyWithBiggestAttack() const;

    /**
     * @brief Get target of following heuristic:
     * Attack enemy which has biggest range.
     */
    std::pair<int, int> attackEnemyWithBiggestRange() const;

    /**
     * @brief Get target of following heuristic:
     * For each enemy stack W check if there is at
     least one friendly stack P that can lost
     * all troops from W.
     * Attack the enemy that meets this condition and
     * that can destroy the friendly stack with the
     * highest average damage dealt.
     */
    std::pair<int, int> attackEnemyWhichCanKillMyStack() const;

    /**
     * @brief Get target of following heuristic:
     * For each enemy stack W, calculate the attack loss
     * that the friendly army might suffer if W attacked
     * now according to the heuristic attackEnemyForBiggestAttackLoss.
     * Attack the enemy for which the attack loss of the friendly army would be
     * the greatest.
     */
    std::pair<int, int> attackEnemyWhichCanReduceMyAttackByMost() const;

    /**
     * @brief Get target of following heuristic:
     * Attack enemy, which is next in order.
     */
    std::pair<int, int> attackEnemyWhichIsNextInOrder() const;

    /**
     * @brief Get target of following heuristic:
     *
     */
    std::pair<int, int> protectStrongestFriendlyStack() const;

    /**
     * @brief Get target of following heuristic:
     * Go to the field that will best prevent the enemy
     stack from attacking the friendly stack
     with the strongest attack. In this heuristic,
     the field is chosen for which the sum of
     attacks from enemy stacks that would be
     able to attack the strongest friendly stack,
     assuming that the stack currently taking its turn moved into this tile
     the field, is the smallest
     */
    std::pair<int, int> spreadFromFriendlyStacks() const;

    /**
     * @brief Get target of following heuristic:
     * Go to the side of the enemy
     */
    std::pair<int, int> goForward() const;

    /**
     * @brief Get target of following heuristic:
     * Move to the tile where the sum of attacks
     * from enemy stacks that would be able
     * to attack the stack currently moving is the smallest.
     */
    std::pair<int, int> hideStack() const;

    /**
     * @brief Get target of following heuristic:
     * Go to the file for which the average distance
     * from all friendly stacks is the smallest.
     */
    std::pair<int, int> goToFriendlyStacks() const;


    /**
     * @brief Get target of following heuristic:
     * Skip current turn.
     */
    std::pair<int, int> doNothing() const;
public:

    /**
     * @brief Construct a new AIBattleController object
     *
     */
    AIBattleController();
    void makeBattleDecision() override;
    void makeSpellDecision() override;
};

#endif