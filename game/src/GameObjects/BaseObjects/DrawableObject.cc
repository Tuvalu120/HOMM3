/**
 * @file DrawableObject.cc
 * @author Bartosz Han
 * @brief This file contains definition of class DrawableObject.
 *
 */


#include "GameObjects/BaseObjects/DrawableObject.h"

using namespace std;

DrawableObject::DrawableObject(string initial_texture_name):
    current_texture_name_(initial_texture_name){}

void DrawableObject::setCurrentTextureName(string new_texture_name){
    current_texture_name_ = new_texture_name;
}

const string DrawableObject::getCurrentTextureName() const{
    return current_texture_name_;
}
