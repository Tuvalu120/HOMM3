/**
 * @file BattleObject.cc
 * @author Bartosz Han
 * @brief This file contains definition of class BattleObject.
 *
 */


#include "GameObjects/BaseObjects/BattleObject.h"

BattleObject::BattleObject(ObjectType object_type, const int player_id, const int battle_priority):
    OwnedByPlayer(player_id), object_type_(object_type), battle_priority_(battle_priority){}

int BattleObject::getBattlePriority() const{
    return battle_priority_;
}

ObjectType BattleObject::getBattleObjectType() const{
    return object_type_;
}

bool BattleObject::operator<(const BattleObject& other) const{
    return battle_priority_ < other.getBattlePriority();
}
