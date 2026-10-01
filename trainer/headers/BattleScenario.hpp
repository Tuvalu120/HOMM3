/**
 * @file BattleScenario.hpp
 * @author Bartosz Han
 * @brief This file contains BattleScenario struct, which contains
 * data about battlefield and armies needed for participating in battle.
 * Objects of this class are used in training and validating computer players.
 *
 */

#ifndef BATTLE_SCENARIO_HPP
#define BATTLE_SCENARIO_HPP

#include "Data/Constants.hpp"
#include "Data/GameData.hpp"
#include "Data/ObjectDataEnums.hpp"
#include "Utilities.hpp"
#include <algorithm>
#include <vector>

using namespace constants;
using namespace game_data;

/**
 * @struct BattleScenario
 * This struct contains
 * data about battlefield and armies needed for participating in battle.
 * Objects of this class are used in training and validating computer players.
 *
 */
struct BattleScenario{
    HeroType hero;
    TileType battlefield_type;
    std::vector<std::pair<HeroStat, int>> hero_stat_list;
    std::vector<std::pair<TroopType, int>> stack_list;
    std::vector<std::pair<SpellType, int>> spell_list;



    /**
     * @brief Creates a randomized BattleScenario with troops, hero, spells, stats, and battlefield type.
     *
     * This function generates a random battle scenario by:
     * - Randomly distributing a total stack power among a set number of troop stacks.
     * - Selecting random troop types of stacks and calculating their quantities.
     * - Randomly selecting a hero.
     * - Randomly distributing a fixed number of stat points among all hero stats.
     * - Randomly selecting a battlefield type.
     */
    static BattleScenario createRandomScenario(){

        //Data regarding creating scenario
        const int N_SPELLS_IN_BATTLE = MAX_NUMBER_OF_SPELLS;
        const int N_STACKS_IN_BATTLE = MAX_NUMBER_OF_STACKS;
        const int TOTAL_STACK_POWER = 20000;
        const int N_STATS_TO_ADD = 10;


        auto split_sum_into_vec = [](int sum, int n){
            std::vector<int> powers_vec = chooseNNumbersFromRange(1, sum-1, n-1);
            powers_vec.push_back(0);
            powers_vec.push_back(sum);
            std::sort(powers_vec.begin(), powers_vec.end());
            return powers_vec;
        };

        BattleScenario scenario;

        // Adding Troops

        std::vector<int> powers_vec = split_sum_into_vec(TOTAL_STACK_POWER, N_STACKS_IN_BATTLE);

        std::vector<TroopType> troops_in_battle = chooseNFromContainer(getTroopList(), N_STACKS_IN_BATTLE);

        for(int i=0; i<N_STACKS_IN_BATTLE; ++i){
            TroopType troop_type = troops_in_battle.at(i);
            float stack_total_power = powers_vec.at(i+1) - powers_vec.at(i);
            int troop_power = game_data::getTroopData(troop_type).troop_battle_data.troop_power;
            int quantity = ceil(stack_total_power / troop_power);
            scenario.stack_list.push_back({troop_type, quantity});
        }

        // Adding Hero

        auto hero_list = getHeroList();
        scenario.hero = chooseFromContainer(hero_list);


        // Adding spells

        if(scenario.hero != INVALID_HERO){

            std::vector<SpellType> spells_in_battle = chooseNFromContainer(getSpellList(), N_SPELLS_IN_BATTLE);

            for(const SpellType& spell: spells_in_battle){
                scenario.spell_list.push_back({spell, 1});
            }
        }

        // Adding Hero Stats

        if(scenario.hero != INVALID_HERO){

            auto stats = HERO_LEVEL_UP_STATISTICS;
            int n_stats = stats.size();

            std::vector<int> stats_splits = split_sum_into_vec(N_STATS_TO_ADD, n_stats);

            for(int i=0; i<n_stats; ++i){
                HeroStat stat = stats.at(i);
                int value = stats_splits.at(i+1) - stats_splits.at(i);
                scenario.hero_stat_list.push_back({stat, value});
            }

        }

        // Adding Battlefield type

        auto battlefield_styles = BATTLEFIELD_STYLES;
        scenario.battlefield_type = chooseFromContainer(battlefield_styles);

        //

        return scenario;
    }
};

#endif