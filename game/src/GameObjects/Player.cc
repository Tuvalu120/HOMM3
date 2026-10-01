/**
 * @file Player.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Player.
 *
 */


#include <algorithm>
#include <iostream>

#include "GameObjects/Player.h"
#include "PlayerControllers/HumanPlayerController.h"
#include "Managers/TownManager.h"
#include "Managers/WorldManager.h"

using namespace std;
using namespace constants;

Player::Player(int player_id, string model_file_name): OwnedByPlayer(player_id),
    current_army_id_(-1),
    player_resources_(){
    if (model_file_name == ""){
        player_controller_ = make_unique<HumanPlayerController>();
    }
    else{
        // Full AIPlayerController would be crated here. However, it has not been implemented.
    }
}

const array<shared_ptr<WorldArmy>, MAX_NUMBER_OF_ARMIES>& Player::getArmies() const{
    return armies_;
}

std::shared_ptr<WorldArmy> Player::attemptToAddArmy(std::shared_ptr<WorldArmy> army_to_add){
    int army_id = -1;
    for (int i = 0 ; i < MAX_NUMBER_OF_ARMIES; ++i){
        if (!doesArmyExist(i)){
            armies_.at(i) = army_to_add;
            army_id = i;
            break;
        }
        else if (getArmyByID(i).getArmyHero().getHeroType() == army_to_add->getArmyHero().getHeroType()){
            cout << "Failed to add new army, because Player has already army with this hero" << endl;
            return nullptr;
        }
    }
    if (army_id == -1){
        cout << "Failed to add new army, because Player has already maximum number of armies" << endl;
        return nullptr;
    }

    if (current_army_id_ == -1){
        setCurrentArmyID(0);
    }
    return army_to_add;
}


shared_ptr<WorldArmy> Player::addArmyToPlayer(HeroType hero_id, int init_x_pos, int init_y_pos){

    auto new_army = WorldArmy::generateArmy(getPlayerID(), hero_id, init_x_pos, init_y_pos);
    return attemptToAddArmy(new_army);
}

int Player::getCurrentArmyID() const{
    return current_army_id_;
}

int Player::getNMaxArmies() const{
    return armies_.size();
}

int Player::getNArmies() const{
    int n_armies = 0;
    for (int i = 0 ; i < MAX_NUMBER_OF_ARMIES; ++i){
        if (armies_.at(i) != nullptr){
            ++n_armies;
        }
    }
    return n_armies;
}

void Player::setCurrentArmyID(int army_id){
    if (army_id < getNMaxArmies() && army_id >= 0){
        current_army_id_ = army_id;
    }
    else{
        throw invalid_argument("Tried to set invalid army ID");
    }
}

WorldArmy& Player::getCurrentArmy() const{
    if(current_army_id_ == -1){
        throw invalid_argument("Tried to access current army, but Player has not initialized any army");
    }
    return *armies_.at(current_army_id_);
}

//Returns  1 if successfully changed army.
//Returns  0 if previous army was the last in Player's turn.
//Returns -1 if Player does not have any Army and indicates that this Player should be eliminated.
int Player::goToNextArmy(){
    if (!isPlayerActive()){
        return -1;
    }
    for(int i = current_army_id_+1; i < MAX_NUMBER_OF_ARMIES; ++i){
        if(doesArmyExist(i)){
            current_army_id_ = i;
            getCurrentArmy().resetMoves();
            return 1;
        }
    }
    return 0;
}

const ResourceArray& Player::getPlayerResources() const{
    return player_resources_;
}

bool Player::canAfford(const ResourceArray& resources) const{
    for (int i = 0; i < N_RESOURCES ; ++i){
        if (player_resources_[i] < resources[i]){
            return false;
        }
    }
    return true;
}

bool Player::spendResources(const ResourceArray& resources){
    if(canAfford(resources)){

        for (int i = 0; i < N_RESOURCES ; ++i){
            player_resources_[i] -= resources[i];
        }
        return true;
    }
    return false;
}

void Player::addResources(const ResourceArray& resources){
    for (int i = 0; i < N_RESOURCES ; ++i){
            player_resources_[i] += resources[i];
    }
}

bool Player::spendResources(Resource resource_type, const unsigned int resource_quantity){
    if (player_resources_.at(resource_type) >= resource_quantity){
        player_resources_.at(resource_type) -= resource_quantity;
        return true;
    }
    return false;
}

void Player::addResources(Resource resource_type, const unsigned int resource_quantity){
        player_resources_.at(resource_type) += resource_quantity;
}

const vector<pair<int,int>>& Player::getExploredWorldTiles() const{
    return explored_world_tiles_;
}

bool Player::hasTileBeenVisited(pair<int,int> tile_pos) const{
    return (find(explored_world_tiles_.begin(), explored_world_tiles_.end(), tile_pos) != explored_world_tiles_.end());
}

void Player::markTileAsVisited(pair<int,int> tile_pos){
    if (!hasTileBeenVisited(tile_pos)){
        explored_world_tiles_.push_back(tile_pos);
    }
}

PlayerController& Player::getPlayerController() const{
    if(player_controller_ != nullptr){
        return *player_controller_;
    }
    throw runtime_error("Cannot get the player controller object of player with ID: " + to_string(getPlayerID()) + ", as it is not an AI player");
}

std::shared_ptr<PlayerController> Player::getPlayerControllerPtr() const{
    return player_controller_;
}


int Player::getArmyID(const Army& army_to_check) const{
    return getArmyID(&army_to_check);
}

int Player::getArmyID(const Army* army_to_check) const{
    for(size_t i = 0; i < armies_.size(); ++i){
        if(army_to_check == armies_[i].get()){
            return i;
        }
    }
    return -1;
}

WorldArmy& Player::getArmyByID(int army_id){
    if (armies_.at(army_id) != nullptr){
        return *armies_.at(army_id);
    }
    throw invalid_argument("Tried to access army with invalid ID");
}

bool Player::doesArmyExist(const int army_to_check_index) const{
    return (armies_.at(army_to_check_index) != nullptr);
}

bool Player::canAddNewArmy(){
    for (int i = 0 ; i < MAX_NUMBER_OF_ARMIES; ++i){
        if (!doesArmyExist(i)){
            return true;
        }
    }
    return false;
}

bool Player::isPlayerActive(){
    for (int i = 0 ; i < MAX_NUMBER_OF_ARMIES; ++i){
    if (doesArmyExist(i)){
        return TownManager::getInstance().getNOfTownsOfPlayer(getPlayerID()) > 0;
    }
    }
    return false;
}


//This function is used, when one of the armies is eliminated.
void Player::moveArmiesInArray(){
    int insert_pos = 0;
    WorldArmy* current_army = &getCurrentArmy();
    for(int i=0 ; i<MAX_NUMBER_OF_ARMIES; ++i){
        if(doesArmyExist(i)){
            armies_.at(insert_pos) = std::move(armies_.at(i));
            if(armies_.at(insert_pos).get() == current_army){
                setCurrentArmyID(insert_pos);
            }
            ++insert_pos;
        }
    }
}

void Player::removeArmy(const int army_id){
    int pos_x = getArmyByID(army_id).getXPos();
    int pos_y = getArmyByID(army_id).getYPos();
    WorldManager::getInstance().getWorldMap().getTileAt(pos_x, pos_y).removeObjectOnTile();
    armies_.at(army_id) = nullptr;
    moveArmiesInArray();

    //If attacking army was eliminated, we must prepare new army to take its turns.
    if(doesArmyExist(army_id)){
        getArmyByID(army_id).resetMoves();
    }
}
