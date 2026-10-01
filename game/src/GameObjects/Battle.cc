/**
 * @file Battle.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Battle.
 */

#include "GameObjects/Battle.h"
#include "GameObjects/TileMaps/Battlefield.h"
#include "GameObjects/Army.h"
#include "Managers/GraphicsManager.h"
#include "Managers/IGraphicsManager.h"
#include "Managers/PlayerManager.h"
#include <memory>

using namespace std;
using namespace constants;

Battle::Battle(Army& attacking_army, Army& defending_army,
    BattleController& attacking_army_controller,
    BattleController& defending_army_controller,
    TileType battlefield_type):
    attacking_army_(attacking_army),
    defending_army_(defending_army),
    attacker_battle_controller_(attacking_army_controller),
    defender_battle_controller_(defending_army_controller),
    attacker_hero_(attacking_army.getArmyHeroPtr()),
    defender_hero_(defending_army.getArmyHeroPtr()),
    battle_turn_(0){

    battlefield_ = std::make_unique<Battlefield>(attacking_army, defending_army, battlefield_type);
    getBattlefield().initializeMap();

    registerThisToBattleElements();

    attacking_player_id_ =  attacking_army.getPlayerID();
    defending_player_id_ =  defending_army.getPlayerID();

    if (attacking_army.getArmyHeroPtr() != nullptr){
        attacking_army.getArmyHeroPtr()->resetManaPoints();
    }

    if (defending_army.getArmyHeroPtr() != nullptr){
        defending_army.getArmyHeroPtr()->resetManaPoints();
    }

    getBattlefield().spawnStacksOnBattlefield(attacking_army, defending_army);
    generateBattleOrder(attacking_army, defending_army);

}

void Battle::registerThisToBattleElements(){

    getAttackingArmy().registerBattle(this);
    getDefendingArmy().registerBattle(this);
    getBattlefield().registerBattle(this);
    attacker_battle_controller_.registerBattle(this);
    defender_battle_controller_.registerBattle(this);

}

void Battle::deregisterThisFromBattleElements(){

    getAttackingArmy().deregisterBattle();
    getDefendingArmy().deregisterBattle();
    getBattlefield().deregisterBattle();
    attacker_battle_controller_.deregisterBattle();
    defender_battle_controller_.deregisterBattle();
}

int Battle::performBattleLoop(){

    Army& attacker = getAttackingArmy();
    Army& defender = getDefendingArmy();
    int result = 0;

    while(result == 0){
        incrementBattleTurnNumber();
        flag_is_player_turn_finished_ = false;
        BattleObject& current_battle_object = getCurrentBattleObject();
        current_player_id_ = current_battle_object.getPlayerID();
        BattleController& current_player_controller = getCurrentBattleController();
        if(isBattleObjectHero(current_battle_object)){
            if (!getCurrentArmy().getArmyHeroPtr()->canCastAnySpell()){
                if(GraphicsManager::getGraphicsManagerType() == REAL_GRAPHICS_MANAGER){
                    cout << "Hero turn skipped - Hero cannot cast any spell" << endl;
                }
                endTurnPrematurely();
            }
            else{
                current_player_controller.makeSpellDecision();
            }

            if(!flag_is_player_turn_finished_){
                current_player_controller.makeBattleDecision();
            }
        }

        else{
            current_player_controller.makeBattleDecision();
        }

        checkForDestroyedStacks(attacker);
        if(!attacker.isArmyActive()){
            result = -1;
        }
        checkForDestroyedStacks(defender);
        if(!defender.isArmyActive()){
            result = 1;
        }

        if(!attacker.isArmyActive() && !defender.isArmyActive()){
            throw BattleLogicError("Both armies were destroyed in the same turn - this is not intended behaviour");
        };

        if(battle_turn_ >= MAX_NUMBER_OF_TURNS_IN_BATTLE){
            if(GraphicsManager::getGraphicsManagerType() == REAL_GRAPHICS_MANAGER){
                cout << "Info: Battle ended prematurely, because it lasted more than " << MAX_NUMBER_OF_TURNS_IN_BATTLE << " turns" << endl;
            }
            result = roll(0.5f) ? 1 : -1;
        }

    }
    return result;
}

Battlefield& Battle::getBattlefield() const{
    if (battlefield_ != nullptr){
        return *battlefield_;
    }
    throw BattleLogicError("Tried to access uninitialized Battlefield when no battle was happening while calling this method.");
}

bool Battle::isBattlefieldInitialized() const{
    return battlefield_ != nullptr;
}

bool Battle::isBattleInProgress() const{
    return isBattlefieldInitialized();
}

BattleObject& Battle::getCurrentBattleObject() const{
    return *getBattleOrder().front();
}

void Battle::incrementBattleTurnNumber(){
    ++battle_turn_;
}

const vector<shared_ptr<BattleObject>>& Battle::getBattleOrder() const{
    return battle_order_;
}

void Battle::generateBattleOrder(Army& attacker, Army& defender){

    vector<shared_ptr<BattleObject>> attacker_objects = attacker.getBattleObjectsOfArmy();
    vector<shared_ptr<BattleObject>> defender_objects = defender.getBattleObjectsOfArmy();

    battle_order_.insert(battle_order_.end(), attacker_objects.begin(), attacker_objects.end());
    battle_order_.insert(battle_order_.end(), defender_objects.begin(), defender_objects.end());

    stable_sort(battle_order_.begin(), battle_order_.end(),
        [](const shared_ptr<BattleObject>& a, const shared_ptr<BattleObject>& b){
            return *a < *b;
        });
}

bool Battle::isBattleObjectHero(const BattleObject& battle_object_to_check) const{
    return ((attacker_hero_.get() != nullptr && attacker_hero_.get() == &battle_object_to_check) ||
        (defender_hero_.get() != nullptr && defender_hero_.get() == &battle_object_to_check));
}

vector<shared_ptr<Stack>>::iterator Battle::removeStackFromGame(Stack& stack_to_remove, Army& army_of_troop){

    getBattlefield().getTileAt(stack_to_remove.getXPos(), stack_to_remove.getYPos()).removeObjectOnTile();

    auto it = find_if(battle_order_.begin(), battle_order_.end(), [&](const shared_ptr<BattleObject>& stack){
        return stack.get() == &stack_to_remove;
    });

    if(it != battle_order_.end()){
        battle_order_.erase(it);
    }
    else{
        throw InvalidBattleObjectError(stack_to_remove.toString(), "was not registered in BattleManager's battleOrder vector and cannot be removed.");
    }
    return army_of_troop.removeStackFromArmy(stack_to_remove);

}

void Battle::checkForDestroyedStacks(Army& army_to_check){
    auto begin_vec = army_to_check.getArmyStacks().begin();
    for (auto it = begin_vec; it != army_to_check.getArmyStacks().end(); ){
        auto stack = army_to_check.getArmyStacks().at(it-begin_vec);
        if (stack->getFlagDestroyAfterInteraction()){
            it = removeStackFromGame(*stack, army_to_check);
        }
        else{
            ++it;
        }
    }
}

void Battle::performBattleAction(const int tile_x_pos, int const tile_y_pos){
    BattlefieldTile& tile = getBattlefield().getTileAt(tile_x_pos, tile_y_pos);
    if(getCurrentBattleObject().attack(tile)){
        rotate(battle_order_.begin(), battle_order_.begin() + 1, battle_order_.end());
    }
    else{
        cout << "Invalid battle action - try again" << endl;
    }
}

Player& Battle::getCurrentPlayer() const{
    return PlayerManager::getInstance().getPlayerByID(current_player_id_);
}

int Battle::getCurrentPlayerID() const{
    return current_player_id_;
}

Army& Battle::getOppositeArmy(const int player_id) const{
    return player_id == attacking_player_id_ ? getDefendingArmy() : getAttackingArmy();
}

Army& Battle::getOppositeArmy() const{
    return  getOppositeArmy(current_player_id_);
}

Army& Battle::getCurrentArmy() const{
    return getCurrentPlayerID() == getAttackingArmy().getPlayerID() ? getAttackingArmy() : getDefendingArmy();
}

Stack& Battle::getCurrentStack() const{
    for (const auto &stack: getCurrentArmy().getArmyStacks()){
        if(&getCurrentBattleObject() == stack.get()){
            return *stack;
        }
    }
    throw InvalidBattleObjectError(getCurrentBattleObject().toString(), " is not a stack in the current army.");
}

void Battle::endTurnPrematurely(){
    rotate(battle_order_.begin(), battle_order_.begin() + 1, battle_order_.end());
    flag_is_player_turn_finished_ = true;
}

BattleController& Battle::getCurrentBattleController(){
    return current_player_id_ == attacking_player_id_ ? attacker_battle_controller_ : defender_battle_controller_;
}

bool Battle::getFlagIsPlayerTurnFinished() const{
    return flag_is_player_turn_finished_;
}

Army& Battle::getAttackingArmy() const{
    return attacking_army_;
}

Army& Battle::getDefendingArmy() const{
    return defending_army_;
}

Battle::~Battle(){

    deregisterThisFromBattleElements();

}
