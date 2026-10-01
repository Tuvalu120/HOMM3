/**
 * @file Hero.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Hero.
 *
 */


#include <iostream>

#include "GameObjects/Hero.h"
#include "Data/GameData.hpp"
#include "Decisions/Decision.h"
#include "Managers/PlayerManager.h"
#include "Managers/DecisionManager.h"
#include "Miscellaneous/GameExceptions.hpp"

using namespace std;
using namespace game_data;
using namespace constants;

Hero::Hero(const HeroType hero_type,
    const int player_id):
    DrawableObject(getHeroData(hero_type).texture_name),
    BattleObject(HERO, player_id, getHeroData(hero_type).hero_battle_data.speed),
    hero_type_(hero_type),
    hero_name_(getHeroData(hero_type).hero_name),
    statistics_(getHeroData(hero_type).hero_battle_data.toArray()),
    current_level_(1),
    experience_points_(0),
    spell_chosen_(-1),
    mana_points_(0){

        PlayerManager::getInstance().validatePlayerID(player_id, "Hero");

    };

bool Hero::attack(Tile& event_tile){
    if (hasSpellAt(spell_chosen_) && canCastSpellAt(spell_chosen_)){
        bool result = getSpellAt(spell_chosen_).cast(event_tile);
        if(result){
            reduceManaPoints(getSpellAt(spell_chosen_).getStatistic(COST));
        }
        spell_chosen_ = -1;
        return result;
    }
    else{
        cout << "Cannot cast spell at position " << spell_chosen_ << endl;
        return false;
    }
}

bool Hero::hasSpellAt(const int index_to_check) const{
    return !(index_to_check == -1) && spells_.at(index_to_check) != nullptr;
}

bool Hero::canAddNewSpell(){
    for(size_t i=0; i<spells_.size(); ++i){
        if(!hasSpellAt(i)){
            return true;
        }
    }
    return false;
}

Spell& Hero::getSpellAt(const int index) const{
    if (hasSpellAt(index)){
        return *spells_.at(index);
    }
    throw UninitializedObjectError("Spell");
}

void Hero::chooseSpell(const int index){
    if (index < 0 || index >= MAX_NUMBER_OF_SPELLS){
        throw invalid_argument("Invalid Hero's spell index: " + to_string(index));
    }
    if (!hasSpellAt(index)){
        throw UninitializedObjectError("Spell");

    }
    spell_chosen_ = index;
}

int Hero::getSpellChosenIndex() const{
    return spell_chosen_;
}

bool Hero::addSpell(SpellType spell_type, int spell_level){
    int index = -1;
    for(int i = 0; i < MAX_NUMBER_OF_SPELLS; ++i){
        if(hasSpellAt(i) && getSpellAt(i).getSpellType() == spell_type){
            if(getSpellAt(i).getLevelOfSpell() < spell_level){
                //Player already has this spell, but with smaller level, so it can be upgraded.
                getSpellAt(i).setLevelOfSpell(spell_level);
                return true;
            }
            else{
                //Player already has this spell with bigger level.
                return false;
            }
        }
        //Player has empty slot for spell and it can be added.
        else if (!hasSpellAt(i) && index == -1){
            index = i;
        }
    }
    if (index == -1){
        //Player already has maximum number of spells.
        return false;
    }

    spells_.at(index) = Spell::generateSpell(spell_type);
    spells_.at(index)->setLevelOfSpell(spell_level);
    return true;
}

int Hero::getNOfSpells() const{
    int n_of_spells = 0;
    for(int i=0; i<MAX_NUMBER_OF_SPELLS; ++i){
        if(spells_.at(i) != nullptr){
            ++n_of_spells;
        }
    }
    return n_of_spells;
}

const array<int, N_HERO_STATS>& Hero::getStatistics() const{
    return statistics_;
}
int Hero::getStatistic(HeroStat stat) const{
    return statistics_.at(stat);
}
void Hero::addStatistic(HeroStat stat_to_add, const unsigned int amount){
    statistics_.at(stat_to_add) += amount;
}

shared_ptr<array<int, N_HERO_STATS>> const Hero::getStatisticsPtr() const{
    return make_shared<array<int, N_HERO_STATS>>(statistics_);
}

const array<unique_ptr<Spell>, MAX_NUMBER_OF_SPELLS>& Hero::getHeroSpells() const{
    return spells_;
}

const string Hero::getHeroName() const{
    return hero_name_;
}

const string Hero::getUITextureName() const{
    return getCurrentTextureName();
}

const pair<vector<string>, vector<string>> Hero::getDataText() const{
    vector<string> names = HERO_DISPLAY_STAT_TEXT;
    names.insert(names.begin(), getHeroName());

    vector<string> values = {""};
    for(const HeroStat& hero_stat: HERO_DISPLAY_STAT){
        values.push_back(to_string(getStatistic(hero_stat)));
    }
    names.push_back(MANA_POINTS_STRING);
    values.push_back(to_string(mana_points_));

    return {names, values};
}

int Hero::getHeroLevel() const{
    return current_level_;
}

int Hero::getHeroExperiencePoints() const{
    return experience_points_;
}

void Hero::addExperiencePoints(const unsigned int value){
    experience_points_ += value;
    while(current_level_ < MAX_LEVEL_OF_HERO && experience_points_ >= EXPERIENCE_POINTS_TO_LEVEL_UP.at(current_level_)){
        levelUp();
    }
}

void Hero::levelUp(){
    experience_points_ -= EXPERIENCE_POINTS_TO_LEVEL_UP.at(current_level_);
    ++current_level_;
    unique_ptr<LevelUpDecision> decision = make_unique<LevelUpDecision>();
    DecisionManager::getInstance().makeDecision(std::move(decision));
}

const vector<pair<SpellType, int>> Hero::getHeroSpellsData() const{
    vector<pair<SpellType, int>> spell_battle_data;
    for(const auto& spell: spells_){
        if(spell != nullptr){
            spell_battle_data.push_back({spell->getSpellType(), spell->getLevelOfSpell()});
        }
    }
    return spell_battle_data;
}

HeroType Hero::getHeroType() const{
    return hero_type_;
}

string Hero::getHeroLevelDescription() const{
    return "Poziom: " + to_string(current_level_) + " | Punkty doświadczenia: " +
    to_string(experience_points_) + " / " + to_string(EXPERIENCE_POINTS_TO_LEVEL_UP.at(current_level_));
}

int Hero::getManaPoints() const{
    return mana_points_;
}
void Hero::resetManaPoints(){
    mana_points_ = getStatistic(KNOWLEDGE) * KNOWLEDGE_MANA_POINTS_MULTIPLIER;
}

void Hero::reduceManaPoints(const int value){
    mana_points_ -= value;
}

bool Hero::canCastSpellAt(const int spell_id) const{
    return (hasSpellAt(spell_id)) && getSpellAt(spell_id).getStatistic(COST) <= mana_points_;
}

bool Hero::canCastAnySpell() const{
    for(int i=0; i<MAX_NUMBER_OF_SPELLS; ++i){
        if(canCastSpellAt(i)){
            return true;
        }
    }
    return false;
}

const string Hero::toString() const{
    return "Hero: " + getHeroName() + " of player: " + to_string(getPlayerID());
};
