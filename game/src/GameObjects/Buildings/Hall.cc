/**
 * @file Hall.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Hall.
 *
 */


#include "GameObjects/Buildings/Hall.h"
#include "Data/GameData.hpp"
#include "GameObjects/Player.h"
#include "GameObjects/Town.h"


using namespace std;
using namespace constants;

Hall::Hall(Town& town_of_building, vector<string> building_sprites):
    Building(town_of_building, MAX_LEVEL_OF_HALL, building_sprites){
        town_of_building.setStatistic(GOLD_GENERATED, game_data::GOLD_PROVIDED_BY_HALL.at(building_level_));
}

bool Hall::canLevelUp(){
    Player& owner = getOwnerOfBuilding();
    return (owner.canAfford(game_data::COST_OF_HALL_UPGRADE.at(building_level_)) && checkBuildingRequirements());
}

void Hall::onLevelUp(){
    town_of_building_.setStatistic(GOLD_GENERATED, game_data::GOLD_PROVIDED_BY_HALL.at(building_level_));
}

bool Hall::checkBuildingRequirements(){
    vector<pair<BuildingType, int>> requrirements = game_data::HALL_REQUIREMENTS[building_level_];
    for (const pair<BuildingType, int>& req : requrirements){
        if (town_of_building_.getBuilding(req.first).getBuildingLevel() < req.second){
        return false;
        }
    };
    return true;
}

const string Hall::getBuildingDescription() const{
    return generateBuildingDescription(game_data::HALL_NAME);
}

const ResourceArray& Hall::getUpgradeCost() const{

    if(getBuildingLevel() == getMaxBuildingLevel()){
        return EMPTY_RESOURCE_ARRAY;
    }
    return game_data::COST_OF_HALL_UPGRADE.at(building_level_);
}
