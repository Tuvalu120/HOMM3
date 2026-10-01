/**
 * @file ObjectDataEnums.hpp
 * @author Bartosz Han
 * @brief This file contains enumerations and related functions for game object data types.
 *
 * This header defines various enums representing types and categories of objects used throughout the game.
 * Whenever there is some kind of object, which have distinctive changes in behaviour when its ID is changed,
 * this file should contain enum for all different types of object, each value of enum representing family of objects with same ID,
 * and same behaviour. This files contains list of different heros, troops, spells, etc, aut also some more abstract objects, such as
 * different moves on world map, or different town actions.
 *
 */
#ifndef OBJECTDATAENUMS_H
#define OBJECTDATAENUMS_H

#include <map>
#include <array>

/// @brief Enum to represent different GUI scenes and state of the game and current turn.
enum SceneType{
    WORLD,
    BATTLE,
    TOWN,
    TOWN_TROOP_BUYING,
    TOWN_HERO_BUYING,
    TOWN_SPELL_BUYING,
    WORLD_DECISION_MAKING,
    EMPTY_SCENE,
    N_GAME_SCENES
};

/// @brief Enum to represent different resources (currencies) of the game.
enum Resource{
    GOLD,
    WOOD,
    ORE,
    CRYSTAL,
    JEWEL,
    N_RESOURCES
};

/// @brief Array to represent set of different resources.
using ResourceArray = std::array<unsigned int, N_RESOURCES>;

/**
 * @brief Generates a ResourceArray from a map of resources and their quantities.
 *
 * This function initializes a ResourceArray and fills it with the quantities
 * specified in the input map.
 *
 * @param resources A map associating Resource enum values with their quantities.
 * @return ResourceArray An array containing the quantities for each resource type.
 */
inline ResourceArray generateResourceArray(const std::map<Resource, int>& resources) {
    ResourceArray arr{};
    for (int i = 0; i < N_RESOURCES; ++i) {
        Resource res = static_cast<Resource>(i);
        arr[i] = resources.count(res) ? resources.at(res) : 0;
    }
    return arr;
};

/// @brief Enum to represent different variants of towns in the game.
enum TownType{
    CASTLE,
    RAMPART,
    DUNGEON,
    N_TOWN_TYPES
};

/// @brief Enum to represent different variants of building of towns in the game.
enum BuildingType{
    HALL,
    FORTIFICATIONS,
    TAVERN,
    MAGE_GUILD,
    DWELLINGS,
    N_BUILDING_TYPES

};

/// @brief Enum to list all possible heroes in the game.
enum HeroType {
    LOYNIS,
    RION,
    SANYA,
    CAITLIN,
    ///
    MALCOM,
    HALON,
    TOROSAR,
    ELLESHAR,
    ///
    AISLINN,
    XARFAX,
    DEEMER,
    MALEKITH,
    ///
    N_HERO_TYPES,
    INVALID_HERO
};


/**
 * @brief Returns an array containing all possible HeroType values.
 *
 * @return std::array<HeroType, N_HERO_TYPES> Array of all valid hero types.
 */
inline std::array<HeroType, N_HERO_TYPES> getHeroList(){
    std::array<HeroType, N_HERO_TYPES> array;
    for( int i=0; i<N_HERO_TYPES; ++i){
        array.at(i) = static_cast<HeroType>(i);
    }
    return array;
}

/// @brief Enum to list all possible spells in the game.
enum SpellType{
    STONE_SKIN,
    PRAYER,
    BLOODLUST,
    AREA_STONE_SKIN,
    AREA_PRAYER,
    AREA_BLOODLUST,
    MAGIC_ARROW,
    LIGHTING_BOLT,
    IMPLOSION,
    FIREBALL,
    METEOR_SHOWER,
    ARMAGEDON,
    N_SPELL_TYPES
};

/**
 * @brief Returns an array containing all possible SpellType values.
 *
 * @return std::array<SpellType, N_SPELL_TYPES> Array of all valid spell types.
 */
inline std::array<SpellType, N_SPELL_TYPES> getSpellList(){
    std::array<SpellType, N_SPELL_TYPES> array;
    for( int i=0; i<N_SPELL_TYPES; ++i){
        array.at(i) = static_cast<SpellType>(i);
    }
    return array;
}

/// @brief Enum to list all possible troops in the game.
enum TroopType{
    PIKEMAN,
    CENTAUR,
    TROGLODYTE,
    ///
    ARCHER,
    DWARF,
    HARPY,
    ///
    GRIFFIN,
    WOOD_ELF,
    BEHOLDER,
    ///
    SWORDSMAN,
    PEGASUS,
    MEDUSA,
    ///
    MONK,
    DENDROID_GUARD,
    MINOTAUR,
    ///
    CAVALIER,
    UNICORN,
    MANTICORE,
    ///
    ANGEL,
    GREEN_DRAGON,
    RED_DRAGON,
    ///
    N_TROOP_TYPES
};

/**
 * @brief Returns an array containing all possible TroopType values.
 *
 * @return std::array<TroopType, N_TROOP_TYPES> Array of all valid troop types.
 */
inline std::array<TroopType, N_TROOP_TYPES> getTroopList(){
    std::array<TroopType, N_TROOP_TYPES> array;
    for( int i=0; i<N_TROOP_TYPES; ++i){
        array.at(i) = static_cast<TroopType>(i);
    }
    return array;
}

/// @brief Enum to represent all types of tiles in the game.
enum TileType{
    GRASS_TILE,
    ROUGH_TILE,
    SNOW_TILE,
    SWAMP_TILE,
    ///
    GRASS_BORDER_TILE,
    ROUGH_BORDER_TILE,
    SNOW_BORDER_TILE,
    SWAMP_BORDER_TILE,
    WORLD_BORDER_TILE,
    N_TILE_TYPES
};

/// @brief Enum to represent different types of decision player can make in town.
enum TownDecision{
    BUY_TROOP_1,
    BUY_TROOP_2,
    BUY_TROOP_3,
    BUY_TROOP_4,
    BUY_TROOP_5,
    BUY_TROOP_6,
    BUY_TROOP_7,
    BUY_HERO_1,
    BUY_HERO_2,
    BUY_HERO_3,
    BUY_SPELL_1,
    BUY_SPELL_2,
    BUY_SPELL_3,
    BUY_SPELL_4,
    BUY_SPELL_5,
    BUY_SPELL_6,
    BUY_SPELL_7,
    BUY_SPELL_8,
    BUY_SPELL_9,
    UPGRADE_HALL,
    UPGRADE_FORTIFICATIONS,
    UPGRADE_TAVERN,
    UPGRADE_MAGE_GUILD,
    UPGRADE_DWELLINGS,
    LEAVE_TOWN,
    N_TOWN_DECISIONS
};

/// @brief Enum to represent all different types of object, which can be present on world map and battlefield.
enum ObjectType{
    ARMY,
    PART_OF_TOWN,
    STACK,

    RESOURCE,
    NEUTRAL_TROOPS,
    MINE,
    SCHOLAR,
    TREASURE_CHEST,

    HERO,

    N_INTERACTIVE_TYPES,
    INVALID_OBJECT,
    UNKNOWN_OBJECT
};

/// @brief Enum to represent different moves, which player can make to get to neighbouring tile.
enum WorldMove{
    UP_LEFT,
    UP,
    UP_RIGHT,
    LEFT,
    RIGHT,
    DOWN_LEFT,
    DOWN,
    DOWN_RIGHT,
    NO_MOVE
};

/// @brief Enum to list all different types of decision options of all world decisions which player can make in game.
enum DecisionOptionType{
    ADD_TROOPS,
    ADD_RESOURCES,
    ADD_EXPERIENCE_POINTS,
    ADD_SPELL,
    ADD_HERO_STAT,
    PARTICIPATE_IN_BATTLE,
    DO_NOTHING
};

/// @brief Enum to list of all world decisions which player can make in the game.
enum DecisionType{
    NEUTRAL_TROOPS_DECISION,
    SCHOLAR_DECISION,
    LEVEL_UP_DECISION,
    CHEST_DECISION
};

/// @brief Enum to list all possible spell families (groups of similar spells).
enum SpellFamily{
    AREA_OF_EFFECT_BUFF_SPELL,
    AREA_OF_EFFECT_DAMAGE_SPELL,
    SINGLE_TARGET_BUFF_SPELL,
    SINGLE_TARGET_DAMAGE_SPELL
};

/// @brief Enum to list all hero's statistics.
enum HeroStat{
    HERO_ATTACK,
    HERO_DEFENSE,
    SPELL_POWER,
    KNOWLEDGE,
    MORALE,

    HERO_SPEED,
    MAX_MOVES,
    DISCOVERY_RADIUS,

    N_HERO_STATS
};

/// @brief Enum to list all troop's statistics.
enum TroopStat{
    TROOP_LEVEL,
    TROOP_ATTACK,
    TROOP_DEFENSE,
    TROOP_SPEED,
    RANGE,
    MIN_DAMAGE,
    MAX_DAMAGE,
    BASE_HEALTH,
    TROOP_POWER,
    GROWTH,

    N_TROOP_STATS
};

/// @brief Enum to list all town's statistics.
enum TownStat{
    GOLD_GENERATED,
    TOWN_ARMY_LEVEL,
    TROOP_GROWTH_MULTIPLIER,
    TROOP_GENERATION_LEVEL,
    SPELL_GENERATION_LEVEL,
    SPELL_BUYING_ENABLED,
    HERO_BUYING_ENABLED,
    N_TOWN_STATS
};

/// @brief Enum to list all spells's statistics.
enum SpellStat{
    DAMAGE_PER_SPELL_POWER_POINT,
    RADIUS,
    BASE_DAMAGE,
    STATS_ADDED,
    COST,
    N_SPELL_STATS
};

#endif