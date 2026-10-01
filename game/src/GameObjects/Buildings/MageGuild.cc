/**
 * @file MageGuild.cc
 * @author Bartosz Han
 * @brief This file contains definition of class MageGuild.
 *
 */


#include "GameObjects/Buildings/MageGuild.h"
#include "Data/GameData.hpp"
#include "GameObjects/Town.h"

using namespace std;
using namespace game_data;
using namespace constants;

MageGuild::MageGuild(Town& town_of_building, vector<string> building_sprites):
    Building(town_of_building, MAX_LEVEL_OF_MAGE_GUILD, building_sprites){
        town_of_building.setStatistic(SPELL_BUYING_ENABLED, 0);
    };

void MageGuild::onLevelUp(){
    town_of_building_.setStatistic(SPELL_BUYING_ENABLED, 1);
    pair<int, int> current_level_spell_info = MAGE_GUILD_SPELLS_ADDED_ON_LEVEL_UP.at(building_level_-1);
    int spell_level = current_level_spell_info.first;
    int quantity = current_level_spell_info.second;
    auto spell_list = getSpellList();

    vector<SpellType> spell_types = chooseNUniqueElementsFromContainerWithDistribution(spell_list,
        quantity, town_of_building_.getTownData().spell_probabilities);

    for(const SpellType spell_type: spell_types){
        town_of_building_.addSpellToResources({spell_type, spell_level});
    }
}

const string MageGuild::getBuildingDescription() const{
    return generateBuildingDescription(game_data::MAGE_GUILD_NAME);
}

const ResourceArray& MageGuild::getUpgradeCost() const{

    if(getBuildingLevel() == getMaxBuildingLevel()){
        return EMPTY_RESOURCE_ARRAY;
    }
    return COST_OF_MAGE_GUILD_UPGRADE.at(building_level_);
}
