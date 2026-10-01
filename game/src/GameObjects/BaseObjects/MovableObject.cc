/**
 * @file MovableObject.cc
 * @author Bartosz Han
 * @brief This file contains definition of class MovableObject.
 *
 */


#include <stdexcept>

#include "GameObjects/BaseObjects/MovableObject.h"

using namespace std;

MovableObject::MovableObject(const string initial_texture_name, const ObjectType object_type,
    const int x_pos, const int y_pos, const int max_moves, const int priority, const bool destroy_after_interaction):
    InteractiveObject(initial_texture_name, object_type, x_pos, y_pos, priority, destroy_after_interaction),max_moves_(max_moves),moves_left_(max_moves){}

MovableObject::MovableObject(string initial_texture_name, const ObjectType object_type, int max_moves):
    InteractiveObject(initial_texture_name, object_type, -1, -1),max_moves_(max_moves){}

void MovableObject::setXPos(int new_x_pos){
    x_pos_ = new_x_pos;
}

void MovableObject::setYPos(int new_y_pos){
    y_pos_ = new_y_pos;
}

int MovableObject::getMaxMoves() const{
    return max_moves_;
}

void MovableObject::setMaxMoves(const int new_max_moves){
    if (new_max_moves > 0){
        max_moves_ = new_max_moves;
        moves_left_ = new_max_moves;
    }
    else{
        throw invalid_argument("Tried to set negative number of moves");
    }
}

int MovableObject::getMovesLeft() const{
    return moves_left_;
}

bool MovableObject::canMove(int moves_to_go) const{
    return (moves_left_ - moves_to_go >= 0);
}

bool MovableObject::canMove() const{
    return (moves_left_ > 0);
}

void MovableObject::addMoves(int number_to_add){
    if (moves_left_ + number_to_add <= max_moves_){
        moves_left_ += number_to_add;
    }
    else{
        throw invalid_argument("Tried to add bigger number of moves than it is allowed");
    }
}
void MovableObject::removeMoves(int number_to_remove){
    if (moves_left_ - number_to_remove >= 0){
        moves_left_ -= number_to_remove;
    }
    else{
        throw invalid_argument("Tried to remove bigger number of moves than it is allowed");
    }
}
void MovableObject::resetMoves(){
    moves_left_ = max_moves_;
}
