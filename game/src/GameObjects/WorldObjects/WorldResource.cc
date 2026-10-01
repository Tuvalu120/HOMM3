/**
 * @file WorldResource.cc
 * @author Bartosz Han
 * @brief This file contains definition of class WorldResource.
 *
 */


#include "GameObjects/WorldObjects/WorldResource.h"
#include "Miscellaneous/GameUtilities.hpp"
#include "Managers/WorldManager.h"

using namespace std;
using namespace game_data;
using namespace constants;

WorldResource::WorldResource(string initial_texture_name, Resource resource_type, int quantity):
    WorldGeneratedInteractiveObject(initial_texture_name, RESOURCE_OBJECT_TYPE), quantity_(quantity), resource_type_(resource_type){};

WorldResource::WorldResource():WorldGeneratedInteractiveObject(RESOURCE_OBJECT_TYPE),
    resource_type_(chooseFromContainerWithDistribution(RESOURCE_TYPES, WORLD_RESOURCE_PROBABILITIES)){

    setCurrentTextureName(WORLD_RESOURCE_IMAGE.at(getResourceType()));
    pair<int, int> possible_quantities = WORLD_RESOURCE_QUANTITY_PROBABILITIES.at(getResourceType());
    setQuantity(generateRandomIntInRange(possible_quantities.first, possible_quantities.second));

}

Resource WorldResource::getResourceType() const{
    return resource_type_;
}

unsigned int WorldResource::getQuantity() const{
    return quantity_;
}

void WorldResource::interact(){
    WorldManager::getInstance().getCurrentPlayer().addResources(resource_type_, quantity_);
}

void WorldResource::setQuantity(const int value){
    quantity_ = value;
}

bool WorldResource::operator==(const WorldResource& other) const{
    return (getXPos() == other.getXPos() && getYPos() == other.getYPos() &&
        getResourceType() == other.getResourceType() && getQuantity() == other.getQuantity());
}