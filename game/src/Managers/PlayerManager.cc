/**
 * @file PlayerManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class PlayerManager.
 *
 */


#include "Managers/PlayerManager.h"
#include "Managers/WorldManager.h"
#include "Managers/TownManager.h"
#include "Miscellaneous/GameExceptions.hpp"
#include "PlayerControllers/PlayerController.h"
#include <memory>

using namespace std;
using namespace constants;

PlayerManager::PlayerManager(): SubManager(){}

int PlayerManager::getNOfPlayers() const{
    return players_.size();
}

int PlayerManager::getNOfActivePlayers() const{
    int n_of_players = 0;
    for (int i=0; i<getNOfPlayers(); ++i){
        if (isPlayerActive(i)){
            ++n_of_players;
        }
    }
    return n_of_players;
}


bool PlayerManager::addArmyToPlayer(int player_id, HeroType hero_id, int init_x_pos, int init_y_pos){

    shared_ptr<WorldArmy> new_army = getPlayerByID(player_id).addArmyToPlayer(hero_id, init_x_pos, init_y_pos);

    if (new_army == nullptr){
        return false;
    }

    int radius = new_army->getDiscoveryRadius();
    auto init_visited_tiles = WorldManager::getInstance().getWorldMap().getTilesWithinRadius(init_x_pos, init_y_pos, radius);
    for (auto visited_tile: init_visited_tiles){
        getPlayerByID(player_id).markTileAsVisited(visited_tile);
    }
    WorldManager::getInstance().getWorldMap().getTileAt(init_x_pos, init_y_pos).addObjectOnTile(new_army);
    return true;
}

bool PlayerManager::isPlayerActive(const int player_id) const{
    return players_.at(player_id) != nullptr;
}

Player& PlayerManager::getPlayerByID(const int player_id) const{
    if(isPlayerActive(player_id)){
        return *players_.at(player_id);
    }
    throw RemovedObjectError("Player");
}

void PlayerManager::addNewPlayer(const int player_id){
    int n_current_players = players_.size();
    if(n_current_players < player_id){
        throw invalid_argument("Invalid new player ID: there exists player with such ID." );
    }
    if(n_current_players > player_id){
        throw invalid_argument("Invalid new player ID: ID value is too big.");
    }
    players_.push_back(make_unique<Player>(player_id));
}

void PlayerManager::removePlayerFromGame(const int player_id){

    while(getPlayerByID(player_id).doesArmyExist(0)){
        getPlayerByID(player_id).removeArmy(0);
    }

    players_.at(player_id) = nullptr;
    player_results_.insert(player_results_.begin(), player_id);
    WorldManager::getInstance().removeMineOwnershipOfPlayer(player_id);
    TownManager::getInstance().removeTownOwnershipOfPlayer(player_id);

}

const vector<int>& PlayerManager::getGameResults() const{
    return player_results_;
}

const vector<int> PlayerManager::getRemainingPlayers() const{
    vector<int> remaining_players;
    for (size_t i=0; i<players_.size(); ++i){
        if (isPlayerActive(i)){
            remaining_players.push_back(i);
        }
    }
    return remaining_players;
}

void PlayerManager::checkForEliminatedPlayer(int player_id, bool check_during_player_turn){

    if (!getPlayerByID(player_id).isPlayerActive()){
        removePlayerFromGame(player_id);
        if (check_during_player_turn){
        WorldManager::getInstance().goToNextPlayer();
        }
    }
}

void PlayerManager::initializePlayers(int n_players){
    vector<int> initial_towns = chooseNNumbersFromRange(0, NUMBER_OF_TOWNS-1, n_players);
    unique_ptr<Player> player;
    for (int i=0; i<n_players; ++i){
        addNewPlayer(i);
        HeroType first_hero_type = static_cast<HeroType>(generateRandomIntInRange(0, N_HERO_TYPES-1));
        TownManager::getInstance().getTownWithID(initial_towns.at(i)).setPlayerID(i);
        pair<int, int> spawn_pos = TownManager::getInstance().getTownWithID(initial_towns.at(i)).getArmyStartingPos();
        addArmyToPlayer(i, first_hero_type, spawn_pos.first, spawn_pos.second);
    }

}

BattleController& PlayerManager::getNeutralPlayerController(){
    return *neutral_player_controller_;
}

std::shared_ptr<BattleController> PlayerManager::getNeutralPlayerControllerPtr() const{
    return neutral_player_controller_;
}


void PlayerManager::validatePlayerID(const int player_id, const std::string object_name, const bool can_be_neutral) const{
    int min_valid_id = can_be_neutral ? -1 : 0;
    if(player_id < min_valid_id || player_id >= constants::MAX_NUMBER_OF_PLAYERS){
        throw InvalidPlayerIDError(object_name, player_id);
    }
}

void PlayerManager::setNeutralPlayerController(std::shared_ptr<BattleController> controller){
    neutral_player_controller_ = controller;
}

void PlayerManager::resetManager(){
    players_.clear();
    player_results_.clear();
    neutral_player_controller_.reset();
}