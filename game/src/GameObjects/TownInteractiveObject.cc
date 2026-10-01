/**
 * @file TownInteractiveObject.cc
 * @author Bartosz Han
 * @brief This file contains definition of class TownInteractiveObject.
 *
 */

#include "GameObjects/TownInteractiveObject.h"
#include "Data/Constants.hpp"

using namespace std;
using namespace constants;

TownInteractiveObject::TownInteractiveObject(Town& town_of_object, vector<string> texture_names,
    const int x_pos, const int y_pos, const int object_pos):
    InteractiveObject(texture_names.at(0), PART_OF_TOWN, x_pos, y_pos, TOWN_PRIORITY),
    town_of_object_(town_of_object), object_inner_pos_(object_pos),
    town_texture_names_(texture_names){};

void TownInteractiveObject::interact(){
    town_of_object_.interactWithTown();
}

void TownInteractiveObject::drawAdditionalData(IPainter& painter) const{
    painter.replacePlayerColorOnPixmap(playerIDToQColor(town_of_object_.getPlayerID()));
    painter.drawSquareOnPixmap(playerIDToQColor(town_of_object_.getPlayerID()), object_inner_pos_);
}

const string TownInteractiveObject::getCurrentTextureName() const{
    int town_level = town_of_object_.getBuilding(HALL).getBuildingLevel();
    return town_texture_names_.at(town_level);
}

int TownInteractiveObject::getPlayerID() const{
    return town_of_object_.getPlayerID();
}

const Town& TownInteractiveObject::getTownOfObject() const{
    return town_of_object_;
}
