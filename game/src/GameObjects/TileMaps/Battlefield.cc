/**
 * @file Battlefield.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Battlefield.
 *
 */


#include <memory>

#include "GameObjects/TileMaps/Battlefield.h"
#include "GameObjects/TileMaps/BattlefieldTile.hpp"
#include "GameObjects/Stack.h"
#include "GameObjects/Army.h"
#include "Managers/BattleManager.h"
#include "Data/GameData.hpp"
#include "Data/Constants.hpp"
#include "Miscellaneous/GameExceptions.hpp"

using namespace std;
using namespace game_data;
using namespace constants;


Battlefield::Battlefield(Army& attacking_army, Army& defending_army, const TileType battlefield_style):
    TileMap(), attacking_army_(attacking_army), defending_army_(defending_army), battlefield_style_(
        battlefield_style){

    for (int i = 0; i < MAX_NUMBER_OF_STACKS; ++i){
        init_attacker_spawn_tiles_.at(i) = pair<int, int>(-1, -1);
        init_defender_spawn_tiles_.at(i) = pair<int, int>(-1, -1);
    }
}

void Battlefield::addTileToMap(const char tile_type,
    const int x_pos, const int y_pos){

    unique_ptr<BattlefieldTile> new_tile;

    if (tile_type >= 'A' && tile_type <= 'H'){
        int stack_index = tile_type - 'A';
        addStackSpawnPos(true, stack_index, x_pos, y_pos);
        new_tile = make_unique<BattlefieldTile>(x_pos, y_pos, getBattlefieldStyle());
    }
    else if (tile_type >= 'a' && tile_type <= 'h'){
        int stack_index = tile_type - 'a';
        addStackSpawnPos(false, stack_index, x_pos, y_pos);
        new_tile = make_unique<BattlefieldTile>(x_pos, y_pos, getBattlefieldStyle());
    }
    else{

        switch (tile_type)
        {
        case 'T': //Tile
            new_tile = make_unique<BattlefieldTile>(x_pos, y_pos, getBattlefieldStyle());
            break;

        case 'X': //Wall
            new_tile = make_unique<BattlefieldTile>(x_pos, y_pos, WORLD_BORDER_TILE, 1, true,
                ADDITIONAL_BATTLEFIELD_OBJECTS_TEXTURES.at(battlefield_style_).at(current_additional_texture_index_));

            new_tile->setCurrentTextureName(TILE_TEXTURES.at(battlefield_style_).at(
                generateRandomIntInRange(0, TILE_TEXTURES.at(battlefield_style_).size()-1)));
            ++current_additional_texture_index_;
            break;

        case 'R': //heRo
            attacker_hero_spawning_pos_ = pair<int, int>(x_pos, y_pos);
            new_tile = make_unique<BattlefieldTile>(x_pos, y_pos, battlefield_style_, 1, true);
            break;

        case 'r': //heRo
            defender_hero_spawning_pos_ = pair<int, int>(x_pos, y_pos);
            new_tile = make_unique<BattlefieldTile>(x_pos, y_pos, battlefield_style_, 1, true);
            break;

        case 'O': //bOrder
            new_tile = make_unique<BattlefieldTile>(x_pos, y_pos, Tile::getBorderTileTypeFromWalkableTileType(battlefield_style_),
                1, true, WORLD_MAP_BORDERS.at(battlefield_style_));
            break;

        default:
            throw UnknownMapCharError(tile_type, BATTLEFIELD_MAPS.at(battlefield_style_));
            break;
        }
    }

    tile_map_.push_back(std::move(new_tile));
};

void Battlefield::initializeMap(){
    current_additional_texture_index_ = 0;
    readMapDataFromFile(ASSETS_PATH + BATTLEFIELD_MAPS.at(battlefield_style_));
}

TileType Battlefield::getBattlefieldStyle() const{
    return battlefield_style_;
}

// spawn_for_attacking_army = true if troop belongs for attacking army.
// spawn_for_attacking_army = false if troop belongs for defending army.
void Battlefield::addStackSpawnPos(const bool spawn_for_attacking_army, const int stack_id,
    const int x_pos, const int y_pos){

       pair<int, int> new_spawn_pos(x_pos, y_pos);
       if (spawn_for_attacking_army){
        init_attacker_spawn_tiles_.at(stack_id) = new_spawn_pos;
       }
       else{
        init_defender_spawn_tiles_.at(stack_id) = new_spawn_pos;
       }
    }

const array<pair<int, int>, MAX_NUMBER_OF_STACKS> Battlefield::getInitAttackerStacksSpawnPos() const{
    return init_attacker_spawn_tiles_;
}

const array<pair<int, int>, MAX_NUMBER_OF_STACKS> Battlefield::getInitDefenderStacksSpawnPos() const{
    return init_defender_spawn_tiles_;
}

const pair<int, int> Battlefield::getDefenderHeroSpawningPos() const{
    return defender_hero_spawning_pos_;
}
const pair<int, int> Battlefield::getAttackerHeroSpawningPos() const{
    return attacker_hero_spawning_pos_;
}

bool Battlefield::checkDistanceFromTarget(const Stack& stack_to_check,
    const int x_stack, const int y_stack, const int x_target, const int y_target)  const{

       int range = stack_to_check.getStatistic(RANGE);
       int distance = roundf(getDistanceBetweenTiles(x_stack, y_stack, x_target, y_target));

    return range >= distance;
}

bool Battlefield::checkForObstaclesOnPath(const int x_stack, const int y_stack, const int x_target, const int y_target)  const{

    float x_pos = x_stack + 0.5f;
    float y_pos = y_stack + 0.5f;

    int delta_x = x_target - x_stack;
    int delta_y = y_target - y_stack;

    int n_steps = max(abs(delta_x), abs(delta_y));

    float x_step = static_cast<float>(delta_x) / n_steps;
    float y_step = static_cast<float>(delta_y) / n_steps;

    int tile_x;
    int tile_y;

    for (int i = 0; i <= n_steps; ++i){

        tile_x = static_cast<int>(x_pos);
        tile_y = static_cast<int>(y_pos);

        if (!getTileAt(tile_x, tile_y).isTileWalkable()
            && (tile_x != x_stack || tile_y != y_stack) && (tile_x != x_target || tile_y != y_target))
        {
            return true;
        }

        x_pos += x_step;
        y_pos += y_step;
    }

    return false;

}

bool Battlefield::canStackAttackFrom(const Stack& stack, const int x_stack, const int y_stack, const int x_target, const int y_target) const{
    bool is_stack_at_check_coordinates = stack.getXPos() == x_stack &&
        stack.getYPos() == y_stack;

    bool check_if_tile_is_walkable = is_stack_at_check_coordinates ? true : getTileAt(x_stack, y_stack).isTileWalkable();

    return check_if_tile_is_walkable && checkDistanceFromTarget(stack, x_stack, y_stack, x_target, y_target)
        && !checkForObstaclesOnPath(x_stack, y_stack, x_target, y_target);
}



bool Battlefield::canCurrentStackAttackFrom(const int x_stack, const int y_stack, const int x_target, const int y_target)  const{

    return canStackAttackFrom(getBattle().getCurrentStack(), x_stack, y_stack, x_target, y_target);

}


bool Battlefield::canCurrentStackAttackEnemyStackThisTurn(const int enemy_x_pos, const int enemy_y_pos) const{

    return getBattle().getCurrentStack().canAttackEnemyStackThisTurn(enemy_x_pos, enemy_y_pos);

}

bool Battlefield::canCurrentStackAttackEnemyStackThisTurn(const InteractiveObject& object) const{
    return canCurrentStackAttackEnemyStackThisTurn(object.getXPos(), object.getYPos());
}



function<bool(const int, const int, const int, const int)> Battlefield::generateCurrentStackEndConditionFunction() const{
    return [this](const int x_start, const int y_start, const int x_end, const int y_end){
        return this->canCurrentStackAttackFrom(x_start, y_start, x_end, y_end);
    };
}

function<bool(const int, const int, const int, const int)> Battlefield::generateStackEndConditionFunction(const Stack& stack) const{
    return [this, &stack](const int x_start, const int y_start, const int x_end, const int y_end){
        return this->canStackAttackFrom(stack, x_start, y_start, x_end, y_end);
    };
}

Army& Battlefield::getAttackingArmy() const{
    return attacking_army_;
}

Army& Battlefield::getDefendingArmy() const{
    return defending_army_;
}

std::vector<std::pair<int, int>> Battlefield::getTilesWhereObjectCanGoTo(bool include_starting_tile){
    return TileMap::getTilesWhereObjectCanGoTo(getBattle().getCurrentStack(), include_starting_tile);
}

std::pair<int, int> Battlefield::getCoverValueOfTile(const int tile_x_pos, const int tile_y_pos){

    int possible_attack_damage = 0;
    int sum_of_distances = 0;

    Stack& current_stack = getBattle().getCurrentStack();
    int current_stack_x_pos = current_stack.getXPos();
    int current_stack_y_pos = current_stack.getYPos();
    bool perform_object_move = current_stack_x_pos == tile_x_pos && current_stack_y_pos == tile_y_pos;

    if(perform_object_move){
        moveObject(current_stack, tile_x_pos, tile_y_pos);
    }

    auto enemy_stacks = getBattle().getOppositeArmy().getArmyStacks();

    for(const auto& stack: enemy_stacks){

        const auto path_data = findPathToTile(stack->getXPos(), stack->getYPos(), tile_x_pos, tile_y_pos,
        generateStackEndConditionFunction(*stack));

        if(stack->canAttackEnemyStackThisTurn(tile_x_pos, tile_y_pos, path_data.first)){
            possible_attack_damage += stack->getCurrentAverageStackDamage();
        }
        int distance = path_data.second;
        if(distance == -1){
            distance = getTileMapHeight() * getTileMapWidth();
        }
        sum_of_distances += distance;
    };

    if(perform_object_move){
        moveObject(current_stack, current_stack_x_pos, current_stack_y_pos);
    };
    return make_pair(possible_attack_damage, sum_of_distances);
}

float Battlefield::getSumOfDistancesBetweenTileAndCurrentArmy(const int tile_x_pos, const int tile_y_pos) const{
    auto stacks = getBattle().getCurrentArmy().getArmyStacks();
    float sum_of_distances = 0.0f;

    for(const auto& stack: stacks){
        sum_of_distances += getDistanceBetweenTiles(stack->getXPos(), stack->getYPos(), tile_x_pos, tile_y_pos);
    }
    return sum_of_distances;

}

void Battlefield::spawnStacksOnBattlefield(Army& attacker, Army& defender){

    pair<int, int> spawn_pos;

    vector<shared_ptr<Stack>> attacker_troops = attacker.getArmyStacks();
    auto attacker_spawn_tiles = getInitAttackerStacksSpawnPos();

    for(size_t i = 0; i < attacker_troops.size(); ++ i){
        spawn_pos = attacker_spawn_tiles.at(i);
        moveObject(attacker_troops.at(i), spawn_pos.first, spawn_pos.second);
    }

    vector<shared_ptr<Stack>> defender_troops = defender.getArmyStacks();
    auto defender_spawn_tiles = getInitDefenderStacksSpawnPos();

    for(size_t i = 0; i < defender_troops.size(); ++ i){
        spawn_pos = defender_spawn_tiles.at(i);
        moveObject(defender_troops.at(i), spawn_pos.first, spawn_pos.second);
    }
}