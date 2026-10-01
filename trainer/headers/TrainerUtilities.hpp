/**
 * @file TrainerUtilities.hpp
 * @author Bartosz Han
 * @brief This file contains various methods
 * used in artificial intelligence training.
 *
 */

#ifndef TRAINER_UTILITIES_HPP
#define TRAINER_UTILITIES_HPP

#include <string>

#include "GameObjects/HeroArmy.h"
#include "BattleScenario.hpp"

// /**
//  * @brief Generates a state vector of given size filled with -1.0f.
//  * @return std::vector<float> The generated vector.
//  */
// inline std::vector<float> generateEmptyStateVector(const int size){
//     std::vector<float> vector;
//     vector.assign(size, -1.0f);
//     return vector;
// }


/**
 * @brief Trims leading and trailing occurrences of a character from a string str.
 *
 * @param str The string to trim (modified in place).
 * @param chr The character to trim.
 * @return Reference to the trimmed string.
 */
inline std::string& trim(std::string& str, const char chr = ' ')
{
    str.erase(0, str.find_first_not_of(chr));
    str.erase(str.find_last_not_of(chr) + 1);
    return str;
}

/**
 * @brief Creates a HeroArmy object from a given BattleScenario.
 *
 * Initializes hero, stats, troops, and spells according to the scenario.
 *
 * @param scenario The battle scenario to use for army creation.
 * @param player_id The player ID to assign to the army.
 * @param stacks_handicap Multiplier for troop quantities.
 */
inline HeroArmy createArmyFromScenario(const BattleScenario scenario, const int player_id, const float stacks_handicap = 1.0f){

    //Hero and HeroArmy initialization

    std::shared_ptr<Hero> hero;

    if(scenario.hero != INVALID_HERO){ //INVALID_HERO indicates that battle would go without heroes.
        hero = std::make_shared<Hero>(scenario.hero, player_id);
    };

    HeroArmy army(hero);
    army.setPlayerID(player_id);

    //HeroStats

    for(const auto& stat_data: scenario.hero_stat_list){
        army.getArmyHero().addStatistic(stat_data.first, stat_data.second);
    }

    // Troops

    for(const auto& troop_data: scenario.stack_list){
        int quantity = troop_data.second * stacks_handicap;
        army.addTroops(troop_data.first, quantity);
    }

    // Spells

    for(const auto& spell_data: scenario.spell_list){
        army.getArmyHero().addSpell(spell_data.first, spell_data.second);
    }

    return army;
}
#endif