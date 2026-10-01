/**
 * @file Spell.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Spell and definition of classes which inherits from it.
 *
 */


#include "GameObjects/Spell.h"
#include "Data/Constants.hpp"
#include "Data/ObjectDataEnums.hpp"
#include "GameObjects/Army.h"
#include "Managers/BattleManager.h"
#include "Miscellaneous/IPainter.h"

using namespace std;
using namespace game_data;
using namespace constants;

Spell::Spell(const SpellType spell_type, const int init_level_of_spell,
    const int maximum_level_of_spell):
    DrawableObject(getSpellData(spell_type).texture_name),
    spell_name_(getSpellData(spell_type).spell_name),
    spell_type_(spell_type),
    spell_family_(getSpellData(spell_type).spell_family),
    spell_description_(getSpellData(spell_type).spell_description),
    statistics_(getSpellData(spell_type).spell_battle_data.toArray()),
    current_level_of_spell_(init_level_of_spell),
    maximum_level_of_spell_(maximum_level_of_spell){}

SpellType Spell::getSpellType() const{
    return spell_type_;
}

int Spell::getLevelOfSpell() const{
    return current_level_of_spell_;
}

void Spell::setLevelOfSpell(const int new_spell_level){
    int spell_level  = min(new_spell_level, maximum_level_of_spell_);
    spell_level      = max(spell_level, 1);
    current_level_of_spell_ = spell_level;
}

int Spell::getStatistic(SpellStat stat) const{
    return statistics_.at(stat).at(current_level_of_spell_-1);
}

const string Spell::getSpellName() const{
    return spell_name_;
}

const string Spell::getSpellDescription() const{
    return spell_description_;
}

SpellFamily Spell::getSpellFamily() const{
    return spell_family_;
}

void Spell::drawAdditionalData(IPainter& painter) const{
    painter.drawNumberOnPixmap(getLevelOfSpell());
}

float Spell::calculateCastingScore(std::pair<int, int> pos) const{
    return calculateCastingScore(pos.first, pos.second);
}

BuffSpell::BuffSpell(const SpellType spell_type, const int init_level_of_spell,
    const int maximum_level_of_spell):Spell(spell_type, init_level_of_spell, maximum_level_of_spell),
    stats_(getSpellData(spell_type).spell_battle_data.troop_stats_to_add){};


DamageSpell::DamageSpell(const SpellType spell_type, const int init_level_of_spell,
    const int maximum_level_of_spell):Spell(spell_type, init_level_of_spell, maximum_level_of_spell){};

int DamageSpell::calculateDamage() const{
    return getStatistic(BASE_DAMAGE) + (getStatistic(DAMAGE_PER_SPELL_POWER_POINT) *
                getBattle().getCurrentArmy().getArmyHeroPtr()->getStatistic(SPELL_POWER));
}


AreaOfEffectBuffSpell::AreaOfEffectBuffSpell(const SpellType spell_type,
    const int init_level_of_spell,
    const int maximum_level_of_spell):
    BuffSpell(spell_type, init_level_of_spell, maximum_level_of_spell){};

vector<Stack*> AreaOfEffectBuffSpell::getStacksOfFriendlyArmyWithinRange(int x_pos, int y_pos) const{

    vector<pair<int, int>> event_tiles = getBattle().getBattlefield().getTilesWithinRadius(
        x_pos, y_pos, getStatistic(RADIUS));

    Army& friendly_army = getBattle().getCurrentArmy();
    vector<Stack*> event_stacks;

    for(const auto& stack: friendly_army.getArmyStacks()){
        int stack_x_pos = stack->getXPos();
        int stack_y_pos = stack->getYPos();
        pair<int, int> pos = {stack_x_pos, stack_y_pos};
        if(find(event_tiles.begin(), event_tiles.end(), pos) != event_tiles.end()){

            event_stacks.push_back(stack.get());
        }
    }
    return event_stacks;
}

bool AreaOfEffectBuffSpell::cast(Tile& event_tile){

    for(Stack* troop: getStacksOfFriendlyArmyWithinRange(event_tile.getXPos(), event_tile.getYPos())){
        for(const TroopStat& troop_stat: stats_){
            troop->addStatistic(troop_stat, getStatistic(STATS_ADDED));
        }
    }
    return true;
};

float AreaOfEffectBuffSpell::calculateCastingScore(int x_pos, int y_pos) const{

    float score = 0;
    for(const auto& stack : getStacksOfFriendlyArmyWithinRange(x_pos, y_pos)){
        int amount_to_increase = getStatistic(STATS_ADDED);
        float avg_damage = stack->getAverageDamagePerTroop();
        for(TroopStat stat : stats_){
            score += amount_to_increase * TROOP_BATTLE_POINTS_TO_PERCENTAGE.at(stat) * avg_damage;
        }
    }
    return score;
}

AreaOfEffectDamageSpell::AreaOfEffectDamageSpell(const SpellType spell_type, const int init_level_of_spell,
    const int maximum_level_of_spell):
    DamageSpell(spell_type, init_level_of_spell, maximum_level_of_spell){};

vector<Stack*> AreaOfEffectDamageSpell::getStacksOfEnemyArmyWithinRange(int x_pos, int y_pos) const{

    vector<pair<int, int>> event_tiles = getBattle().getBattlefield().getTilesWithinRadius(
        x_pos, y_pos, getStatistic(RADIUS));

    Army& enemy_army = getBattle().getOppositeArmy(getBattle().getCurrentArmy().getPlayerID());
    vector<Stack*> event_stacks;

    for(const auto& stack: enemy_army.getArmyStacks()){
        int stack_x_pos = stack->getXPos();
        int stack_y_pos = stack->getYPos();
        pair<int, int> pos = {stack_x_pos, stack_y_pos};
        if(find(event_tiles.begin(), event_tiles.end(), pos) != event_tiles.end()){

            event_stacks.push_back(stack.get());
        }
    }
    return event_stacks;
}

bool AreaOfEffectDamageSpell::cast(Tile& event_tile){

    for(const auto& troop: getStacksOfEnemyArmyWithinRange(event_tile.getXPos(), event_tile.getYPos())){
        troop->receiveDamage(calculateDamage());
    }

    return true;

};

float AreaOfEffectDamageSpell::calculateCastingScore(int x_pos, int y_pos) const{
    float score = 0;
    for(const auto& stack: getStacksOfEnemyArmyWithinRange(x_pos, y_pos)){
        int damage = calculateDamage();
        score += stack->calculateDecreaseInAttackIfDamaged(damage);
    }
    return score;
}

SingleTargetBuffSpell::SingleTargetBuffSpell(const SpellType spell_type, const int init_level_of_spell,
    const int maximum_level_of_spell):
    BuffSpell(spell_type, init_level_of_spell, maximum_level_of_spell){};

bool SingleTargetBuffSpell::cast(Tile& event_tile){
    int target_x_pos = event_tile.getXPos();
    int target_y_pos = event_tile.getYPos();
    Army& friendly_army = getBattle().getCurrentArmy();

    for(const auto& stack: friendly_army.getArmyStacks()){
        int stack_x_pos = stack->getXPos();
        int stack_y_pos = stack->getYPos();
        if(stack_x_pos == target_x_pos && stack_y_pos == target_y_pos){
            for(const TroopStat& troop_stat: stats_){
                stack->addStatistic(troop_stat, getStatistic(STATS_ADDED));
                return true;
            }
        }
    }
    return false;

};

float SingleTargetBuffSpell::calculateCastingScore(int x_pos, int y_pos) const{
    if(!getBattle().getBattlefield().getTileAt(x_pos, y_pos).hasInteractiveObject()){
        return 0;
    }
    for(const auto& stack : getBattle().getCurrentArmy().getArmyStacks()){
        if(stack->getXPos() == x_pos && stack->getYPos() == y_pos){
            int amount_to_increase = getStatistic(STATS_ADDED);
            float score = 0;
            float avg_damage = stack->getAverageDamagePerTroop();
            for(TroopStat stat : stats_){
                score += amount_to_increase * TROOP_BATTLE_POINTS_TO_PERCENTAGE.at(stat) * avg_damage;
            }
            return score;
        }
    }
    return 0.0f; //There was enemy Stack on this tile
}

SingleTargetDamageSpell::SingleTargetDamageSpell(const SpellType spell_type,
    const int init_level_of_spell,
    const int maximum_level_of_spell):
    DamageSpell(spell_type, init_level_of_spell, maximum_level_of_spell){};

bool SingleTargetDamageSpell::cast(Tile& event_tile){
    int target_x_pos = event_tile.getXPos();
    int target_y_pos = event_tile.getYPos();

    Army& enemy_army = getBattle().getOppositeArmy(getBattle().getCurrentArmy().getPlayerID());

    for(const auto& stack: enemy_army.getArmyStacks()){
        int stack_x_pos = stack->getXPos();
        int stack_y_pos = stack->getYPos();
        if(stack_x_pos == target_x_pos && stack_y_pos == target_y_pos){

            stack->receiveDamage(calculateDamage());
            return true;
        }
    }
    return false;
};

float SingleTargetDamageSpell::calculateCastingScore(int x_pos, int y_pos) const{
    if(!getBattle().getBattlefield().getTileAt(x_pos, y_pos).hasInteractiveObject()){
        return 0;
    }
    for(const auto& stack : getBattle().getOppositeArmy().getArmyStacks()){
        if(stack->getXPos() == x_pos && stack->getYPos() == y_pos){
            int damage = calculateDamage();
            return stack->calculateDecreaseInAttackIfDamaged(damage);
        }
    }
    return 0; //There was friendly Stack on this tile
}


std::unique_ptr<Spell> Spell::generateSpell(const SpellType spell_type){
    switch (getSpellData(spell_type).spell_family){
        case SINGLE_TARGET_BUFF_SPELL:
            return make_unique<SingleTargetBuffSpell>(spell_type);
        case AREA_OF_EFFECT_BUFF_SPELL:
            return make_unique<AreaOfEffectBuffSpell>(spell_type);
        case SINGLE_TARGET_DAMAGE_SPELL:
            return make_unique<SingleTargetDamageSpell>(spell_type);
        case AREA_OF_EFFECT_DAMAGE_SPELL:
            return make_unique<AreaOfEffectDamageSpell>(spell_type);
        default:
            throw InvalidIDError("SpellFamily", getSpellData(spell_type).spell_family);
    }
}
