/**
 * @file InteractiveObject.cc
 * @author Bartosz Han
 * @brief This file contains definition of class InteractiveObject.
 *
 */


#include "GameObjects/BaseObjects/InteractiveObject.h"
#include "Data/ObjectDataEnums.hpp"

using namespace std;

InteractiveObject::InteractiveObject(string initial_texture_name, const ObjectType object_type,
    int x_pos, int y_pos, int priority, bool destroy_after_interaction):
    DrawableObject(initial_texture_name),
    interactive_object_type_(object_type),
    priority_(priority),
    x_pos_(x_pos),
    y_pos_(y_pos),
    destroy_after_interaction_(destroy_after_interaction){};

InteractiveObject::InteractiveObject():
    InteractiveObject("", UNKNOWN_OBJECT, -1, -1){};

int InteractiveObject::getXPos() const{
    return x_pos_;
}
int InteractiveObject::getYPos() const{
    return y_pos_;
}
bool InteractiveObject::getFlagDestroyAfterInteraction() const {
    return destroy_after_interaction_;
}
void InteractiveObject::setFlagDestroyAfterInteraction(){
    destroy_after_interaction_ = true;
}

int InteractiveObject::getPriority() const{
    return priority_;
}

int InteractiveObject::getPriority(WorldArmy&) const{
    return priority_;
}

ObjectType InteractiveObject::getInteractiveObjectType() const{
    return interactive_object_type_;
}

void InteractiveObject::drawAdditionalData(IPainter&) const{}
