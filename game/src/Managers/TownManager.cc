/**
 * @file TownManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class TownManager.
 *
 */


#include "Managers/TownManager.h"
#include "Miscellaneous/GameExceptions.hpp"

using namespace std;
using namespace game_data;
using namespace constants;

TownManager::TownManager(): SubManager(), current_town_id_(-1){}

Town& TownManager::getTownWithID(const int id) const{
    return *towns_.at(id);
}

void TownManager::generateTowns(const vector<pair<int, int>> possible_town_coords, const int n_towns){
    if (n_towns > static_cast<int>(possible_town_coords.size())){
        throw MapStructureError("Tried to generate more towns than allowed by the map");
    }
    vector<pair<int, int>> chosen_coords = chooseNFromContainer(possible_town_coords, n_towns);
    for (int i=0; i<n_towns; ++i){
        TownType town_type = static_cast<TownType>(i % N_TOWN_TYPES);
        TownData town_data = TOWN_DATA.at(town_type);
        towns_.at(i) = make_unique<Town>(town_data, town_type, i, chosen_coords.at(i).first, chosen_coords.at(i).second);
    }
}

int TownManager::getCurrentTownID() const{
    return current_town_id_;
}

void TownManager::resetCurrentTownID(){
    current_town_id_ = -1;
}

void TownManager::setCurrentTownID(const int new_town_id){
    current_town_id_ = new_town_id;
}

Town& TownManager::getCurrentTown() const{
    if (current_town_id_ != -1){
        return *towns_.at(current_town_id_);
    }
    else{
        throw TownLogicError("Tried to access current Town data when no Town is being visited right now");
    }
}

void TownManager::onNewDay(){
    for(const auto& town : towns_){
        town->onNewDay();
    }
}

void TownManager::onNewWeek(){
    for(const auto& town : towns_){
        town->onNewWeek();
    }
}

void TownManager::resetFlagsWasTownVisitedThisTurn(){
    for(const auto& town : towns_){
        town->resetFlagWasTownVisitedThisTurn();
    }
}

int TownManager::getNOfTownsOfPlayer(const int player_id) const{
    int counter = 0;
    for(const auto& town : towns_){
        if(town->getPlayerID() == player_id){
            ++counter;
        }
    }
    return counter;
}

void TownManager::removeTownOwnershipOfPlayer(const int player_id){
    for(const auto& town: towns_){
        if(town->getPlayerID() == player_id){
            town->setPlayerID(-1);
        }
    }
}

void TownManager::resetManager(){
    current_town_id_ = -1;
    for(auto& town: towns_){
        town.reset();
    }
}