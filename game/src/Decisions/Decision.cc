/**
 * @file Decision.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Decision and definition of classes which inherits from it.
 *
 */


#include "Decisions/Decision.h"
#include "GameObjects/Hero.h"
#include "GameObjects/Army.h"
#include "Managers/WorldManager.h"
#include "Data/GameData.hpp"

using namespace std;
using namespace game_data;
using namespace constants;

Decision::Decision(const DecisionType decision_type): decision_type_(decision_type){}

void Decision::chooseDecisionOption(const int option_id){
    if(static_cast<size_t>(option_id) >= decision_options_.size()){
        throw invalid_argument("DecisionOption");
    }
    decision_options_.at(option_id)->choose();
}

DecisionType Decision::getDecisionType() const{
    return decision_type_;
}

int Decision::getNOfDecisionOptions() const{
    return decision_options_.size();
}
DecisionOption& Decision::getDecisionOptionAt(const int index) const{
    if(static_cast<size_t>(index) >= decision_options_.size()){
        throw invalid_argument("Invalid decision option number");
    }
    return *decision_options_.at(index);
}

const std::vector<std::unique_ptr<DecisionOption>>& Decision::getDecisionOptions() const{
    return decision_options_;
}

string Decision::getDecisionName(){
    return decision_name_;
}


NeutralTroopsDecision::NeutralTroopsDecision(Army& decision_army):
    Decision(NEUTRAL_TROOPS_DECISION),army_(decision_army){

    decision_name_ = "Znalazłeś obóz z neutralnymi jednostkami";

    TroopType troop = decision_army.getArmyStacks().at(0)->getTroopType();
    int quantity = decision_army.getArmyStacks().at(0)->getQuantity();
    const unsigned int troops_cost = getTroopData(troop).troop_battle_data.troop_power * quantity;
    if(troops_cost <= WorldManager::getInstance().getCurrentPlayer().getPlayerResources().at(GOLD)){
        decision_options_.push_back(make_unique<AddTroopsDecisionOption>(troop, quantity, troops_cost));
    }
    decision_options_.push_back(make_unique<BattleDecisionOption>(decision_army));
    decision_options_.push_back(make_unique<DoNothingDecisionOption>());
}

void NeutralTroopsDecision::chooseDecisionOption(const int option_id){
    //Destroy army if AddTroopsDecisionOption was chosen
        if (getDecisionOptionAt(option_id).getDecisionOptionType() == ADD_TROOPS){
            army_.removeAllStacksFromArmy();
        }
        Decision::chooseDecisionOption(option_id);
    }


ScholarDecision::ScholarDecision():Decision(SCHOLAR_DECISION){

    decision_name_ = "Znalazłeś mędrca";

    Hero& current_hero = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero();
    vector<pair<SpellType, int>> init_spell_data = current_hero.getHeroSpellsData();
    vector<pair<SpellType, int>> level_up_spell_data;
    vector<pair<SpellType, int>> new_spell_data;
    vector<SpellType> init_spells;



    for(const pair<SpellType, int>& spell: init_spell_data){
        if(spell.second < MAX_LEVEL_OF_SPELL){
            level_up_spell_data.push_back({spell.first, spell.second+1});
        }
        init_spells.push_back(spell.first);
    }
    for(SpellType spell: getSpellList()){
        if(find(init_spells.begin(), init_spells.end(), spell) == init_spells.end()){
            new_spell_data.push_back({spell, 1});
        }
    }

    const pair<SpellType, int>& new_spell = chooseFromContainer(new_spell_data);
    array<HeroStat, N_OF_HERO_LEVEL_UP_STATISTICS> hero_stats = HERO_LEVEL_UP_STATISTICS;
    const HeroStat& hero_stat_to_add = chooseFromContainer(hero_stats);

    if(!level_up_spell_data.empty()){
        const pair<SpellType, int>& level_up_spell = chooseFromContainer(level_up_spell_data);
        decision_options_.push_back(make_unique<AddSpellDecisionOption>(level_up_spell.first, level_up_spell.second));
    }

    if(current_hero.canAddNewSpell()){
        decision_options_.push_back(make_unique<AddSpellDecisionOption>(new_spell.first, new_spell.second));
    };

    decision_options_.push_back(make_unique<AddHeroStatisticDecisionOption>(hero_stat_to_add, 1));

}

LevelUpDecision::LevelUpDecision():Decision(LEVEL_UP_DECISION){

    decision_name_ = "Zdobyłeś nowy poziom";

    Hero& current_hero = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero();
    const auto hero_statistics_probabilities = getHeroData(current_hero.getHeroType()).level_up_stat_probabilities;
    auto hero_level_up_stats = HERO_LEVEL_UP_STATISTICS;
    vector<HeroStat> chosen_statistics = chooseNUniqueElementsFromContainerWithDistribution(hero_level_up_stats,
        N_LEVEL_UP_DECISION_OPTIONS, hero_statistics_probabilities);

    for(const HeroStat& hero_stat: chosen_statistics){
        decision_options_.push_back(make_unique<AddHeroStatisticDecisionOption>(hero_stat, 1));
    }
}

ChestDecision::ChestDecision():Decision(CHEST_DECISION){

    decision_name_ = "Znalazłeś skrzynię ze skarbami";

    int chest_variant_chosen = generateRandomIntInRange(0, N_GOLD_CHESTS_VARIANTS-1);
    int gold_quantity = GOLD_AMOUNT_IN_CHESTS.at(chest_variant_chosen);
    int experience_points_quantity = EXPERIENCE_AMOUNT_IN_CHESTS.at(chest_variant_chosen);
    decision_options_.push_back((make_unique<AddResourceDecisionOption>(GOLD, gold_quantity)));
    decision_options_.push_back((make_unique<AddExperiencePointsDecisionOption>(experience_points_quantity)));
}
