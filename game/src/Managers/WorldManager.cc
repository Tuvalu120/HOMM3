/**
 * @file WorldManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class WorldManager.
 *
 */

#include "Managers/WorldManager.h"
#include "Managers/GraphicsManager.h"
#include "Managers/TownManager.h"
#include "Data/Constants.hpp"
#include "GameObjects/WorldObjects/WorldResource.h"
#include "Miscellaneous/GameExceptions.hpp"
#include "Miscellaneous/ObjectGenerator.hpp"

using namespace std;
using namespace constants;
using namespace game_data;

WorldManager::WorldManager(): SubManager(), current_day_(1),
    current_player_id_(0),
    flag_was_army_eliminated_during_its_turn_(false){}

void WorldManager::createWorldMap(){

    world_map_ = make_unique<WorldMap>();
    world_map_->initializeMap();

}

WorldMap& WorldManager::getWorldMap(){
    return *world_map_;
}

void WorldManager::moveCurrentArmyToTile(int new_x_pos, int new_y_pos){
    if (!getWorldMap().getTileAt(new_x_pos, new_y_pos).isTileWalkable()){
        cout << "Cannot move objects to tile " << new_x_pos << " " << new_y_pos << endl;
        return;
    }
    WorldArmy& current_army = getCurrentPlayer().getCurrentArmy();
    int army_x_pos = current_army.getXPos();
    int army_y_pos = current_army.getYPos();
    auto move_data = getWorldMap().findPathToTile(army_x_pos, army_y_pos, new_x_pos, new_y_pos);
    vector<WorldMove> moves = move_data.first;

    for(WorldMove move: moves){
        getWorldMap().moveObject(current_army, move);
        for (auto &discovered_tile: getWorldMap().getTilesWithinRadius(current_army.getXPos(), current_army.getYPos(), current_army.getDiscoveryRadius())){
            getCurrentPlayer().markTileAsVisited(discovered_tile);
        }
        current_army.removeMoves(getWorldMap().getTileAt(current_army.getXPos(), current_army.getYPos()).getMoveCost());
        attemptToInteract();
        if(!current_army.canMove()){
            break;
        }
    }

    if(!GameManager::getInstance().isGameFinished() && !getCurrentPlayer().getCurrentArmy().canMove()){
        endTurnOfArmy();
    }
}

void WorldManager::moveCurrentArmyToTile(WorldMove move){

        WorldArmy& current_army = getCurrentPlayer().getCurrentArmy();
        int army_x_pos = current_army.getXPos();
        int army_y_pos = current_army.getYPos();
        pair<int,int> add_move = WorldMap::worldMoveToInt(move);
        moveCurrentArmyToTile(army_x_pos+add_move.first, army_y_pos+add_move.second);
    }

Player& WorldManager::getCurrentPlayer() const{
    return PlayerManager::getInstance().getPlayerByID(current_player_id_);
}

int WorldManager::getCurrentPlayerID() const{
    return current_player_id_;
}

void WorldManager::setCurrentPlayer(int new_current_player){
    current_player_id_ = new_current_player;
}

void WorldManager::endTurnOfArmy(){
    if (PlayerManager::getInstance().isPlayerActive(getCurrentPlayerID())){
        attemptToInteract(true);
        int check_for_next_army = getCurrentPlayer().goToNextArmy();
        if (check_for_next_army == 0){
            goToNextPlayer();
        }
        TownManager::getInstance().resetFlagsWasTownVisitedThisTurn();
        resetNeutralTroopsVisitFlag();
        GraphicsManager::getInstance().resetWorldMapOffset();
    }
    else{
        throw WorldLogicError("Player with ID: " + to_string(getCurrentPlayerID()) + " is not active and no actions can be performed.");
    }

}

void WorldManager::goToNextPlayer(){
    for (int i = current_player_id_ + 1; i < PlayerManager::getInstance().getNOfPlayers(); ++i){


        if(PlayerManager::getInstance().isPlayerActive(i)){
            current_player_id_ = i;
            getCurrentPlayer().setCurrentArmyID(0);
            getCurrentPlayer().getCurrentArmy().resetMoves();
            return;
        }
    }
    if (!GameManager::getInstance().isGameFinished()){
        endDay();
    }
}


void WorldManager::endDay(){
    ++current_day_;
    for(int i=0; i<MAX_NUMBER_OF_PLAYERS; ++i){
        if(PlayerManager::getInstance().isPlayerActive(i)){
            setCurrentPlayer(i);
            break;
        }
    }
    getCurrentPlayer().setCurrentArmyID(0);
    getCurrentPlayer().getCurrentArmy().resetMoves();
    onNewDay();

    if(current_day_ % NUMBER_OF_DAYS_IN_WEEK == 0){
        onNewWeek();
    }
}

void WorldManager::onNewDay(){
    TownManager::getInstance().onNewDay();

    MinesGenerator& generator = static_cast<MinesGenerator&>(*world_static_objects_generators_.at(MINE_GENERATOR_VECTOR_POS));
    generator.addResourcesToPlayers();

}

void WorldManager::onNewWeek(){
    TownManager::getInstance().onNewWeek();
    generateWorldObjects();
}

int WorldManager::getCurrentDay() const{
    return current_day_;
}

int WorldManager::getCurrentWeek() const{
    return ((current_day_-1) / NUMBER_OF_DAYS_IN_WEEK) + 1;
}

int WorldManager::getGameProgressValue() const{
    int init_value = ((current_day_-1) / GAME_PROGRESS_GROWTH_DAYS) + 1;
    return min(init_value, MAX_GAME_PROGRESS_VALUE);
}

void WorldManager::removeInteractiveObject(int x_pos, int y_pos){
    getWorldMap().getTileAt(x_pos, y_pos).removeObjectOnTile();
}

void WorldManager::attemptToInteract(const bool only_low_priority){
    InteractiveObject* object_to_interact = nullptr;
    vector<InteractiveObject*> other_interactive_objects;
    int max_priority = -1;
    WorldArmy& current_army = getCurrentPlayer().getCurrentArmy();
    int x_pos = current_army.getXPos();
    int y_pos = current_army.getYPos();
    auto tiles_to_check = getWorldMap().getNeighboursOfTile(x_pos, y_pos).first;

    for (auto& tile : tiles_to_check){
        if(tile.get().hasInteractiveObject()){
            if (tile.get().getObjectOnTile().getPriority() > max_priority){
                max_priority = tile.get().getObjectOnTile().getPriority(current_army);
                object_to_interact = &tile.get().getObjectOnTile();
            }
            if (tile.get().getObjectOnTile().getPriority() == OTHER_OBJECT_PRIORITY){
                other_interactive_objects.push_back(&tile.get().getObjectOnTile());
            }
        }
    }

    //Player will interact with all 'other' objects and only one object with higher priority than that
    for(InteractiveObject* object: other_interactive_objects){
        object->interact();
        if (object->getFlagDestroyAfterInteraction()){
            removeInteractiveObject(object->getXPos(), object->getYPos());
        }
        if(flag_was_army_eliminated_during_its_turn_){
            return;
        }
    }
    if (object_to_interact != nullptr && max_priority > OTHER_OBJECT_PRIORITY && !only_low_priority){
        object_to_interact->interact();
        if (object_to_interact->getFlagDestroyAfterInteraction()){
            removeInteractiveObject(object_to_interact->getXPos(), object_to_interact->getYPos());
        }
        if(flag_was_army_eliminated_during_its_turn_){
            return;
        }
    }
}

void WorldManager::initializeGenerators(){

    world_dynamic_objects_generators_.push_back(make_unique<WorldResourcesGenerator>(NUMBER_OF_WORLD_RESOURCES_ON_MAP));
    world_dynamic_objects_generators_.push_back(make_unique<WorldNeutralTroopsGenerator>(NUMBER_OF_NEUTRAL_TROOPS_ON_MAP));
    world_dynamic_objects_generators_.push_back(make_unique<ScholarsGenerator>(NUMBER_OF_SCHOLARS_ON_MAP));
    world_dynamic_objects_generators_.push_back(make_unique<TreasureChestsGenerator>(NUMBER_OF_CHESTS_ON_MAP));

    world_static_objects_generators_.push_back(make_unique<MinesGenerator>(NUMBER_OF_MINES_ON_MAP));
    generateWorldObjects();
}

void WorldManager::generateWorldObjects(){
    for(const auto& generator: world_dynamic_objects_generators_){
        generator->updateObjects();
    }
}

void WorldManager::resetNeutralTroopsVisitFlag(){
    WorldNeutralTroopsGenerator& generator = static_cast<WorldNeutralTroopsGenerator&>(
        *world_dynamic_objects_generators_.at(NEUTRAL_ARMY_VECTOR_POS));

    for (const auto& army: generator.getWorldObjectsOnMap()){
        army.get().resetVisitFlag();
    }
}

void WorldManager::removeMineOwnershipOfPlayer(const int player_id){
    MinesGenerator& generator = static_cast<MinesGenerator&>(*world_static_objects_generators_.at(MINE_GENERATOR_VECTOR_POS));
    for(const auto& mine: generator.getWorldObjectsOnMap()){
        if(mine.get().getPlayerID() == player_id){
            mine.get().setPlayerID(-1);
        }
    }
}

void WorldManager::setFlagWasArmyEliminatedDuringItsTurn(){
    flag_was_army_eliminated_during_its_turn_ = true;
}

TileType WorldManager::getTileTypeOfCurrentArmyTile(){
    int x_pos = getCurrentPlayer().getCurrentArmy().getXPos();
    int y_pos = getCurrentPlayer().getCurrentArmy().getYPos();
    return getWorldMap().getTileAt(x_pos, y_pos).getTileType();
}

BaseObjectGenerator& WorldManager::getObjectGenerator(const ObjectType object_type_generated_by_generator) const{
    for (const auto& generator: world_dynamic_objects_generators_){
        if(generator->getObjectType() == object_type_generated_by_generator){
            return *generator;
        }
    }
    for (const auto& generator: world_static_objects_generators_){
        if(generator->getObjectType() == object_type_generated_by_generator){
            return *generator;
        }
    }
    throw invalid_argument("No generator found for the specified object type.");
}


void WorldManager::resetManager(){
    current_day_ = 1;
    current_player_id_ = 0;
    flag_was_army_eliminated_during_its_turn_ = false;
    world_map_.reset();
    world_dynamic_objects_generators_.clear();
    world_static_objects_generators_.clear();
}