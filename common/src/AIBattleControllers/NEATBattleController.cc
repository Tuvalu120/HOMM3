/**
 * @file SubManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class NEATBattleController.
 *
 */


#include <algorithm>
#include <cstddef>
#include <iterator>
#include <vector>

#include "AIBattleControllers/NEATBattleController.h"
#include "AIBattleControllers/BattleAction.hpp"
#include "Data/ObjectDataEnums.hpp"
#include "GameObjects/Hero.h"
#include "GameObjects/Spell.h"
#include "Managers/BattleManager.h"
#include "Data/Constants.hpp"
#include "Managers/GraphicsManager.h"
#include "Managers/IGraphicsManager.h"
#include "NeuralNetwork/NeuralNetwork.h"
#include "GameObjects/Army.h"
#include "Utilities.hpp"
#include "AIBattleControllers/AIBattleControllerConstants.hpp"

using namespace std;
using namespace constants;
using namespace game_data;



NEATBattleController::NEATBattleController(const NeuralNetwork& network):network_(network){};

std::pair<int, int> NEATBattleController::selectBattleAction(){

    vector<float> state = generateStateVector();

    vector<float> output = network_.propagate(state);

    auto max_value = max_element(output.begin(), output.end());
    BattleAction decision = ai_battle_controller_constants::AI_BATTLE_ACTIONS.at(distance(output.begin(), max_value));

    bool attacking_decision = find(ai_battle_controller_constants::ATTACKING_ACTIONS.begin(),
        ai_battle_controller_constants::ATTACKING_ACTIONS.end(), decision) != ai_battle_controller_constants::ATTACKING_ACTIONS.end();

    if(attacking_decision){
        staleness_ = 0;
    }
    else{
        ++staleness_;
        if(staleness_ >= ai_battle_controller_constants::MAX_NEAT_CONTROLLER_STALENESS){
            do {
                output.at(distance(output.begin(), max_value)) = ai_battle_controller_constants::INVALID_OUTPUT;
                max_value = max_element(output.begin(), output.end());
                decision = ai_battle_controller_constants::AI_BATTLE_ACTIONS.at(distance(output.begin(), max_value));
                attacking_decision = find(ai_battle_controller_constants::ATTACKING_ACTIONS.begin(),
                    ai_battle_controller_constants::ATTACKING_ACTIONS.end(), decision) != ai_battle_controller_constants::ATTACKING_ACTIONS.end();
            }while (!attacking_decision);
        }
    }

    performed_actions_.push_back(decision);
    if(GraphicsManager::getInstance().getFlagIsAIVerbose()){
        cout << "AI: PERFORMED ACTION - " << actionToString(decision) << endl;
    }


    pair<int, int> coordinates = battleActionToCoordinates(decision);

    return coordinates;

}


vector<float> NEATBattleController::generateStateVector() const{


    ///

    /*
       FRIENDLY ARMY
       ENEMY ARMY
    */

    Stack& current_stack = getBattle().getCurrentStack();
    Army& friendly_army = getBattle().getCurrentArmy();
    Army& enemy_army = getBattle().getOppositeArmy();

    vector<float> friendly_army_state = generateFriendlyArmyState(friendly_army, current_stack);
    vector<float> enemy_army_state = generateEnemyArmyState(enemy_army, current_stack);

    vector<float> state = friendly_army_state;
    state.reserve(ai_battle_controller_constants::EXPECTED_N_OUT_NODES);
    state.insert(state.end(), enemy_army_state.begin(), enemy_army_state.end());

    normalizeStateVector(state);

    return state;
}

void NEATBattleController::normalizeStateVector(std::vector<float>& state) const{
    for(int i=0; i< ai_battle_controller_constants::EXPECTED_N_IN_NODES; ++i){
        state.at(i) /= ai_battle_controller_constants::STATE_VECTOR_NORMALIZATION_VALUES.at(i);
    }
}


std::vector<float> NEATBattleController::shapeStateVector(std::vector<std::vector<float>>& state,
    const int state_vector_size, const int n_items_to_log) const{

    stable_sort(state.begin(), state.end(),
    [](const std::vector<float>& v1, const std::vector<float>& v2){
        const int index = ai_battle_controller_constants::DISTANCE_INDEX_IN_STATE_VECTOR;
        return v1.at(index) < v2.at(index);
    });

    if(static_cast<int>(state.size()) > n_items_to_log){
        state.resize(n_items_to_log);
    }

    std::vector<float> flat_vector = flatten(state);

    int n_to_fill = n_items_to_log - state.size();
    if(n_to_fill > 0){
        std::vector<float> empty_state_vector;
        empty_state_vector.assign(n_to_fill*state_vector_size, ai_battle_controller_constants::NO_DATA_INDICATOR);
        flat_vector.insert(flat_vector.end(), empty_state_vector.begin(), empty_state_vector.end());
    }

    return flat_vector;

}


std::vector<float> NEATBattleController::generateFriendlyArmyState(const Army& friendly_army, const Stack& current_stack) const{

    /*

    Order:

    HERO STATE

    STACK 0 STATE
    STACK 1 STATE
    ...
    STACK N STATE

    */

    vector<float> army_state;
    army_state.reserve(ai_battle_controller_constants::HERO_AND_SPELL_VECTOR_SIZE +
        ai_battle_controller_constants::N_OF_FRIENDLY_STACKS_IN_STATE_VECTOR * ai_battle_controller_constants::FRIENDLY_STACK_STATE_VECTOR_SIZE);


    if(friendly_army.getArmyHeroPtr() != nullptr){
        vector<float> hero_state = generateHeroStateVector(*friendly_army.getArmyHeroPtr());
        army_state.insert(army_state.end(), hero_state.begin(), hero_state.end());
    }
    else{
        army_state.assign(ai_battle_controller_constants::HERO_AND_SPELL_VECTOR_SIZE, ai_battle_controller_constants::NO_DATA_INDICATOR);
    }

    vector<vector<float>> stacks_states;

    for(const auto& friendly_stack: friendly_army.getArmyStacks()){
        stacks_states.push_back(generateStackStateWithRespectToFriendlyStack(current_stack, *friendly_stack));
    }

    vector<float> shaped_stacks_states = shapeStateVector(stacks_states,
        ai_battle_controller_constants::FRIENDLY_STACK_STATE_VECTOR_SIZE, ai_battle_controller_constants::N_OF_FRIENDLY_STACKS_IN_STATE_VECTOR);
    army_state.insert(army_state.end(), shaped_stacks_states.begin(), shaped_stacks_states.end());

    return army_state;
}

std::vector<float> NEATBattleController::generateEnemyArmyState(const Army& enemy_army, const Stack& current_stack) const{

    /*

    Order:

    HERO STATE

    STACK 0 STATE
    STACK 1 STATE
    ...
    STACK N STATE

    */

    vector<float> army_state;
    army_state.reserve(ai_battle_controller_constants::HERO_AND_SPELL_VECTOR_SIZE + ai_battle_controller_constants::N_OF_ENEMY_STACKS_IN_STATE_VECTOR * ai_battle_controller_constants::ENEMY_STACK_STATE_VECTOR_SIZE);

    if(enemy_army.getArmyHeroPtr() != nullptr){
        vector<float> hero_state = generateHeroStateVector(*enemy_army.getArmyHeroPtr());
        army_state.insert(army_state.end(), hero_state.begin(), hero_state.end());
    }
    else{
        army_state.assign(ai_battle_controller_constants::HERO_AND_SPELL_VECTOR_SIZE, ai_battle_controller_constants::NO_DATA_INDICATOR);
    }

    vector<vector<float>> stacks_states;

    for(const auto& enemy_stack: enemy_army.getArmyStacks()){
        stacks_states.push_back(generateStackStateWithRespectToEnemyStack(current_stack, *enemy_stack));
    }

    vector<float> shaped_stacks_states = shapeStateVector(stacks_states,
        ai_battle_controller_constants::ENEMY_STACK_STATE_VECTOR_SIZE,
        ai_battle_controller_constants::N_OF_ENEMY_STACKS_IN_STATE_VECTOR);

    army_state.insert(army_state.end(), shaped_stacks_states.begin(), shaped_stacks_states.end());

    return army_state;
}


std::vector<float> NEATBattleController::generateStackStateWithRespectToFriendlyStack(const Stack& current_stack, const Stack& friendly_stack) const{

    /*

    Order:

    DISTANCE
    HEALTH
    QUANTITY
    RANGE
    SPEED
    CURRENT_AVERAGE_DAMAGE_DONE

    TOTAL : 6

    */

    std::vector<float> state;
    state.reserve(ai_battle_controller_constants::FRIENDLY_STACK_STATE_VECTOR_SIZE);

    float distance = getBattle().getBattlefield().getWalkingDistanceBetweenTiles(current_stack.getXPos(), current_stack.getYPos(),
        friendly_stack.getXPos(), friendly_stack.getYPos());

    state.push_back(distance);
    state.push_back(friendly_stack.getStatistic(BASE_HEALTH));
    state.push_back(friendly_stack.getQuantity());
    state.push_back(friendly_stack.getStatistic(RANGE));
    state.push_back(friendly_stack.getStatistic(TROOP_SPEED));
    state.push_back(friendly_stack.getCurrentAverageStackDamage());

    return state;
}

std::vector<float> NEATBattleController::generateStackStateWithRespectToEnemyStack(const Stack& current_stack, const Stack& enemy_stack) const{

    /*

    Order:

    DISTANCE
    HEALTH
    QUANTITY
    RANGE
    SPEED
    EXPECTED_DAMAGE_DEALT
    EXPECTED_DAMAGE_RECEIVED
    FLAG_CAN_ATTACK_THIS_TURN

    TOTAL : 8

    */

    std::vector<float> state;
    state.reserve(ai_battle_controller_constants::ENEMY_STACK_STATE_VECTOR_SIZE);

    auto path_data = getBattle().getBattlefield().findPathToTile(current_stack.getXPos(), current_stack.getYPos(),
        enemy_stack.getXPos(), enemy_stack.getYPos(),
            getBattle().getBattlefield().generateStackEndConditionFunction(current_stack));

    float distance = path_data.second != -1 ? path_data.second : BATTLEFIELD_NO_PATH_DISTANCE;
    state.push_back(distance);
    state.push_back(enemy_stack.getStatistic(BASE_HEALTH));
    state.push_back(enemy_stack.getQuantity());
    state.push_back(enemy_stack.getStatistic(RANGE));
    state.push_back(enemy_stack.getStatistic(TROOP_SPEED));


    float expected_damage_dealt = current_stack.calculateAverageStackDamage(enemy_stack);
    float expected_damage_received = enemy_stack.calculateAverageStackDamage(current_stack);

    state.push_back(expected_damage_dealt);
    state.push_back(expected_damage_received);

    float can_attack_this_turn = static_cast<float>(current_stack.canAttackEnemyStackThisTurn(enemy_stack.getXPos(),
         enemy_stack.getYPos(), path_data.first));

    state.push_back(can_attack_this_turn);

    return state;

}

vector<float> NEATBattleController::generateSpellStateVector(const Spell& spell, const Hero& owner_of_spell) const{

    /*

    Order:

    COST
    DAMAGE
    RADIUS
    ATTACK_ADDED
    DEFENSE_ADDED

    TOTAL: 5

    */

    vector<float> state;
    state.reserve(ai_battle_controller_constants::SPELL_STATE_VECTOR_SIZE);

    state.push_back(spell.getStatistic(COST));

    float total_damage = spell.getStatistic(BASE_DAMAGE) + owner_of_spell.getStatistic(SPELL_POWER) * spell.getStatistic(DAMAGE_PER_SPELL_POWER_POINT);

    state.push_back(total_damage);
    state.push_back(spell.getStatistic(RADIUS));

    vector<TroopStat> stats_added = getSpellData(spell.getSpellType()).spell_battle_data.troop_stats_to_add;

    float attack_added = 0.0f;

    if(find(stats_added.begin(), stats_added.end(), TROOP_ATTACK) != stats_added.end()){
        attack_added = spell.getStatistic(STATS_ADDED);
    };

    float defense_added = 0.0f;

    if(find(stats_added.begin(), stats_added.end(), TROOP_DEFENSE) != stats_added.end()){
        defense_added = spell.getStatistic(STATS_ADDED);
    };

    state.push_back(attack_added);
    state.push_back(defense_added);

    return state;
}

std::vector<float> NEATBattleController::generateHeroStateVector(const Hero& hero) const{

    /*

    Order:

    ATTACK
    DEFENSE
    MANA
    SPEED


    SPELL AT INDEX 0 STATE
    ...
    SPELL AT INDEX 3 STATE

    TOTAL (HERO ONLY): 4
    TOTAL: 24

    */

    vector<float> state;
    state.reserve(ai_battle_controller_constants::HERO_AND_SPELL_VECTOR_SIZE);


    float morale = hero.getStatistic(MORALE);
    state.push_back(hero.getStatistic(HERO_ATTACK) + morale);
    state.push_back(hero.getStatistic(HERO_DEFENSE) + morale);
    state.push_back(hero.getManaPoints());
    state.push_back(hero.getStatistic(HERO_SPEED));


    for(int i = 0; i< MAX_NUMBER_OF_SPELLS; ++i){
        vector<float> spell_state;
        if(hero.hasSpellAt(i)){
            spell_state = generateSpellStateVector(hero.getSpellAt(i), hero);
        }
        else{
            spell_state.assign(ai_battle_controller_constants::SPELL_STATE_VECTOR_SIZE, ai_battle_controller_constants::NO_DATA_INDICATOR);
        }
        state.insert(state.end(), spell_state.begin(), spell_state.end());
    }

    return state;

}


const vector<float> NEATBattleController::predict(const vector<float>& input){
    return network_.propagate(input);
};

const std::vector<BattleAction>& NEATBattleController::getPerformedActionsVector() const{
    return performed_actions_;
}
