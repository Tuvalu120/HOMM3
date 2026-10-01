/**
 * @file Fortifications.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Fortifications.
 *
 */


#include "GameObjects/Buildings/Fortifications.h"
#include "Data/GameData.hpp"
#include "GameObjects/Town.h"

using namespace std;
using namespace game_data;
using namespace constants;

Fortifications::Fortifications(Town& town_of_building, vector<string> building_sprites):
    Building(town_of_building, MAX_LEVEL_OF_FORTIFICATIONS, building_sprites){
        town_of_building.setStatistic(TOWN_ARMY_LEVEL, TOWN_ARMY_LEVELS.at(building_level_));
        town_of_building.setStatistic(TROOP_GROWTH_MULTIPLIER, CREATURE_GROWTH_MULTIPLIERS.at(building_level_));
    };

void Fortifications::onLevelUp(){
    town_of_building_.setStatistic(TOWN_ARMY_LEVEL, TOWN_ARMY_LEVELS[building_level_]);
    town_of_building_.resetArmy();
    town_of_building_.setStatistic(TROOP_GROWTH_MULTIPLIER, CREATURE_GROWTH_MULTIPLIERS[building_level_]);
}

const string Fortifications::getBuildingDescription() const{
    return building_level_ == 0 ? FORTIFICATIONS_NAME_LEVEL_0  : generateBuildingDescription(game_data::FORTIFICATIONS_NAME);
}

const ResourceArray& Fortifications::getUpgradeCost() const{

    if(getBuildingLevel() == getMaxBuildingLevel()){
        return EMPTY_RESOURCE_ARRAY;
    }
    return COST_OF_FORTIFICATIONS_UPGRADE.at(building_level_);
}
