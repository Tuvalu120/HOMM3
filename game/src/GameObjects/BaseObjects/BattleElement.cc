/**
 * @file BattleElement.cc
 * @author Bartosz Han
 * @brief This file contains definition of class BattleElement.
 */

#include "GameObjects/BaseObjects/BattleElement.h"


Battle& BattleElement::getBattle() const{
    return *battle_;
}

void BattleElement::registerBattle(Battle* battle){
    battle_ = battle;
}

void BattleElement::deregisterBattle(){
    battle_ = nullptr;
}