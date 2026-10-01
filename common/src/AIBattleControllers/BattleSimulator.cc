/**
 * @file BattleSimulator.cc
 * @author Bartosz Han
 * @brief This file contains definition of class BattleSimulator.
 *
 */


#include "AIBattleControllers/BattleSimulator.h"
#include "Data/Constants.hpp"
#include "GameObjects/Army.h"
#include "GameObjects/BaseObjects/BattleObject.h"
#include <algorithm>
#include <cmath>
#include <cstddef>

using namespace std;
using namespace constants;

BattleSimulator::BattleSimulator(Battle& battle):battle_(battle){
    for(const auto& stack: battle.getAttackingArmy().getArmyStacks()){
        initial_pos_[stack] = {stack->getXPos(), stack->getYPos()};
        mock_health_[stack.get()] = stack->getCurrentKnownHealth();
        all_stacks_.push_back(stack.get());
    };

    for(const auto& stack: battle.getDefendingArmy().getArmyStacks()){
        initial_pos_[stack] = {stack->getXPos(), stack->getYPos()};
        mock_health_[stack.get()] = stack->getCurrentKnownHealth();
        all_stacks_.push_back(stack.get());
    };

    for(const auto& battle_object: battle.getBattleOrder()){
        if(battle_object->getBattleObjectType() != HERO){
            battle_order_.push_back(battle_object.get());
        }
    };
};

Battle& BattleSimulator::getBattle() const{
    return battle_;
}

Stack& BattleSimulator::getCurrentStack() const{
    BattleObject* current_battle_object = battle_order_.front();

    auto it = find(all_stacks_.begin(), all_stacks_.end(), current_battle_object);
    if(it != all_stacks_.end()){
        return **it;
    }
    throw InvalidBattleObjectError(current_battle_object->toString(), " was not found in current battle");
}

void BattleSimulator::rotateBattleOrder(){
    rotate(battle_order_.begin(), battle_order_.begin() + 1, battle_order_.end());
}

void BattleSimulator::moveCurrentStack(Stack& current_stack, std::vector<WorldMove> path){
    int moves_left = current_stack.getMaxMoves();
    for(const WorldMove& move: path){
        getBattle().getBattlefield().moveObject(current_stack, move);
        moves_left -= getBattle().getBattlefield().getTileAt(current_stack.getXPos(), current_stack.getYPos()).getMoveCost();
        if(moves_left <= 0){
            break;
        }
    }
}


void BattleSimulator::performMockBattleAction(int event_x_pos, int event_y_pos){

    Stack& current_stack = getCurrentStack();
    if((event_x_pos == END_TURN_INDICATOR.first && event_y_pos == END_TURN_INDICATOR.second) || (event_x_pos == current_stack.getXPos() && event_y_pos == current_stack.getYPos())){
        rotateBattleOrder();
        return;
    }

    Tile& event_tile = getBattle().getBattlefield().getTileAt(event_x_pos, event_y_pos);

    if(!event_tile.hasInteractiveObject()){
        //Walk to action
        auto path_data = getBattle().getBattlefield().findPathToTile(
            current_stack.getXPos(), current_stack.getYPos(), event_x_pos, event_y_pos);

        moveCurrentStack(current_stack, path_data.first);
    }
    else{
        //Walk and attack - if object on tile is enemy
        for (auto enemy_stack: getBattle().getOppositeArmy(current_stack.getPlayerID()).getArmyStacks()){
            if(isStackActive(*enemy_stack) && enemy_stack->getXPos() == event_x_pos && enemy_stack->getYPos() == event_y_pos){


                auto path_data = getBattle().getBattlefield().findPathToTile(
                    current_stack.getXPos(), current_stack.getYPos(), event_x_pos, event_y_pos,
                    getBattle().getBattlefield().generateStackEndConditionFunction(current_stack));

                moveCurrentStack(current_stack, path_data.first);

                if(getBattle().getBattlefield().canStackAttackFrom(
                    current_stack, current_stack.getXPos(), current_stack.getYPos(), event_x_pos, event_y_pos)){

                    dealMockDamage(current_stack, *enemy_stack);
                }
                break;
            }
        }
    }
    rotateBattleOrder();
}


void BattleSimulator::dealMockDamage(Stack& attacker, Stack& enemy){

    int attacker_quantity = getMockStackTroopQuantity(attacker);
    float damage_multiplier = attacker.calculateDamageMultiplierTowardsEnemyStack(enemy);
    int damage = attacker_quantity * attacker.getAverageDamagePerTroop() * damage_multiplier;
    mock_health_.at(&enemy) -= damage;

    if(mock_health_.at(&enemy) <= 0){
        removeStackFromSimulation(enemy);
    }

}

void BattleSimulator::removeStackFromSimulation(Stack& stack_to_remove){
    auto it = find(battle_order_.begin(), battle_order_.end(), &stack_to_remove);
    if(it != battle_order_.end()){
        battle_order_.erase(it);
    }
    mock_health_.at(&stack_to_remove) = 0;
    getBattle().getBattlefield().getTileAt(stack_to_remove.getXPos(), stack_to_remove.getYPos()).removeObjectOnTile();
}

bool BattleSimulator::isMockBattleFinished() const{
    int first_object_player_id = battle_order_.front()->getPlayerID();
    for(size_t i=1; i<battle_order_.size(); ++i){
        if(battle_order_.at(i)->getPlayerID() != first_object_player_id){
            //There are two objects with different player ID's - so both armies are active.
            return false;
        }
    }
    return true;
}

bool BattleSimulator::isStackActive(const Stack& stack) const{
    return find(battle_order_.begin(), battle_order_.end(), &stack) != battle_order_.end();
}

std::pair<float, float> BattleSimulator::calculateSimulationSumOfAttacks(){
    float mock_current_army_sum_of_damages = 0.0f;
    float mock_enemy_army_sum_of_damages = 0.0f;

    for(const auto& stack: getBattle().getCurrentArmy().getArmyStacks()){
        if(isStackActive(*stack)){
            int mock_quantity = getMockStackTroopQuantity(*stack);
            mock_current_army_sum_of_damages += stack->getCurrentAverageStackDamage(mock_quantity);
        }
    }

    for(const auto& stack: getBattle().getOppositeArmy().getArmyStacks()){
        if(isStackActive(*stack)){
            int mock_quantity = getMockStackTroopQuantity(*stack);
            mock_enemy_army_sum_of_damages += stack->getCurrentAverageStackDamage(mock_quantity);
        }
    }

    return {mock_current_army_sum_of_damages, mock_enemy_army_sum_of_damages};
}

void BattleSimulator::restoreBattleState(){

    for(const auto& stack: getBattle().getAttackingArmy().getArmyStacks()){
        if(isStackActive(*stack)){
            getBattle().getBattlefield().getTileAt(stack->getXPos(), stack->getYPos()).removeObjectOnTile();
        }
    }

    for(const auto& stack: getBattle().getDefendingArmy().getArmyStacks()){
        if(isStackActive(*stack)){
            getBattle().getBattlefield().getTileAt(stack->getXPos(), stack->getYPos()).removeObjectOnTile();
        }
    }

    for(const auto& pair: initial_pos_){
        auto object = pair.first;
        int x_pos = pair.second.first;
        int y_pos = pair.second.second;
        getBattle().getBattlefield().moveObject(object, x_pos, y_pos);
    }
}

int BattleSimulator::getMockStackHealth(const Stack& stack) const{
    return mock_health_.at(&stack);
}
int BattleSimulator::getMockStackTroopQuantity(const Stack& stack) const{
    return stack.healthToQuantity(getMockStackHealth(stack));
}

int BattleSimulator::calculatePotentialLossInAttack(const Stack& stack, const int damage) const{
    int previous_health = getMockStackHealth(stack);
    float previous_attack = getMockStackTroopQuantity(stack) * stack.getAverageDamagePerTroop();
    int potential_new_health = previous_health - damage;
    float new_attack = stack.healthToQuantity(potential_new_health) * stack.getAverageDamagePerTroop();
    return previous_attack - new_attack;
}


pair<int, int> BattleSimulator::chooseNextMove() const{

    vector<pair<Stack*, float>> attack_loss;
    Stack& mock_current_stack = getCurrentStack();

    for(const auto& enemy_stack: getBattle().getOppositeArmy(mock_current_stack.getPlayerID()).getArmyStacks()){
        if(isStackActive(*enemy_stack)){
            float damage_multiplier = mock_current_stack.calculateDamageMultiplierTowardsEnemyStack(*enemy_stack);
            int expected_damage = damage_multiplier * getMockStackTroopQuantity(mock_current_stack) * mock_current_stack.getAverageDamagePerTroop();
            float attack_delta = calculatePotentialLossInAttack(*enemy_stack, expected_damage);
            attack_loss.push_back({enemy_stack.get(), attack_delta});
        }
    }

    sort(attack_loss.begin(), attack_loss.end(), [](auto& t1, auto& t2){
        return t1.second > t2.second;
    });

    //Attempt to attack
    for(const auto& [enemy_stack, health]: attack_loss){
        if(mock_current_stack.canAttackEnemyStackThisTurn(*enemy_stack)){
            return {enemy_stack->getXPos(), enemy_stack->getYPos()};
        }
    }

    //Attempt to get closer
    for(const auto& [enemy_stack, health]: attack_loss){
        if(mock_current_stack.canGetCloserToEnemyStackThisTurn(*enemy_stack)){
            return {enemy_stack->getXPos(), enemy_stack->getYPos()};
        }
    }

    //Go forward
    vector<pair<int, int>> walkable_tiles = getBattle().getBattlefield().TileMap::getTilesWhereObjectCanGoTo(mock_current_stack);

    int target_x_pos = mock_current_stack.getPlayerID() == getBattle().getAttackingArmy().getPlayerID() ? BATTLEFIELD_WIDTH - 1 : 0;
    int target_y_pos = mock_current_stack.getYPos();

    float best_distance = INFINITY;
    int best_x_pos = mock_current_stack.getXPos();
    int best_y_pos = mock_current_stack.getYPos();

    for(const auto& tile: walkable_tiles){
        float distance = getBattle().getBattlefield().getDistanceBetweenTiles(
            tile.first, tile.second, target_x_pos, target_y_pos);
        if(distance < best_distance){
            best_distance = distance;
            best_x_pos = tile.first;
            best_y_pos = tile.second;
        }
    }
    return {best_x_pos, best_y_pos};
}


void BattleSimulator::simulate(int n_turns){

    for(int i=0; i<n_turns; ++i){
        pair<int, int> move = chooseNextMove();
        performMockBattleAction(move.first, move.second);
        if(isMockBattleFinished()){
            return;
        }
    }
}
