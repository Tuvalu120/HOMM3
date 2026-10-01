/**
 * @file Building.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Building.
 *
 */


#include "GameObjects/Buildings/Building.h"
#include "GameObjects/Town.h"
#include "Managers/PlayerManager.h"

using namespace std;

Building::Building(Town& town_of_building, int max_level, vector<string> building_sprites):
    DynamicDrawableObject(building_sprites),town_of_building_(town_of_building),building_level_(0),max_building_level_(max_level),
    building_sprites_(building_sprites){
        if (building_sprites.size() != static_cast<size_t>(max_building_level_ + 1)){
            throw invalid_argument("Number of sprites of the building does not match the number of possible levels of building");
        }
    };

Player& Building::getOwnerOfBuilding(){
    int owner_id = town_of_building_.getPlayerID();
    return PlayerManager::getInstance().getPlayerByID(owner_id);
}

bool Building::attemptToLevelUp(){
    if(building_level_ < max_building_level_ && canLevelUp()){
        getOwnerOfBuilding().spendResources(getUpgradeCost());
        ++building_level_;
        onLevelUp();
        return true;
    }
    return false;
};
int Building::getBuildingLevel() const{
    return building_level_;
}

int Building::getMaxBuildingLevel() const{
    return max_building_level_;
}

const string Building::getCurrentTextureName() const{
    return getTextureAt(getBuildingLevel());
}

const string Building::generateBuildingDescription(const string building_name) const{
    return building_name + " (Poziom " + to_string(building_level_) + ")";
}

bool Building::canLevelUp(){
    return getBuildingLevel() != getMaxBuildingLevel() ? getOwnerOfBuilding().canAfford(getUpgradeCost()) : false;
}
