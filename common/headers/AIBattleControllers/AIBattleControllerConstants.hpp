/**
 * @file AIBattleControllerConstants.hpp
 * @author Bartosz Han
 * @brief This file contains various constants related to artificial intelligence.
 *
 */

#ifndef AI_BATTLE_CONTROLLER_CONSTANTS_HPP
#define AI_BATTLE_CONTROLLER_CONSTANTS_HPP

#include "BattleAction.hpp"
#include "Data/Constants.hpp"

/**
 * @namespace ai_battle_controller_constants
 * @brief This namespace contains various constants related to artificial intelligence.
 *
 */
namespace ai_battle_controller_constants{

    /// @brief List of all actions, which can be performed by NEATBattleController
    const std::vector<BattleAction> AI_BATTLE_ACTIONS={

        GO_FORWARD,
        HIDE_STACK,
        GO_TO_FRIENDLY_STACKS,
        PROTECT_STRONGEST_FRIENDLY_STACK,
        SPREAD_FROM_FRIENDLY_STACKS,
        ATTACK_ENEMY_WITH_BIGGEST_HEALTH,
        ATTACK_ENEMY_WITH_LOWEST_ATTACK,
        ATTACK_ENEMY_FOR_BIGGEST_ATTACK_LOSS,
        EXECUTE_ENEMY_WITH_BIGGEST_ATTACK,
        ATTACK_ENEMY_WITH_BIGGEST_RANGE,
        ATTACK_ENEMY_WHICH_CAN_KILL_MY_STACK,
        ATTACK_ENEMY_WHICH_CAN_REDUCE_MY_ATTACK_BY_MOST,
        ATTACK_ENEMY_WHICH_IS_NEXT_IN_ORDER,
        DO_NOTHING_BATTLE_ACTION

    };

    /// @brief List of all attacking battle actions
    const std::vector<BattleAction> ATTACKING_ACTIONS={

        ATTACK_CLOSEST_ENEMY,
        ATTACK_FURTHEST_ENEMY,
        ATTACK_ENEMY_WITH_LOWEST_HEALTH,
        ATTACK_ENEMY_WITH_BIGGEST_HEALTH,
        ATTACK_ENEMY_WITH_LOWEST_ATTACK,
        ATTACK_ENEMY_WITH_BIGGEST_ATTACK,
        ATTACK_ENEMY_FOR_BIGGEST_ATTACK_LOSS,
        EXECUTE_ENEMY_WITH_BIGGEST_ATTACK,
        ATTACK_ENEMY_WITH_BIGGEST_RANGE,
        ATTACK_ENEMY_WHICH_CAN_KILL_MY_STACK,
        ATTACK_ENEMY_WHICH_CAN_REDUCE_MY_ATTACK_BY_MOST,
        ATTACK_ENEMY_WHICH_IS_NEXT_IN_ORDER

    };

    const std::vector<BattleAction> NON_ATTACKING_ACTIONS = {

        GO_FORWARD,
        HIDE_STACK,
        GO_TO_FRIENDLY_STACKS,
        SPREAD_FROM_FRIENDLY_STACKS,
        PROTECT_STRONGEST_FRIENDLY_STACK,
        DO_NOTHING_BATTLE_ACTION,

    };

        const int DISTANCE_INDEX_IN_STATE_VECTOR = 0;

    const int FRIENDLY_STACK_STATE_VECTOR_SIZE = 6;
    const int ENEMY_STACK_STATE_VECTOR_SIZE = 8;
    const int SPELL_STATE_VECTOR_SIZE = 5;
    const int HERO_STATE_VECTOR_SIZE = 4;
    const int HERO_AND_SPELL_VECTOR_SIZE = constants::MAX_NUMBER_OF_SPELLS * SPELL_STATE_VECTOR_SIZE + HERO_STATE_VECTOR_SIZE;

    const int N_OF_FRIENDLY_STACKS_IN_STATE_VECTOR = 8;
    const int N_OF_ENEMY_STACKS_IN_STATE_VECTOR = 8;

    /// @brief Expected numer of input nodes.
    const int EXPECTED_N_IN_NODES =
        N_OF_FRIENDLY_STACKS_IN_STATE_VECTOR * FRIENDLY_STACK_STATE_VECTOR_SIZE +
        N_OF_ENEMY_STACKS_IN_STATE_VECTOR * ENEMY_STACK_STATE_VECTOR_SIZE
        + 2 * HERO_AND_SPELL_VECTOR_SIZE;

    /// @brief Expected numer of output nodes.
    const int EXPECTED_N_OUT_NODES = AI_BATTLE_ACTIONS.size();


    const int MAX_NEAT_CONTROLLER_STALENESS = 10;

    const int VCMI_N_EVALUATION_TURNS_EASY = 4;
    const int VCMI_N_EVALUATION_TURNS_HARD = 10;


    const float NO_DATA_INDICATOR = 0.0f;

    const float INVALID_OUTPUT = -10000.0f;


    /// @brief List of variables, by which data is being divided, in order to normalize it.
    const std::vector<float> STATE_VECTOR_NORMALIZATION_VALUES = {
        20.0f, 20.0f, 150.0f, 12.0f, 32.0f, 500.0f, 8.0f, 12.0f, 12.0f, 32.0f,
        500.0f, 8.0f, 12.0f, 12.0f, 32.0f, 500.0f, 8.0f, 12.0f, 12.0f, 32.0f,
        500.0f, 8.0f, 12.0f, 12.0f, 20.0f, 200.0f, 100.0f, 12.0f, 12.0f, 150.0f,
        20.0f, 200.0f, 100.0f, 12.0f, 12.0f, 150.0f, 20.0f, 200.0f, 100.0f, 12.0f,
        12.0f, 150.0f, 20.0f, 200.0f, 100.0f, 12.0f, 12.0f, 150.0f, 20.0f, 200.0f,
        100.0f, 12.0f, 12.0f, 150.0f, 20.0f, 200.0f, 100.0f, 12.0f, 12.0f, 150.0f,
        20.0f, 200.0f, 100.0f, 12.0f, 12.0f, 150.0f, 20.0f, 200.0f, 100.0f, 12.0f,
        12.0f, 150.0f, 20.0f, 20.0f, 150.0f, 12.0f, 32.0f, 500.0f, 8.0f, 12.0f,
        12.0f, 32.0f, 500.0f, 8.0f, 12.0f, 12.0f, 32.0f, 500.0f, 8.0f, 12.0f,
        12.0f, 32.0f, 500.0f, 8.0f, 12.0f, 12.0f, 20.0f, 200.0f, 100.0f, 12.0f,
        12.0f, 200.0f, 200.0f, 1.0f, 20.0f, 200.0f, 100.0f, 12.0f, 12.0f, 200.0f,
        200.0f, 1.0f, 20.0f, 200.0f, 100.0f, 12.0f, 12.0f, 200.0f, 200.0f, 1.0f,
        20.0f, 200.0f, 100.0f, 12.0f, 12.0f, 200.0f, 200.0f, 1.0f, 20.0f, 200.0f,
        100.0f, 12.0f, 12.0f, 200.0f, 200.0f, 1.0f, 20.0f, 200.0f, 100.0f, 12.0f,
        12.0f, 200.0f, 200.0f, 1.0f, 20.0f, 200.0f, 100.0f, 12.0f, 12.0f, 200.0f,
        200.0f, 1.0f, 20.0f, 200.0f, 100.0f, 12.0f, 12.0f, 200.0f, 200.0f, 1.0f

    };
}

#endif