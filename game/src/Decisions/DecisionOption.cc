/**
 * @file DecisionOption.cc
 * @author Bartosz Han
 * @brief This file contains definition of class DecisionOption and definition of classes which inherits from it.
 *
 */


#include "Decisions/DecisionOption.h"
#include "Data/GameData.hpp"
#include "Managers/WorldManager.h"
#include "Managers/BattleManager.h"

using namespace std;
using namespace game_data;
using namespace constants;

DecisionOption::DecisionOption(const DecisionOptionType decision_option):
    decision_option_(decision_option){};

DecisionOptionType DecisionOption::getDecisionOptionType() const{
    return decision_option_;
}


////

AddTroopsDecisionOption::AddTroopsDecisionOption(const TroopType troop, const int quantity, const int troops_cost):
    DecisionOption(ADD_TROOPS), troop_type_(troop), troops_quantity_(quantity),
    cost_(troops_cost){
        setCurrentTextureName(getTroopTextureName(troop));
    };

void AddTroopsDecisionOption::choose(){
    if(WorldManager::getInstance().getCurrentPlayer().spendResources(GOLD, cost_)){
        WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().addTroops(troop_type_, troops_quantity_);
    }
    else{
        throw DecisionLogicError("Cannot generate AddTroopDecisionOption if Player does not have enough Gold");
    }
};

const string AddTroopsDecisionOption::getOptionName() const{
    return "Wykup stworzenia do swojej armii";
};

const pair<vector<string>, vector<string>> AddTroopsDecisionOption::getOptionDescription() const{
    vector<string> names = {"Nazwa jednostki", "Liczba jednostek", "Koszt wykupu (w złocie)"};
    vector<string> values = {getTroopName(troop_type_), to_string(troops_quantity_), to_string(cost_)};
    return {names, values};
};

TroopType AddTroopsDecisionOption::getTroopType() const{
    return troop_type_;
};

int AddTroopsDecisionOption::getTroopsQuantity() const{
    return troops_quantity_;
};

int AddTroopsDecisionOption::getCost() const{
    return cost_;
};

////

AddResourceDecisionOption::AddResourceDecisionOption(const Resource resource, const int quantity):
    DecisionOption(ADD_RESOURCES),resource_type_(resource), resource_quantity_(quantity){

    setCurrentTextureName(WORLD_RESOURCE_IMAGE.at(resource_type_));

}

void AddResourceDecisionOption::choose(){
    WorldManager::getInstance().getCurrentPlayer().addResources(resource_type_, resource_quantity_);
}

const string AddResourceDecisionOption::getOptionName() const{
    return "Zdobądź zasoby: " + RESOURCES_NAMES.at(resource_type_);
}

const pair<vector<string>, vector<string>> AddResourceDecisionOption::getOptionDescription() const{
    vector<string> names = {"Ilość"};
    vector<string> values = {to_string(resource_quantity_)};
    return {names, values};
}

Resource AddResourceDecisionOption::getResourceType() const{
    return resource_type_;
}

int AddResourceDecisionOption::getResourceQuantity() const{
    return resource_quantity_;
}

////

AddExperiencePointsDecisionOption::AddExperiencePointsDecisionOption(const int quantity):
    DecisionOption(ADD_EXPERIENCE_POINTS),quantity_(quantity){

        setCurrentTextureName("Icons/DecisionAddExperiencePointsImage.png");

}

void AddExperiencePointsDecisionOption::choose(){
    WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().addExperiencePoints(quantity_);
}

const string AddExperiencePointsDecisionOption::getOptionName() const{
    return "Zdobądź punkty doświadczenia";
}

const pair<vector<string>, vector<string>> AddExperiencePointsDecisionOption::getOptionDescription() const{
    vector<string> names = {"Ilość punktów"};
    vector<string> values = {to_string(quantity_)};
    return {names, values};
}

int AddExperiencePointsDecisionOption::getExperiencePoints() const{
    return quantity_;
}

////

AddSpellDecisionOption::AddSpellDecisionOption(const SpellType spell, const int level):
    DecisionOption(ADD_SPELL),spell_type_(spell), spell_level_(level){
        setCurrentTextureName(getSpellData(spell_type_).texture_name);
}

void AddSpellDecisionOption::choose(){
    WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().addSpell(spell_type_, spell_level_);
}

const string AddSpellDecisionOption::getOptionName() const{
    return "Naucz się zaklęcia:\n" + getSpellData(spell_type_).spell_name;
}

const pair<vector<string>, vector<string>> AddSpellDecisionOption::getOptionDescription() const{
    vector<string> names = {"Poziom"};
    vector<string> values = {to_string(spell_level_)};
    return {names, values};
}

SpellType AddSpellDecisionOption::getSpellType() const{
    return spell_type_;
}

int AddSpellDecisionOption::getSpellLevel() const{
    return spell_level_;
}

////

AddHeroStatisticDecisionOption::AddHeroStatisticDecisionOption(const HeroStat hero_stat, const int quantity):
    DecisionOption(ADD_HERO_STAT),hero_stat_(hero_stat),quantity_(quantity){
        setCurrentTextureName(HERO_STATISTICS_IMAGE.at(hero_stat));
    }

void AddHeroStatisticDecisionOption::choose(){
    WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().addStatistic(hero_stat_, quantity_);
}

const string AddHeroStatisticDecisionOption::getOptionName() const{
    return "Zwiększ atrybut: " + HERO_DISPLAY_STAT_TEXT.at(hero_stat_);
}

const pair<vector<string>, vector<string>> AddHeroStatisticDecisionOption::getOptionDescription() const{
    vector<string> names = {"Zwiększ o"};
    vector<string> values = {to_string(quantity_)};
    return {names, values};
}

HeroStat AddHeroStatisticDecisionOption::getHeroStat() const{
    return hero_stat_;
}

int AddHeroStatisticDecisionOption::getQuantity() const{
    return quantity_;
}

////

BattleDecisionOption::BattleDecisionOption(Army& decision_army):
    DecisionOption(PARTICIPATE_IN_BATTLE),
    army_(decision_army),troop_type_(decision_army.getArmyStacks().at(0)->getTroopType()),
    troop_quantity_(decision_army.getArmyStacks().at(0)->getQuantity()){
    setCurrentTextureName("Icons/DecisionBattleImage.png");
};

void BattleDecisionOption::choose(){
    BattleManager::getInstance().battle(WorldManager::getInstance().getCurrentPlayer().getCurrentArmy(), army_);
};

const string BattleDecisionOption::getOptionName() const{
    return "Walcz z armią";
}

const pair<vector<string>, vector<string>> BattleDecisionOption::getOptionDescription() const{
    vector<string> names = {"Nazwa jednostki", "Ilość jednostek"};
    vector<string> values = {getTroopName(troop_type_), to_string(troop_quantity_)};
    return {names, values};
}

TroopType BattleDecisionOption::getTroopType() const{
    return troop_type_;
}

int BattleDecisionOption::getTroopQuantity() const{
    return troop_quantity_;
}

////

DoNothingDecisionOption::DoNothingDecisionOption():
    DecisionOption(DO_NOTHING){
        setCurrentTextureName("Icons/DecisionDoNothingImage.png");
}

void DoNothingDecisionOption::choose(){}

const string DoNothingDecisionOption::getOptionName() const{
    return "Nie wykonuj żadnej akcji";
}

const pair<vector<string>, vector<string>> DoNothingDecisionOption::getOptionDescription() const{
    return {{""},{""}};
}