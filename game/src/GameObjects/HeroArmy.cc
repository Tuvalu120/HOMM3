/**
 * @file HeroArmy.cc
 * @author Bartosz Han
 * @brief This file contains definition of class HeroArmy.
 *
 */

#include "GameObjects/HeroArmy.h"
#include <memory>

using namespace std;

HeroArmy::HeroArmy(std::shared_ptr<Hero> const army_hero):
    Army(),hero_(army_hero){};

HeroArmy::HeroArmy(shared_ptr<Hero> const army_hero,
    const int init_pos_x, const int init_pos_y, const bool is_world_army):
    Army(army_hero->getCurrentTextureName(), init_pos_x, init_pos_y, is_world_army),
    hero_(army_hero){};

Hero& HeroArmy::getArmyHero() const{
    return *hero_;
}

vector<shared_ptr<BattleObject>> HeroArmy::getBattleObjectsOfArmy() const{

    vector<shared_ptr<BattleObject>> battle_objects = Army::getBattleObjectsOfArmy();

    if(hero_ != nullptr){
        battle_objects.push_back(hero_);
    }

    return battle_objects;
}

shared_ptr<Hero> HeroArmy::getArmyHeroPtr() const{
    return hero_;
}
