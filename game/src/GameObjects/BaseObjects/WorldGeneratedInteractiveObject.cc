/**
 * @file WorldGeneratedInteractiveObject.cc
 * @author Bartosz Han
 * @brief This file contains definition of class WorldGeneratedInteractiveObject.
 *
 */


#include "GameObjects/BaseObjects/WorldGeneratedInteractiveObject.h"

using namespace std;

WorldGeneratedInteractiveObject::WorldGeneratedInteractiveObject(const string initial_texture_name, const ObjectType object_type,
        const int priority, const bool destroy_after_interaction):
        InteractiveObject(initial_texture_name, object_type, -1, -1, priority, destroy_after_interaction){};

WorldGeneratedInteractiveObject::WorldGeneratedInteractiveObject(const ObjectType object_type,
        const int priority, const bool destroy_after_interaction):
        WorldGeneratedInteractiveObject("", object_type, priority, destroy_after_interaction){};

void WorldGeneratedInteractiveObject::setPos(const pair<int, int> pos){
        x_pos_ = pos.first;
        y_pos_ = pos.second;
}
