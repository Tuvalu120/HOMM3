/**
 * @file AIBattleController.cc
 * @author Bartosz Han
 * @brief This file contains definition of class AIBattleController.
 *
 */

#include <algorithm>
#include <stdexcept>

#include "AIBattleControllers/AIBattleController.h"
#include "Data/Constants.hpp"
#include "Data/ObjectDataEnums.hpp"
#include "Managers/BattleManager.h"
#include "GameObjects/Army.h"
#include "PlayerControllers/PlayerController.h"

using namespace std;

AIBattleController::AIBattleController():spell_target_(NO_SPELL_TARGET){};

vector<reference_wrapper<Stack>> AIBattleController::getEnemyStacksInOrder(
    function<float(Stack&)> score_function) const{

        vector<shared_ptr<Stack>> stacks = getBattle().getOppositeArmy().getArmyStacks();
        vector<pair<reference_wrapper<Stack>, float>> stacks_val_vec;

        for(const auto& stack : stacks){
            float score = score_function(*stack);
            stacks_val_vec.push_back({*stack, score});
        }

        stable_sort(stacks_val_vec.begin(), stacks_val_vec.end(), [](auto& t1, auto& t2){
            return t1.second < t2.second;
        });

        vector<reference_wrapper<Stack>> ordered_stacks;
        for (const auto& stack_val: stacks_val_vec){
            if(stack_val.second != 0){
                ordered_stacks.push_back(stack_val.first);
            }
        }

        return ordered_stacks;
}

vector<pair<int,int>> AIBattleController::getWalkableTilesInOrder(
    function<float(const std::pair<int,int>&)> score_function, std::function<std::pair<int,int>()> edge_case_heuristic) const{

        vector<pair<int, int>> valid_tiles = getBattle().getBattlefield().getTilesWhereObjectCanGoTo(false);

        vector<pair<pair<int, int>, float>> tiles_val_vec;

        for(const auto& tile: valid_tiles){
            float score = score_function(tile);
            tiles_val_vec.push_back({tile, score});
        }

        stable_sort(tiles_val_vec.begin(), tiles_val_vec.end(), [](auto& t1, auto& t2){
            return t1.second < t2.second;
        });

        vector<pair<int,int>> ordered_tiles;
        for(const auto& tile_val: tiles_val_vec){
            if(tile_val.second != 0){
                ordered_tiles.push_back(tile_val.first);
            }
        }

        //If no tile meet conditions, use other heuristic
        if(ordered_tiles.empty()){
            ordered_tiles.push_back(edge_case_heuristic());
        }

        return ordered_tiles;
}

pair<int, int> AIBattleController::attackEnemyWithCondition(const Stack& current_stack,
    function<float(Stack&)> score_function, std::function<std::pair<int,int>()> edge_case_heuristic) const{

    auto sorted_enemy_stacks = getEnemyStacksInOrder(score_function);

    for (const auto& stack: sorted_enemy_stacks){
        if(current_stack.canAttackEnemyStackThisTurn(stack)){
            return make_pair<int, int>(stack.get().getXPos(), stack.get().getYPos());
        }
    }

    //If there are no enemies that can be attacked now, get closer to enemy, provided path can be found to this stack.
    for (const auto& stack: sorted_enemy_stacks){
        int x_pos = stack.get().getXPos();
        int y_pos = stack.get().getYPos();
        if(current_stack.canGetCloserToEnemyStackThisTurn(x_pos, y_pos)){
            return make_pair<int, int>(stack.get().getXPos(), stack.get().getYPos());
        }
    }

    //In rare cases, when there isn't path to any enemy stack, use some other heuristic.
    return edge_case_heuristic();
}

pair<int, int> AIBattleController::attackEnemyWithCondition(const Stack& current_stack,
    function<float(Stack&)> score_function) const{
        return attackEnemyWithCondition(current_stack, score_function, [this](){ return attackEnemyForBiggestAttackLoss();});
    };

pair<int, int> AIBattleController::attackClosestEnemy() const{
    Stack& current_stack = getBattle().getCurrentStack();

    int stack_x_pos = current_stack.getXPos();
    int stack_y_pos = current_stack.getYPos();

    function<float(const Stack&)> score_function([stack_x_pos, stack_y_pos, this](const Stack& s){
        return getBattle().getBattlefield().getWalkingDistanceBetweenTiles(stack_x_pos, stack_y_pos, s.getXPos(), s.getYPos());
    });

    return attackEnemyWithCondition(current_stack, score_function);
}

pair<int, int> AIBattleController::attackFurthestEnemy() const{
    Stack& current_stack = getBattle().getCurrentStack();

    int stack_x_pos = current_stack.getXPos();
    int stack_y_pos = current_stack.getYPos();

    function<float(const Stack&)> score_function([stack_x_pos, stack_y_pos, this](const Stack& s){
        return -1 * getBattle().getBattlefield().getWalkingDistanceBetweenTiles(stack_x_pos, stack_y_pos, s.getXPos(), s.getYPos());
    });

    return attackEnemyWithCondition(current_stack, score_function);
}

pair<int, int> AIBattleController::attackEnemyWithLowsetHealth() const{
    Stack& current_stack = getBattle().getCurrentStack();

    function<float(const Stack&)> score_function([](const Stack& s){
        return s.getCurrentKnownHealth();
    });

    return attackEnemyWithCondition(current_stack, score_function);
}

pair<int, int> AIBattleController::attackEnemyWithBiggestHealth() const{

    Stack& current_stack = getBattle().getCurrentStack();

    function<float(const Stack&)> score_function([](const Stack& s){
        return -1 * s.getCurrentKnownHealth();
    });

    return attackEnemyWithCondition(current_stack, score_function);

}

pair<int, int> AIBattleController::attackEnemyWithLowsetAttack() const{
    Stack& current_stack = getBattle().getCurrentStack();

    function<float(const Stack&)> score_function([](const Stack& s){
        return s.getAverageDamagePerTroop() * s.getQuantity();
    });

    return attackEnemyWithCondition(current_stack, score_function);
}

pair<int, int> AIBattleController::attackEnemyWithBiggestAttack() const{
    Stack& current_stack = getBattle().getCurrentStack();

    function<float(const Stack&)> score_function([](const Stack& s){
        return -1 * s.getAverageDamagePerTroop() * s.getQuantity();
    });

    return attackEnemyWithCondition(current_stack, score_function, [this](){return goForward();});
}

std::pair<int, int> AIBattleController::attackEnemyForBiggestAttackLoss() const{
    Stack& current_stack = getBattle().getCurrentStack();

    function<float(const Stack&)> score_function([&current_stack](const Stack& s){
        float damage = current_stack.calculateAverageStackDamage(s);
        int new_quantity = s.receiveMockDamage(damage);
        int quantity_delta = s.getQuantity() - new_quantity;
        return -1 * quantity_delta * s.getAverageDamagePerTroop();
    });

    return attackEnemyWithCondition(current_stack, score_function, [this](){return attackEnemyWithBiggestAttack();});
}

std::pair<int, int> AIBattleController::executeEnemyWithBiggestAttack() const{
    Stack& current_stack = getBattle().getCurrentStack();
    Army& enemy_army = getBattle().getOppositeArmy();

    Stack* strongest_enemy_stack = nullptr;
    int strongest_enemy_attack = -1;

    for(const auto& enemy_stack: enemy_army.getArmyStacks()){
        int max_damage = current_stack.calculateMinimumDamage(*enemy_stack);
        if(current_stack.canAttackEnemyStackThisTurn(*enemy_stack) && enemy_stack->getCurrentKnownHealth() <= max_damage){
            int enemy_stack_dmg = enemy_stack->getCurrentAverageStackDamage();
            if(enemy_stack_dmg >= strongest_enemy_attack){
                strongest_enemy_stack = enemy_stack.get();
                strongest_enemy_attack = enemy_stack_dmg;
            }
        }
    };

    if(strongest_enemy_stack != nullptr){
        return {strongest_enemy_stack->getXPos(), strongest_enemy_stack->getYPos()};
    }

    return attackEnemyWithLowsetHealth();
}

std::pair<int, int> AIBattleController::attackEnemyWithBiggestRange() const{
    Stack& current_stack = getBattle().getCurrentStack();
    Army& enemy_army = getBattle().getOppositeArmy();

    Stack* strongest_enemy_stack = nullptr;
    vector<Stack*> all_enemy_stacks;
    int max_range = -1;
    int strongest_enemy_attack = -1;

    for(const auto& enemy_stack: enemy_army.getArmyStacks()){
        all_enemy_stacks.push_back(enemy_stack.get());
        if(current_stack.canAttackEnemyStackThisTurn(*enemy_stack)){
            int enemy_stack_dmg = enemy_stack->getCurrentAverageStackDamage();
            int enemy_range = enemy_stack->getStatistic(RANGE);
            if((enemy_range > max_range) || (enemy_range == max_range && enemy_stack_dmg > strongest_enemy_attack)){
                strongest_enemy_stack = enemy_stack.get();
                max_range = enemy_range;
                strongest_enemy_attack = enemy_stack_dmg;
            }
        }
    };

    if(strongest_enemy_stack != nullptr){
        return {strongest_enemy_stack->getXPos(), strongest_enemy_stack->getYPos()};
    }

    stable_sort(all_enemy_stacks.begin(), all_enemy_stacks.end(), [](Stack* s1, Stack* s2){
        if(s1->getStatistic(RANGE) != s2->getStatistic(RANGE)){
            return s1->getStatistic(RANGE) > s2->getStatistic(RANGE);
        }
        else{
            return s1->getCurrentAverageStackDamage() > s2->getCurrentAverageStackDamage();
        }
    });

    for(const Stack* enemy_stack: all_enemy_stacks){
        if(current_stack.canGetCloserToEnemyStackThisTurn(enemy_stack->getXPos(), enemy_stack->getYPos())){
            return {enemy_stack->getXPos(), enemy_stack->getYPos()};
        }
    }

    return attackEnemyForBiggestAttackLoss();

}

std::pair<int, int> AIBattleController::attackEnemyWhichCanKillMyStack() const{
    Stack& current_stack = getBattle().getCurrentStack();
    function<float(const Stack&)> score_function([this](const Stack& s){
        int my_attack_loss = 0;

        //Opposite of opposite army is current army
        for(const auto& stack: getBattle().getCurrentArmy().getArmyStacks()){
            if(s.canAttackEnemyStackThisTurn(*stack)){
                float damage = s.calculateMinimumDamage(*stack);
                if(stack->getCurrentKnownHealth() <= damage){
                    my_attack_loss += stack->getCurrentAverageStackDamage();
                }
            }
        }
        return -1 * my_attack_loss;
    });
    return attackEnemyWithCondition(current_stack, score_function, [this](){return attackEnemyWithBiggestAttack();});
}

std::pair<int, int> AIBattleController::attackEnemyWhichCanReduceMyAttackByMost() const{

    Stack& current_stack = getBattle().getCurrentStack();

    function<float(const Stack&)> score_function([this](const Stack& t){
        float max_attack_loss = 0;
        for(const auto& stack: getBattle().getCurrentArmy().getArmyStacks()){
            if(t.canAttackEnemyStackThisTurn(*stack)){
                float damage = t.calculateAverageStackDamage(*stack);
                int new_quantity = stack->receiveMockDamage(damage);
                int quantity_delta = stack->getQuantity() - new_quantity;
                float current_attack_loss = quantity_delta * stack->getAverageDamagePerTroop();
                max_attack_loss = max(max_attack_loss, current_attack_loss);
            }
        }
        return -1 * max_attack_loss;

    });
    return attackEnemyWithCondition(current_stack, score_function, [this](){return attackEnemyWithBiggestAttack();});

}

pair<int, int> AIBattleController::attackEnemyWhichIsNextInOrder() const{
    Stack& current_stack = getBattle().getCurrentStack();
    auto battle_order = getBattle().getBattleOrder();
    for(const auto& battle_object: battle_order){
        if(battle_object->getBattleObjectType() == STACK && battle_object->getPlayerID() != current_stack.getPlayerID()){

            for(const auto& enemy_stack: getBattle().getOppositeArmy().getArmyStacks()){
                if(enemy_stack == battle_object){
                    if(current_stack.canAttackEnemyStackThisTurn(*enemy_stack)){
                        return {enemy_stack->getXPos(), enemy_stack->getYPos()};
                    }
                }
            }
        }
    }
    return attackClosestEnemy();
}

std::pair<int, int> AIBattleController::protectStrongestFriendlyStack() const{

    Stack& current_stack = getBattle().getCurrentStack();
    int best_attack_value = -1;
    Stack* best_stack = nullptr;

    //Get best friendly Stack
    for(const auto& stack: getBattle().getCurrentArmy().getArmyStacks()){
        int attack_value = stack->getCurrentAverageStackDamage();
        if(attack_value > best_attack_value){
            best_stack = stack.get();
            best_attack_value = attack_value;
        }
    };

    if(&current_stack == best_stack){ //Current Stack is best - to protect it, we should hide it.
        return hideStack();
    }


    int current_stack_x_pos = current_stack.getXPos();
    int current_stack_y_pos = current_stack.getYPos();

    int best_stack_x_pos = best_stack->getXPos();
    int best_stack_y_pos = best_stack->getYPos();

    pair<int, int> best_cover_score = getBattle().getBattlefield().getCoverValueOfTile(
            best_stack_x_pos, best_stack_y_pos);;
    pair<int, int> best_tile = {current_stack_x_pos, current_stack_y_pos};

    vector<pair<int, int>> valid_tiles = getBattle().getBattlefield().getTilesWhereObjectCanGoTo();


    for(const pair<int, int>& tile: valid_tiles){

        if(tile.first == current_stack_x_pos && tile.second == current_stack_y_pos){
            continue;
        }

        getBattle().getBattlefield().moveObject(current_stack, tile.first, tile.second);

        pair<int, int> current_cover_score = getBattle().getBattlefield().getCoverValueOfTile(
            best_stack_x_pos, best_stack_y_pos);

        bool is_new_best_value = false;
        if(current_cover_score.first != best_cover_score.first){
            is_new_best_value = current_cover_score.first < best_cover_score.first;
        }
        else{
            is_new_best_value = current_cover_score.second >= best_cover_score.second;
        }
        if(is_new_best_value){
            best_cover_score = current_cover_score;
            best_tile = tile;
        }

        getBattle().getBattlefield().moveObject(current_stack, current_stack_x_pos, current_stack_y_pos);
    }

    if(current_stack_x_pos == best_tile.first && current_stack_y_pos == best_tile.second){
        return attackEnemyWithLowsetAttack(); //This is to prevent AI from being infinitefly stuck.
    }
    return best_tile;
}


pair<int, int> AIBattleController::goForward() const{
    Stack& current_stack = getBattle().getCurrentStack();
    int target_x_pos = current_stack.getPlayerID() == getBattle().getAttackingArmy().getPlayerID() ? constants::BATTLEFIELD_WIDTH - 1 : 0;
    int target_y_pos = current_stack.getYPos();

    function<float(const std::pair<int,int>&)> score_function([target_x_pos, target_y_pos, this](const pair<int,int>& t){
        return getBattle().getBattlefield().getDistanceBetweenTiles(t.first, t.second, target_x_pos, target_y_pos);});

    return getWalkableTilesInOrder(score_function, [&](){return doNothing();}).at(0);
};


pair<int, int> AIBattleController::hideStack() const{

    vector<pair<int, int>> valid_tiles = getBattle().getBattlefield().getTilesWhereObjectCanGoTo(false);
    map<pair<int, int>, pair<int, float>> cover_value;

    for (const pair<int, int>& tile: valid_tiles){
        cover_value[tile] = getBattle().getBattlefield().getCoverValueOfTile(tile.first, tile.second);
    }

    stable_sort(valid_tiles.begin(), valid_tiles.end(),[&cover_value](const pair<int,int>& t1, const pair<int,int>& t2){
        pair<int, float> v1 = cover_value.at(t1);
        pair<int, float> v2 = cover_value.at(t2);
        if(v1.first != v2.first){
            return v1.first < v2.first;
        }
        else{
            return v1.second > v2.second;
        }
    });

    if(valid_tiles.empty()){
        return attackEnemyWithLowsetAttack(); //This is to prevent AI from being infinitefly stuck.
    }

    return valid_tiles.at(0);
}

pair<int, int> AIBattleController::goToFriendlyStacks() const{

    function<float(const std::pair<int,int>&)> score_function([this](const pair<int,int>& t){
        float sum_of_distances = getBattle().getBattlefield().getSumOfDistancesBetweenTileAndCurrentArmy(t.first, t.second);
        sum_of_distances -= getBattle().getBattlefield().getDistanceBetweenTiles(t.first, t.second, getBattle().getCurrentStack().getXPos(),
            getBattle().getCurrentStack().getYPos());
        return sum_of_distances;
    });

    return getWalkableTilesInOrder(score_function, [&](){return attackEnemyForBiggestAttackLoss();}).at(0);
}

pair<int, int> AIBattleController::spreadFromFriendlyStacks() const{

    function<float(const std::pair<int,int>&)> score_function([this](const pair<int,int>& t){
        float sum_of_distances = getBattle().getBattlefield().getSumOfDistancesBetweenTileAndCurrentArmy(t.first, t.second);
        sum_of_distances -= getBattle().getBattlefield().getDistanceBetweenTiles(t.first, t.second, getBattle().getCurrentStack().getXPos(),
            getBattle().getCurrentStack().getYPos());
        return -1 * sum_of_distances;
    });

    return getWalkableTilesInOrder(score_function, [&](){return attackEnemyForBiggestAttackLoss();}).at(0);
}

pair<int, int> AIBattleController::doNothing() const{
    return constants::END_TURN_INDICATOR;
}

pair<int, int> AIBattleController::battleActionToCoordinates(BattleAction action) const{
    switch(action){
        case ATTACK_CLOSEST_ENEMY:
            return attackClosestEnemy();
        case ATTACK_FURTHEST_ENEMY:
            return attackFurthestEnemy();
        case ATTACK_ENEMY_WITH_LOWEST_HEALTH:
            return attackEnemyWithLowsetHealth();
        case ATTACK_ENEMY_WITH_BIGGEST_HEALTH:
            return attackEnemyWithBiggestHealth();
        case ATTACK_ENEMY_WITH_LOWEST_ATTACK:
            return attackEnemyWithLowsetAttack();
        case ATTACK_ENEMY_WITH_BIGGEST_ATTACK:
            return attackEnemyWithBiggestAttack();
        case GO_FORWARD:
            return goForward();
        case HIDE_STACK:
            return hideStack();
        case GO_TO_FRIENDLY_STACKS:
            return goToFriendlyStacks();
        case SPREAD_FROM_FRIENDLY_STACKS:
            return spreadFromFriendlyStacks();
        case ATTACK_ENEMY_FOR_BIGGEST_ATTACK_LOSS:
            return attackEnemyForBiggestAttackLoss();
        case EXECUTE_ENEMY_WITH_BIGGEST_ATTACK:
            return executeEnemyWithBiggestAttack();
        case ATTACK_ENEMY_WITH_BIGGEST_RANGE:
            return attackEnemyWithBiggestRange();
        case ATTACK_ENEMY_WHICH_CAN_KILL_MY_STACK:
            return attackEnemyWhichCanKillMyStack();
        case ATTACK_ENEMY_WHICH_CAN_REDUCE_MY_ATTACK_BY_MOST:
            return attackEnemyWhichCanReduceMyAttackByMost();
        case ATTACK_ENEMY_WHICH_IS_NEXT_IN_ORDER:
            return attackEnemyWhichIsNextInOrder();
        case PROTECT_STRONGEST_FRIENDLY_STACK:
            return protectStrongestFriendlyStack();
        case DO_NOTHING_BATTLE_ACTION:
            return doNothing();
        default:
            throw invalid_argument("Unknown battle action");
    }
}

void AIBattleController::makeSpellDecision(){

    Hero* hero = getBattle().getCurrentArmy().getArmyHeroPtr().get();

    if (hero == nullptr){
        throw std::logic_error("Tried to make spell decision when there wasn't any hero to cast spells");
    }

    SpellCastInfo current_best_spell{
    .spell_index = -1,
    .target_x_pos = -1,
    .target_y_pos = -1};

    float best_score = 0;
    SpellCastInfo best_cast = {current_best_spell};

    for (int i=0; i<constants::MAX_NUMBER_OF_SPELLS; ++i){
        if(hero->canCastSpellAt(i)){
            Spell& spell = hero->getSpellAt(i);
            for(const auto& tile: getBattle().getBattlefield().getTileMap()){
                int current_x_pos = tile->getXPos();
                int current_y_pos = tile->getYPos();

                float score = spell.calculateCastingScore(current_x_pos, current_y_pos);

                if(score > best_score){
                    best_score = score;
                    SpellCastInfo new_best{
                        i, current_x_pos, current_y_pos
                    };
                    best_cast = new_best;
                }
            }
        }
    }

    if(best_score == 0.0f){
        setDecisionVars(constants::END_TURN_INDICATOR);
    }
    else{
        setDecisionVars(best_cast.spell_index);
        spell_target_ = {best_cast.target_x_pos, best_cast.target_y_pos};
    }
    BattleController::makeSpellDecision();

}

void AIBattleController::makeBattleDecision(){
    if(spell_target_ == NO_SPELL_TARGET){ //It is Stack action
        std::pair<int, int> target = selectBattleAction();


        //If, for some reason, target was declared as current Stack's coordinates, skip turn.
        //Few heuristics use current Stacks's coordinates as declaration, that there isn't any legal move, other than skipping turn.
        Stack& current_stack = getBattle().getCurrentStack();
        int stack_x_pos = current_stack.getXPos();
        int stack_y_pos = current_stack.getYPos();

        if(target.first == stack_x_pos && target.second == stack_y_pos){
            target = constants::END_TURN_INDICATOR;
        }

        setDecisionVars(target);
    }
    else{
        setDecisionVars(spell_target_);
        spell_target_ = NO_SPELL_TARGET;
    }
    BattleController::makeBattleDecision();
}
