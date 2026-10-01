/**
 * @file Dwellings.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Dwellings.
 *
 */


#include "GameObjects/Buildings/Dwellings.h"
#include "Data/GameData.hpp"
#include "GameObjects/Town.h"

using namespace std;
using namespace constants;

Dwellings::Dwellings(Town& town_of_building, vector<string> building_sprites):
    Building(town_of_building, MAX_LEVEL_OF_DWELLINGS, building_sprites){};

void Dwellings::onLevelUp(){
    town_of_building_.increaseSingleTroopAvailableToBuy(building_level_-1);
}

const string Dwellings::getBuildingDescription() const{
    return building_level_ == 0 ? game_data::DWELLINGS_NAME_LEVEL_0 : generateBuildingDescription(game_data::DWELLINGS_NAME);
}

const ResourceArray& Dwellings::getUpgradeCost() const{

    if(getBuildingLevel() == getMaxBuildingLevel()){
        return EMPTY_RESOURCE_ARRAY;
    }
    return town_of_building_.getTownData().cost_of_upgrading_dwelling.at(building_level_);
}
