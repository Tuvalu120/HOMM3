/**
 * @file Mine.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Mine.
 *
 */

#include "Managers/WorldManager.h"
#include "Managers/BattleManager.h"
#include "GameObjects/WorldObjects/Mine.h"
#include "Data/GameData.hpp"
#include "Managers/PlayerManager.h"

using namespace std;
using namespace game_data;
using namespace constants;

Mine::Mine():WorldGeneratedInteractiveObject(MINES_OBJECT_TYPE,
    priority_=OTHER_OBJECT_PRIORITY, destroy_after_interaction_=false),OwnedByPlayer(-1){
        auto resource_types = RESOURCE_TYPES;
        resource_ = chooseFromContainerWithDistribution(resource_types, MINE_RESOURCE_PROBABILITIES);
        quantity_ = MINE_RESOURCE_QUANTITY.at(resource_);
        setCurrentTextureName(MINE_IMAGE.at(resource_));
        resetArmy();
    };

void Mine::interact(){
    if(WorldManager::getInstance().getCurrentPlayer().getPlayerID() != getPlayerID()){
        int result = BattleManager::getInstance().battle(WorldManager::getInstance().getCurrentPlayer().getCurrentArmy(), *mine_army_);
        if (result == 1){
            setPlayerID(WorldManager::getInstance().getCurrentPlayer().getPlayerID());
        }
    }
}

void Mine::setPlayerID(const int new_player_id){
    OwnedByPlayer::replacePlayerID(new_player_id);

    resetArmy();
}

void Mine::resetArmy(){
    vector<vector<TroopType>> possible_troop_types_groups = TROOP_TYPES;
    mine_army_ = Army::generatePatternArmy(MINE_ARMY_LEVEL, chooseFromContainer(possible_troop_types_groups));
}

Resource Mine::getResourceType() const{
    return resource_;
}

int Mine::getResourceQuantity() const{
    return quantity_;
}

void Mine::addResourceToPlayer(){
    if(getPlayerID() != -1){
        PlayerManager::getInstance().getPlayerByID(getPlayerID()).addResources(resource_, quantity_);
    }
}

Army& Mine::getMineArmy() const{
    return *mine_army_;
}

void Mine::drawAdditionalData(IPainter& painter) const{
    painter.replacePlayerColorOnPixmap(playerIDToQColor(getPlayerID()));
    painter.drawSquareOnPixmap(playerIDToQColor(getPlayerID()));
}

bool Mine::operator==(const Mine& other) const{
    return (this->getXPos() == other.getXPos() && this->getYPos() == other.getYPos() &&
            this->getResourceType() == other.getResourceType() && this->getResourceQuantity() == other.getResourceQuantity());
}