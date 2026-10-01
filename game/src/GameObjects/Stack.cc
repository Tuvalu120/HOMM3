/**
 * @file Stack.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Stack.
 *
 */


#include "GameObjects/Stack.h"
#include "Data/Constants.hpp"
#include "Data/ObjectDataEnums.hpp"
#include "Data/GameData.hpp"
#include "Miscellaneous/IPainter.h"
#include "Managers/PlayerManager.h"
#include "Managers/BattleManager.h"

#include <algorithm>

using namespace std;
using namespace game_data;
using namespace constants;

const array<int, N_HERO_STATS> Stack::null_hero_statistics_ = {};
std::function<float(Stack&, Stack&)> Stack::stack_damage_function_ = randomStackDamageFunction();

Stack::Stack(
    const TroopType troop_type,
    const int player_id,
    const int quantity):

    InteractiveObject(getTroopData(troop_type).texture_name, STACK, -1, -1),
    MovableObject(getTroopData(troop_type).texture_name, STACK, getTroopData(troop_type).troop_battle_data.speed),
    BattleObject(STACK, player_id, getTroopData(troop_type).troop_battle_data.speed),
    troop_type_(troop_type),
    troop_name_(game_data::getTroopName(troop_type)),
    statistics_(getTroopData(troop_type).troop_battle_data.toArray()),
    quantity_(quantity),
    current_health_(statistics_.at(BASE_HEALTH) * quantity),
    troop_battle_data_(getTroopData(troop_type).troop_battle_data){

        if (quantity <= 0){
            throw invalid_argument("Troop's quantity when initializing must be positive");
        }
        if(statistics_.at(BASE_HEALTH) <= 0){
            throw invalid_argument("Troop's base health must be positive");
        }
        PlayerManager::getInstance().validatePlayerID(player_id, "Troop", true);
        resetMoves();
    };

bool Stack::attack(Tile& event_tile){
    //Dealing damage
    if(event_tile.hasInteractiveObject()){
        return walkAndAttack(event_tile);
    }
    else{ //walking to the tile
        return walkTo(event_tile);
    }
}

void Stack::interact(Stack& event_stack){
    int damage_dealt = stack_damage_function_(*this, event_stack);
    event_stack.receiveDamage(damage_dealt);
}

int Stack::calculateDamage(const Stack& enemy_stack, const std::function<float()> damage_per_troop_function) const{
    float damage_multiplier = calculateDamageMultiplierTowardsEnemyStack(enemy_stack);

    float total_damage = 0.0;
    for (int i = 0; i< getQuantity(); ++i){
        total_damage += damage_per_troop_function();
    }
    total_damage *= damage_multiplier;
    return total_damage;
}

int Stack::calculateRandomStackDamage(const Stack& enemy_stack) const{

    function<float()> damage_function = [this](){return generateRandomIntInRange(getStatistic(MIN_DAMAGE), getStatistic(MAX_DAMAGE));};
    return calculateDamage(enemy_stack, damage_function);

}

float Stack::calculateAverageStackDamage(const Stack& enemy_stack) const{

    function<float()> damage_function = [this](){return getAverageDamagePerTroop();};
    return calculateDamage(enemy_stack, damage_function);

}

int  Stack::calculateMinimumDamage(const Stack& enemy_stack) const{

    function<float()> damage_function = [this](){return getStatistic(MIN_DAMAGE);};
    return calculateDamage(enemy_stack, damage_function);

}

int Stack::getQuantity() const{
    return quantity_;
}

int Stack::getCurrentHealth() const{
    return current_health_;
}

int Stack::getCurrentKnownHealth() const{
    return getStatistic(BASE_HEALTH) * getQuantity();
}

int Stack::healthToQuantity(const int health) const{
    int quantity = health / getStatistic(BASE_HEALTH);
    if (health > 0 && health % getStatistic(BASE_HEALTH) != 0){
        ++quantity;
    }
    return quantity;
}


void Stack::receiveDamage(const int damage){
    current_health_ -= damage;
    quantity_ = healthToQuantity(current_health_);

    if(quantity_ <= 0){
        setFlagDestroyAfterInteraction();
    }
}

bool Stack::walkTo(Tile& event_tile, bool stop_when_in_range){

    auto path_data = stop_when_in_range ? getBattle().getBattlefield().findPathToTile(
        getXPos(), getYPos(), event_tile.getXPos(), event_tile.getYPos(),
        getBattle().getBattlefield().generateCurrentStackEndConditionFunction()) :
        getBattle().getBattlefield().findPathToTile(getXPos(), getYPos(), event_tile.getXPos(), event_tile.getYPos());

    auto moves = path_data.first;
    int cost = path_data.second;

    if (cost == -1){
        cout << "Cannot find any path to " <<  event_tile.getXPos() << " " << event_tile.getYPos() << endl;
        return false;
    }

    for(WorldMove move: moves){
        getBattle().getBattlefield().moveObject(*this, move);
        removeMoves(getBattle().getBattlefield().getTileAt((getXPos(), getYPos())).getMoveCost());
        if(!canMove()){
            break;
        }
    }
    resetMoves();
    return true;

}

bool Stack::walkAndAttack(Tile& event_tile){

    int tile_x_pos = event_tile.getXPos();
    int tile_y_pos = event_tile.getYPos();

    for (auto troop: getBattle().getOppositeArmy(getPlayerID()).getArmyStacks()){
        if(troop->getXPos() == tile_x_pos && tile_y_pos == troop->getYPos()){
            if(walkTo(event_tile, true)){
                //Walk and attempt to deal damage
                if(getBattle().getBattlefield().canCurrentStackAttackFrom(getXPos(), getYPos(), tile_x_pos, tile_y_pos)){
                    interact(*troop);
                }
                return true;
            }
            //There is no path to the event tile.
            return false;
        }
    }
    cout << "Cannot do anything at tile " <<  event_tile.getXPos() << " " << event_tile.getYPos() << " - there is already friendly unit" << endl;
    return false;
}

void Stack::setHeroStatistics(shared_ptr<array<int, N_HERO_STATS>> statistics){
    hero_statistics_ = statistics;
}

const array<int, N_HERO_STATS>& Stack::getHeroStatistics() const{
    if (hero_statistics_ != nullptr){
        return *hero_statistics_.get();
    }
    return null_hero_statistics_;
}

const array<int, N_TROOP_STATS>& Stack::getStatistics() const{
    return statistics_;
}

int Stack::getStatistic(TroopStat stat) const{
    return statistics_.at(stat);
}
void Stack::addStatistic(TroopStat stat_to_add, int amount){
    statistics_.at(stat_to_add) += amount;
}

int Stack::getHeroStatistic(HeroStat stat) const{
    return getHeroStatistics().at(stat);
}

void Stack::addQuantity(const int value_to_add){
    quantity_ += value_to_add;
    current_health_ += statistics_.at(BASE_HEALTH) * value_to_add;
}

TroopType Stack::getTroopType() const{
    return troop_type_;
}

const string Stack::getUITextureName() const{
    return getCurrentTextureName();
}

const string Stack::getTroopName() const{
    return troop_name_;
}

const pair<vector<string>, vector<string>> Stack::getDataText() const{
    vector<string> names = TROOP_DISPLAY_STAT_TEXT;
    names.insert(names.begin(), getTroopName());

    vector<string> values = {""};
    for(const TroopStat& troop_stat: TROOP_DISPLAY_STAT){
        values.push_back(to_string(getStatistic(troop_stat)));
    }
    return {names, values};
}

void Stack::drawAdditionalData(IPainter& painter) const{
    painter.drawNumberOnPixmap(getQuantity(), playerIDToQColor(getPlayerID()));
}

void Stack::resetTroopData(){
    statistics_ = troop_battle_data_.toArray();
}

const string Stack::toString() const{
    return getTroopName() + " of player: " + to_string(getPlayerID()) + " with quantity of: " + std::to_string(getQuantity());
}


float Stack::getCurrentAverageStackDamage() const{
    return getCurrentAverageStackDamage(quantity_);
}

float Stack::getCurrentAverageStackDamage(const int scenario_quantity) const{
    float damage_multiplier = 1.0f + (ATTACK_POINT_TO_PERCENTAGE_RATIO * (getStatistic(TROOP_ATTACK) + getHeroStatistic(HERO_ATTACK) + getHeroStatistic(MORALE)));
    return scenario_quantity * getAverageDamagePerTroop() * damage_multiplier;
}

bool Stack::canAttackEnemyStackThisTurn(const int enemy_x_pos, const int enemy_y_pos, const std::vector<WorldMove> path_to_attack) const{
    int temp_x_pos = getXPos();
    int temp_y_pos = getYPos();
    int moves_left = getMovesLeft();


    if(getBattle().getBattlefield().canStackAttackFrom(*this, temp_x_pos, temp_y_pos, enemy_x_pos, enemy_y_pos)){
        return true;
    }

    for(const WorldMove& move: path_to_attack){
        std::pair<int, int> move_pair = Battlefield::worldMoveToInt(move);
        temp_x_pos += move_pair.first;
        temp_y_pos += move_pair.second;

        if(getBattle().getBattlefield().canStackAttackFrom(*this, temp_x_pos, temp_y_pos, enemy_x_pos, enemy_y_pos)){
            return true;
        }

        moves_left -= getBattle().getBattlefield().getTileAt((getXPos(), getYPos())).getMoveCost();
        if(moves_left <= 0){
            break;
        }
    }
    return false;
}

bool Stack::canAttackEnemyStackThisTurn(const int enemy_x_pos, const int enemy_y_pos) const{

    const auto path_data = getBattle().getBattlefield().findPathToTile(getXPos(), getYPos(), enemy_x_pos, enemy_y_pos,
        getBattle().getBattlefield().generateStackEndConditionFunction(*this)).first;

    return canAttackEnemyStackThisTurn(enemy_x_pos, enemy_y_pos, path_data);
}

bool Stack::canAttackEnemyStackThisTurn(const InteractiveObject& object) const{
    return canAttackEnemyStackThisTurn(object.getXPos(), object.getYPos());
}

bool Stack::canGetCloserToEnemyStackThisTurn(const int enemy_x_pos, const int enemy_y_pos) const{
    return getBattle().getBattlefield().findPathToTile(getXPos(), getYPos(), enemy_x_pos, enemy_y_pos, true).second != -1;
}

bool Stack::canGetCloserToEnemyStackThisTurn(const InteractiveObject& enemy_object) const{
    return getBattle().getBattlefield().findPathToTile(getXPos(), getYPos(), enemy_object.getXPos(), enemy_object.getYPos(), true).second != -1;
}

float Stack::getAverageDamagePerTroop() const{
    return getStatistic(MAX_DAMAGE) + getStatistic(MIN_DAMAGE) / 2.0f;
}

int Stack::receiveMockDamage(const int damage) const{
    int new_health = getCurrentHealth() - damage;
    int new_quantity = healthToQuantity(new_health);
    return max(new_quantity, 0);
}

int Stack::calculateDecreaseInAttackIfDamaged(const int damage) const{
    int new_quantity = receiveMockDamage(damage);
    int quantity_delta = quantity_ - new_quantity;
    return quantity_delta * getAverageDamagePerTroop();
}

std::function<float(Stack&, Stack&)> Stack::randomStackDamageFunction(){
    return [](Stack& attacker, Stack& attacked){return attacker.calculateRandomStackDamage(attacked);};
}

std::function<float(Stack&, Stack&)> Stack::avarageStackDamageFunction(){
    return [](Stack& attacker, Stack& attacked){return attacker.calculateAverageStackDamage(attacked);};
}

void Stack::registerStackDamageFunction(std::function<float(Stack&, Stack&)> function){
    stack_damage_function_ = function;
}

float Stack::calculateDamageMultiplierTowardsEnemyStack(const Stack& enemy_stack) const{
    float damage_multiplier = 1.0f;
    int hero_attack_pow = getHeroStatistic(HERO_ATTACK) - enemy_stack.getHeroStatistic(HERO_DEFENSE);
    int hero_morale = getHeroStatistic(MORALE) - enemy_stack.getHeroStatistic(MORALE);
    int troop_attacking_pow = getStatistic(TROOP_ATTACK) - enemy_stack.getStatistic(TROOP_DEFENSE);
    int attacking_power = hero_attack_pow + hero_morale + troop_attacking_pow;
    if (attacking_power >= 0){

        attacking_power = min(attacking_power, MAX_ATTACK_POINTS);
        damage_multiplier += attacking_power * ATTACK_POINT_TO_PERCENTAGE_RATIO;
    }

    else{

        attacking_power = min(-1 * attacking_power, MAX_DEFENSE_POINTS);
        damage_multiplier -= attacking_power * DEFENSE_POINT_TO_PERCENTAGE_RATIO;

    }
    return damage_multiplier;
}
