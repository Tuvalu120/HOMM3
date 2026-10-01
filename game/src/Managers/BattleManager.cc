/**
 * @file BattleManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class BattleManager.
 *
 */


#include <memory>

#include "Managers/BattleManager.h"
#include "Data/ObjectDataEnums.hpp"
#include "GameObjects/Hero.h"
#include "Managers/IGraphicsManager.h"
#include "Managers/PlayerManager.h"
#include "Managers/GraphicsManager.h"
#include "Managers/WorldManager.h"
#include "GameObjects/Army.h"

using namespace std;
using namespace game_data;
using namespace constants;

BattleManager::BattleManager(): SubManager(){};

int BattleManager::battle(Army& attacker, Army& defender){

    initializeBattle(attacker, defender);
    int result = getCurrentBattle().performBattleLoop();
    endBattle(result);
    return result;

}

int BattleManager::battle(Army& attacker, Army& defender, BattleController& attacker_controller, BattleController& defender_controller, TileType battlefield_style){
    current_battle_ = make_unique<Battle>(attacker, defender, attacker_controller, defender_controller, battlefield_style);
    GraphicsManager::getInstance().setToScene(BATTLE);
    return getCurrentBattle().performBattleLoop();
}


void BattleManager::initializeBattle(Army& attacker, Army& defender){

    int attacking_player_id = attacker.getPlayerID();
    int defending_player_id = defender.getPlayerID();

    auto attacker_battle_controller = attacking_player_id != -1 ? PlayerManager::getInstance().getPlayerByID(attacking_player_id).getPlayerControllerPtr() :
        PlayerManager::getInstance().getNeutralPlayerControllerPtr();

    auto defender_battle_controller = defending_player_id != -1 ? PlayerManager::getInstance().getPlayerByID(defending_player_id).getPlayerControllerPtr() :
        PlayerManager::getInstance().getNeutralPlayerControllerPtr();

    TileType battlefield_type = WorldManager::getInstance().getTileTypeOfCurrentArmyTile();

    current_battle_ = make_unique<Battle>(attacker, defender, *attacker_battle_controller, *defender_battle_controller, battlefield_type);

    attacking_army_id_ =    attacker.isWorldArmy() ? PlayerManager::getInstance().getPlayerByID(attacking_player_id).getArmyID(attacker) : -1;
    defending_army_id_ =    defender.isWorldArmy() ? PlayerManager::getInstance().getPlayerByID(defending_player_id).getArmyID(defender) : -1;

    defender.setArmyExperienceReward();

    GraphicsManager::getInstance().setToScene(BATTLE);

}

Battle& BattleManager::getCurrentBattle() const{
    if(current_battle_ != nullptr){
        return *current_battle_;
    }
    throw UninitializedObjectError("Battle");
}

bool BattleManager::isBattleInProgress() const{
    if(current_battle_ == nullptr){
        return false;
    }
    return getCurrentBattle().isBattleInProgress();
}

void BattleManager::endBattle(const int result){


    GraphicsManager::getInstance().setToScene(WORLD);
    Army& attacking_army = getAttackingArmy();
    Army& defending_army = getDefendingArmy();
    current_battle_.reset();

    if (result == 1){

        int experience_points_gain = defending_army.getArmyExperienceReward();
        attacking_army.resetStacksData();
        if (defending_army_id_ != -1){
            int defending_player_id = defending_army.getPlayerID();
            PlayerManager::getInstance().getPlayerByID(defending_player_id).removeArmy(defending_army_id_);
            PlayerManager::getInstance().checkForEliminatedPlayer(defending_player_id, false);
        }
        //WorldManager::getCurrentPlayer().getCurrentArmy() is always the attacker.
        if (attacking_army_id_ != -1){
            WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().addExperiencePoints(experience_points_gain);
        }
    }
    else if (result == -1){
        defending_army.resetStacksData();
        if (attacking_army_id_ != -1){
            int attacking_player_id = attacking_army.getPlayerID();
            PlayerManager::getInstance().getPlayerByID(attacking_player_id).removeArmy(attacking_army_id_);
            WorldManager::getInstance().setFlagWasArmyEliminatedDuringItsTurn();
            PlayerManager::getInstance().checkForEliminatedPlayer(attacking_player_id, true);
        }
    }
}

Army& BattleManager::getAttackingArmy() const{
    return getCurrentBattle().getAttackingArmy();
}

Army& BattleManager::getDefendingArmy() const{
    return getCurrentBattle().getDefendingArmy();
}

void BattleManager::resetManager(){

    current_battle_.reset();
    attacking_army_id_ = -1;
    defending_army_id_ = -1;
}
