/**
 * @file Tavern.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Tavern.
 *
 */


#include "GameObjects/Buildings/Tavern.h"
#include "Data/GameData.hpp"
#include "GameObjects/Town.h"


using namespace std;
using namespace game_data;
using namespace constants;

Tavern::Tavern(Town& town_of_building, vector<string> building_sprites):
    Building(town_of_building, MAX_LEVEL_OF_TAVERN, building_sprites){
        town_of_building.setStatistic(HERO_BUYING_ENABLED, 0);
    };

void Tavern::onLevelUp(){
    town_of_building_.setStatistic(HERO_BUYING_ENABLED, 1);
    town_of_building_.resetHeroesInTavern();
}

const string Tavern::getBuildingDescription() const{
    return building_level_ == 0 ? game_data::TAVERN_NAME_LEVEL_0 : game_data::TAVERN_NAME;
}

const ResourceArray& Tavern::getUpgradeCost() const{

    if(getBuildingLevel() == getMaxBuildingLevel()){
        return EMPTY_RESOURCE_ARRAY;
    }
    return COST_OF_TAVERN_UPGRADE.at(building_level_);
}
