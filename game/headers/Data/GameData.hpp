/**
 * @file GameData.hpp
 * @author Bartosz Han
 * @brief This file contains core data structures and several constants for game entities and mechanics.
 *
 * This file defines several data structures and constants used to represent and define
 * towns, troops, heroes, spells, resources, and various game mechanics in the game.
 *
 * It includes:
 *  - Structures to represent towns, heroes, spells, troops, and their battle-related substructures,
 *    as well as functions retrieve data from them.
 *  - Data for all towns, troops, heroes, and spells available in the game.
 *  - Arrays and constants for resource types, probabilities, costs, and other gameplay parameters.
 *  - Asset paths for graphical representation of game resources.
 *
 * This file can be considered as an asset file, as it defines and declares various types of resources used in game.
 *
 */


#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "Data/Constants.hpp"
#include "Data/ObjectDataEnums.hpp"
#include "Miscellaneous/GameUtilities.hpp"
#include "Miscellaneous/GameExceptions.hpp"

/// @brief Array to represent different values of some spell statistic when level of spell is changed.
using SpellDataArray = std::array<unsigned int, constants::MAX_LEVEL_OF_SPELL>;

/// @brief Struct to represent state of resources in Town.
struct TownData{

    std::vector<TroopType> town_troops;
    std::array<int, N_HERO_TYPES> hero_probabilities;
    std::array<int, N_SPELL_TYPES> spell_probabilities;
    std::array<ResourceArray, constants::MAX_LEVEL_OF_DWELLINGS> cost_of_upgrading_dwelling;
    std::array<ResourceArray, constants::MAX_LEVEL_OF_DWELLINGS> cost_of_buying_troop;
    std::array<std::vector<std::string>, N_BUILDING_TYPES> town_building_textures;
    std::vector<std::vector<std::string>> town_textures;
};

/// @brief Struct to represent battle-related statistics of spells.
struct SpellBattleData
{
    SpellDataArray damage_per_spell_power_point = {};
    SpellDataArray radius = {};
    SpellDataArray base_damage = {};
    SpellDataArray stats_added = {};
    SpellDataArray cost = {};
    std::vector<TroopStat> troop_stats_to_add = {};

    /// @brief Convert SpellBattleData into array, in which every element defines one type of statistic of spell.
    std::array<SpellDataArray, N_SPELL_STATS> toArray() const{
        return {
            damage_per_spell_power_point,
            radius,
            base_damage,
            stats_added,
            cost
        };
    };
};

/// @brief Struct to represent spell and its statistics.
struct SpellData{
    SpellBattleData spell_battle_data;
    std::string spell_name;
    SpellFamily spell_family;
    std::string spell_description;
    std::string texture_name;
};

/// @brief Struct to represent battle-related statistics of heroes.
struct HeroBattleData
{
    int attack;
    int defense;
    int spell_power;
    int knowledge;
    int morale;
    int speed;
    int max_moves;
    int discovery_radius;

    /// @brief Convert HeroBattleData into array, in which every element defines one statistic of hero.
    std::array<int, N_HERO_STATS> toArray() const{
        return {attack,
                defense,
                spell_power,
                knowledge,
                morale,
                speed,
                max_moves,
                discovery_radius
                };
    };
};

/// @brief Struct to represent hero and its statistics.
struct HeroData{
    HeroBattleData hero_battle_data;
    std::string hero_name;
    std::string texture_name;
    std::vector<std::pair<TroopType, int>> init_army;
    std::vector<SpellType> init_spells;
    std::array<int, constants::N_OF_HERO_LEVEL_UP_STATISTICS> level_up_stat_probabilities;
};

/// @brief Struct to represent battle-related statistics of troops.
struct TroopBattleData{

    int level;
    int attack;
    int defense;
    int speed;
    int range;
    int min_damage;
    int max_damage;
    int base_health;
    int troop_power;
    int growth;

    /// @brief Convert TroopBattleData into array, in which every element defines one statistic of hero.
    std::array<int, N_TROOP_STATS> toArray() const{
        return {
            level,
            attack,
            defense,
            speed,
            range,
            min_damage,
            max_damage,
            base_health,
            troop_power,
            growth
        };
    };
};

/// @brief Struct to represent troops and its statistics.
struct TroopData{
        TroopBattleData troop_battle_data;
        std::string troop_name;
        std::string texture_name;
    };

/**
 * @namespace game_data
 * @brief This struct contains data representing various resources present in game, most notably world objects, towns, heroes, troops and spells.
 *
 */
namespace game_data{


    //AI

    const std::string AI_MODEL_FILEPATH = "assets/AI/model";

    //ARMY AND HERO

    /// @brief Array of int values, where each value represents how strong non-player-controlled army (armies in mines, towns, etc.) should be
    /// at given stage of game, base of game progress value.
    const std::array<int, constants::MAX_LEVEL_OF_DWELLINGS> PATTERN_ARMY_POWER = {279, 847, 1953, 3594, 5695, 8679, 12880};

    /// @brief Array of int values, where each value represents experience points needed by hero to increase their level by one.
    /// For example, hero with level 1 need as many experience points to level up as it is declared at index 1.
    /// @note As heroes start at level 1, first value of array is unused, and is here for convience.
    const std::array<int, constants::MAX_LEVEL_OF_HERO> EXPERIENCE_POINTS_TO_LEVEL_UP = {0, 750, 1500, 2400, 3450, 4650, 6000, 7500,
        9150, 11025, 13125, 15450, 18240, 21588, 25605, 30425, 36209, 43149, 51477, 61470};

    /// @brief Additional experience points for defeating enemy WorldArmy (any army directly controlled by another player).
    const int EXPERIENCE_BONUS_FROM_DEFEATING_ARMY_WITH_HERO = 500;

    /// @brief Additional experience points for capturing any town
    const int EXPERIENCE_BONUS_FROM_CAPTURING_TOWN = 500;

    const std::string MANA_POINTS_STRING = "Mana";

    /// @brief Order of hero statictic to display at hero statictic layout.
    const std::vector<HeroStat> HERO_DISPLAY_STAT = {HERO_ATTACK, HERO_DEFENSE, SPELL_POWER, KNOWLEDGE};

    const std::vector<std::string> HERO_DISPLAY_STAT_TEXT = {"Atak", "Obrona", "Wiedza", "Moc Zaklęć"};

    /// @brief Number of statictic to display at hero statistic layout.
    constexpr int HERO_STATISTICS_DISPLAY_COUNT = 4;

    /// @brief Vector of paths to hero statistics icons.
    const std::array<std::string, HERO_STATISTICS_DISPLAY_COUNT> HERO_STATISTICS_IMAGE = { "Icons/heroAttackIcon.png",
        "Icons/heroDefenseIcon.png", "Icons/heroSpellPowerIcon.png", "Icons/heroKnowledgeIcon.png"};

    const std::vector<std::string> TROOP_DISPLAY_STAT_TEXT = {"Atak", "Obrona", "Szybkość", "Zasięg ataku", "Min. obrażenia", "Max. obrażenia", "Zdrowie jednostki"};

    /// @brief Order of troop statictic to display at troop data layout.
    const std::vector<TroopStat> TROOP_DISPLAY_STAT = {TROOP_ATTACK, TROOP_DEFENSE, TROOP_SPEED, RANGE, MIN_DAMAGE, MAX_DAMAGE, BASE_HEALTH};

    //WORLD RESOURCES

    /// @brief Maximum number of WorldResource objects on map.
    const int NUMBER_OF_WORLD_RESOURCES_ON_MAP = 250;

    /// @brief All resource types available in game.
    /// @note Whenever there is a vector which references resources, the values in that vector represents resources at same index in this vector.
    const std::array<Resource, N_RESOURCES> RESOURCE_TYPES = {GOLD, WOOD, ORE, CRYSTAL, JEWEL};

    /// @brief Probabilities, in which new world generated resource will be certain type of resource.
    const std::array<unsigned int, N_RESOURCES> WORLD_RESOURCE_PROBABILITIES = {60, 15, 15, 5 ,5};

    /// @brief Possible ranges of quantities of world resources. First value of pair is minimum quantity, and second is maximum quantity.
    const std::array<std::pair<unsigned int, unsigned int>, N_RESOURCES> WORLD_RESOURCE_QUANTITY_PROBABILITIES = {{{500,1000}, {5,10}, {5,10}, {3,6}, {3,6}}};

    const std::array<std::string, N_RESOURCES> RESOURCES_NAMES = {"Złoto", "Drewno", "Ruda", "Kryształy", "Klejnoty"};

    /// @brief Vector of paths to world resources images.
    const std::array<std::string, N_RESOURCES> WORLD_RESOURCE_IMAGE = {"WorldObjects/WorldResourceGold.png", "WorldObjects/WorldResourceWood.png",
        "WorldObjects/WorldResourceOre.png", "WorldObjects/WorldResourceCrystal.png", "WorldObjects/WorldResourceJewel.png"};

    //MINES

    /// @brief Maximum number of Mine objects on map.
    const int NUMBER_OF_MINES_ON_MAP = 30;

    /// @brief Strength of armies defending mines. This value is for both neutral and player-captured mines.
    const int MINE_ARMY_LEVEL = 2;

    /// @brief Probabilities, in which new world generated mines will be generating certain type of resource.
    const std::array<unsigned int, N_RESOURCES> MINE_RESOURCE_PROBABILITIES = {60, 15, 15, 5 ,5};

    /// @brief Quantities of resources generated by mines.
    const std::array<unsigned int, N_RESOURCES> MINE_RESOURCE_QUANTITY = {1000, 2, 2, 1 ,1};

        /// @brief Vector of paths to world mines images.
    const std::array<std::string, N_RESOURCES> MINE_IMAGE = {"WorldObjects/MineGold.png", "WorldObjects/MineWood.png",
        "WorldObjects/MineOre.png", "WorldObjects/MineCrystal.png", "WorldObjects/MineJewel.png"};

    //SCHOLARS

    /// @brief Maximum number of Scholars objects on map.
    const int NUMBER_OF_SCHOLARS_ON_MAP = 50;

        /// @brief Path to world scholar image.
    const std::string SCHOLAR_IMAGE = "WorldObjects/Scholar.png";

    //TREASURE CHESTS

    /// @brief Maximum number of TreasureChest objects on map.
    const int NUMBER_OF_CHESTS_ON_MAP = 100;

    /// @brief Number of different treasure chest variants, which can be generated on the map.
    const int N_GOLD_CHESTS_VARIANTS = 3;

    /// @brief Possible amounts of gold in different variants of chests.
    const std::array<unsigned int, N_GOLD_CHESTS_VARIANTS> GOLD_AMOUNT_IN_CHESTS = {1000, 1500, 2000};

    /// @brief Possible amounts of experience points in different variants of chests.
    const std::array<unsigned int, N_GOLD_CHESTS_VARIANTS> EXPERIENCE_AMOUNT_IN_CHESTS = {500, 1000, 1500};

    /// @brief Path to treasure chest image.
    const std::string TREASURE_CHEST_IMAGE = "WorldObjects/WorldTreasureChest.png";


    //WORLD NEUTRAL TROOPS

    /// @brief Maximum number of WorldNeutral objects on map.
    const int NUMBER_OF_NEUTRAL_TROOPS_ON_MAP = 50;

    //TILE_DATA

    /// @brief Number of different textures of tiles.
    const int N_SINGLE_TYPE_TEXTURES = 10;

    /// @brief List of grass tile textures
    const std::vector<std::string> GRASS_TILE_TEXTURES = generateIncrementingStrings("Tiles/grass/GrassTile",  N_SINGLE_TYPE_TEXTURES-1, ".png", 1);

    /// @brief List of rough tile textures
    const std::vector<std::string> ROUGH_TILE_TEXTURES = generateIncrementingStrings("Tiles/rough/RoughTile",  N_SINGLE_TYPE_TEXTURES-1, ".png", 1);

    /// @brief List of snow tile textures
    const std::vector<std::string> SNOW_TILE_TEXTURES  = generateIncrementingStrings("Tiles/snow/SnowTile",    N_SINGLE_TYPE_TEXTURES-1, ".png", 1);

    /// @brief List of swamp tile textures
    const std::vector<std::string> SWAMP_TILE_TEXTURES = generateIncrementingStrings("Tiles/swamp/SwampTile",  N_SINGLE_TYPE_TEXTURES-1, ".png", 1);

    /// @brief List of all tile textures in game.
    const std::vector<std::vector<std::string>> TILE_TEXTURES = {GRASS_TILE_TEXTURES, ROUGH_TILE_TEXTURES, SNOW_TILE_TEXTURES, SWAMP_TILE_TEXTURES};

    /// @brief List of file paths of files containing text data describing battlefield maps.
    const std::vector<std::string> BATTLEFIELD_MAPS = {
        "Maps/GrassBattlefield.txt",
        "Maps/RoughBattlefield.txt",
        "Maps/SnowBattlefield.txt",
        "Maps/SwampBattlefield.txt"
    };


    /// @brief List of additional object textures in grass battlefields.
    const std::vector<std::string> GRASS_BATTLEFIELD_OBJECTS_TEXTURES = {
        "BattlefieldObjects/grass/grassA_0_0.png",
        "BattlefieldObjects/grass/grassA_0_1.png",
        "BattlefieldObjects/grass/grassA_0_2.png",
        "BattlefieldObjects/grass/grassB_0_0.png",
        "BattlefieldObjects/grass/grassB_0_1.png",
        "BattlefieldObjects/grass/grassB_0_2.png",
        "BattlefieldObjects/grass/grassB_0_3.png",
        "BattlefieldObjects/grass/grassB_1_0.png",
        "BattlefieldObjects/grass/grassB_1_1.png",
        "BattlefieldObjects/grass/grassB_1_2.png",
        "BattlefieldObjects/grass/grassB_1_3.png",
        "BattlefieldObjects/grass/grassB_2_0.png",
        "BattlefieldObjects/grass/grassB_2_1.png",
        "BattlefieldObjects/grass/grassB_2_2.png",
        "BattlefieldObjects/grass/grassB_2_3.png",
        "BattlefieldObjects/grass/grassB_3_0.png",
        "BattlefieldObjects/grass/grassB_3_1.png",
        "BattlefieldObjects/grass/grassB_3_2.png",
        "BattlefieldObjects/grass/grassB_3_3.png",
        "BattlefieldObjects/grass/grassA_0_0.png",
        "BattlefieldObjects/grass/grassA_0_1.png",
        "BattlefieldObjects/grass/grassA_0_2.png"
    };

    /// @brief List of additional object textures in rough battlefields.
    const std::vector<std::string> ROUGH_BATTLEFIELD_OBJECTS_TEXTURES = {
        "BattlefieldObjects/rough/roughA_0_0.png",
        "BattlefieldObjects/rough/roughA_0_1.png",
        "BattlefieldObjects/rough/roughC_0_0.png",
        "BattlefieldObjects/rough/roughC_0_1.png",
        "BattlefieldObjects/rough/roughA_1_0.png",
        "BattlefieldObjects/rough/roughA_1_1.png",
        "BattlefieldObjects/rough/roughC_1_0.png",
        "BattlefieldObjects/rough/roughC_1_1.png",
        "BattlefieldObjects/rough/roughB_0_0.png",
        "BattlefieldObjects/rough/roughB_0_1.png",
        "BattlefieldObjects/rough/roughB_0_2.png",
        "BattlefieldObjects/rough/roughB_0_3.png",
        "BattlefieldObjects/rough/roughB_1_0.png",
        "BattlefieldObjects/rough/roughB_1_1.png",
        "BattlefieldObjects/rough/roughB_1_2.png",
        "BattlefieldObjects/rough/roughB_1_3.png",
        "BattlefieldObjects/rough/roughB_2_0.png",
        "BattlefieldObjects/rough/roughB_2_1.png",
        "BattlefieldObjects/rough/roughB_2_2.png",
        "BattlefieldObjects/rough/roughB_2_3.png"
    };

    /// @brief List of additional object textures in snow battlefields.
    const std::vector<std::string> SNOW_BATTLEFIELD_OBJECTS_TEXTURES = {
        "BattlefieldObjects/snow/snowB_0_0.png",
        "BattlefieldObjects/snow/snowB_0_1.png",
        "BattlefieldObjects/snow/snowB_0_2.png",
        "BattlefieldObjects/snow/snowB_0_3.png",
        "BattlefieldObjects/snow/snowB_0_4.png",
        "BattlefieldObjects/snow/snowB_0_5.png",
        "BattlefieldObjects/snow/snowB_1_0.png",
        "BattlefieldObjects/snow/snowB_1_1.png",
        "BattlefieldObjects/snow/snowB_1_2.png",
        "BattlefieldObjects/snow/snowB_1_3.png",
        "BattlefieldObjects/snow/snowB_1_4.png",
        "BattlefieldObjects/snow/snowB_1_5.png",
        "BattlefieldObjects/snow/snowB_2_0.png",
        "BattlefieldObjects/snow/snowB_2_1.png",
        "BattlefieldObjects/snow/snowB_2_2.png",
        "BattlefieldObjects/snow/snowB_2_3.png",
        "BattlefieldObjects/snow/snowB_2_4.png",
        "BattlefieldObjects/snow/snowB_2_5.png",
        "BattlefieldObjects/snow/snowA_0_0.png",
        "BattlefieldObjects/snow/snowA_0_1.png",
        "BattlefieldObjects/snow/snowA_0_0.png",
        "BattlefieldObjects/snow/snowA_0_1.png",
    };

    /// @brief List of additional object textures in swamp battlefields.
    const std::vector<std::string> SWAMP_BATTLEFIELD_OBJECTS_TEXTURES = {
        "BattlefieldObjects/swamp/swampC_0_0.png",
        "BattlefieldObjects/swamp/swampC_0_1.png",
        "BattlefieldObjects/swamp/swampC_0_2.png",
        "BattlefieldObjects/swamp/swampA_0_0.png",
        "BattlefieldObjects/swamp/swampA_0_1.png",
        "BattlefieldObjects/swamp/swampA_1_0.png",
        "BattlefieldObjects/swamp/swampA_1_1.png",
        "BattlefieldObjects/swamp/swampB_0_0.png",
        "BattlefieldObjects/swamp/swampB_0_1.png",
        "BattlefieldObjects/swamp/swampB_1_0.png",
        "BattlefieldObjects/swamp/swampB_1_1.png",
        "BattlefieldObjects/swamp/swampC_0_0.png",
        "BattlefieldObjects/swamp/swampC_0_1.png",
        "BattlefieldObjects/swamp/swampC_0_2.png",
    };

    /// @brief List representing texture to draw as a borders of world map, based on terrain type.
    /// Each terrain has only one border texture.
    const std::vector<std::string> WORLD_MAP_BORDERS = {
        "Borders/GrassBorder.png",
        "Borders/RoughBorder.png",
        "Borders/SnowBorder.png",
        "Borders/SwampBorder.png"
    };

    const int N_BATTLEFIELD_STYLES = 4;
    const std::array<TileType, N_BATTLEFIELD_STYLES> BATTLEFIELD_STYLES = {GRASS_TILE, ROUGH_TILE, SNOW_TILE, SWAMP_TILE};

    /**
     * @brief List of additional object textures lists in battlefields.
     *
     * Whenever new Battlefield is created, there are several object which are walls. In battlefield maps, all those objects are marked as 'X'.
     * Every vector in this list represents one set of textures, which describes which texture use to draw as i-th object on the battlefield map.
     *
     * @note Map is always created from left to right and then from top to bottom. For example, the first texture on the list
     * will be shown at the most upper-left 'X' tile.
     *
     */
    const std::vector<std::vector<std::string>> ADDITIONAL_BATTLEFIELD_OBJECTS_TEXTURES =
        {GRASS_BATTLEFIELD_OBJECTS_TEXTURES, ROUGH_BATTLEFIELD_OBJECTS_TEXTURES, SNOW_BATTLEFIELD_OBJECTS_TEXTURES, SWAMP_BATTLEFIELD_OBJECTS_TEXTURES};


    //TOWNS

    //HALL

    /// @brief Name of hall building
    const std::string HALL_NAME = "Kapitol";

    /// @brief Gold provided by hall at the beginning of each day.
    const std::array<int, constants::MAX_LEVEL_OF_HALL + 1> GOLD_PROVIDED_BY_HALL = {500, 1000, 2000, 4000};

    /// @brief Vector of upgrades costs of hall. Each element describes cost of upgrading hall to i+1th level.
    const std::array<ResourceArray, constants::MAX_LEVEL_OF_HALL> COST_OF_HALL_UPGRADE = {
        generateResourceArray({{GOLD, 2500}}),
        generateResourceArray({{GOLD, 5000}}),
        generateResourceArray({{GOLD, 10000}})
    };

    /// @brief Additional building requirements for hall to be upgraded. Each element of array corresponds upgrading hall to level i+1.
    /// Each elements of vector of single level requirement is a vector of pairs, of which on each pair first element is building and second element is
    /// required minimum level of this building. Hall cannot be upgraded if all criteria on given level are meet.
    const std::vector<std::pair<BuildingType, int>> HALL_REQUIREMENTS[constants::MAX_LEVEL_OF_HALL] = {
        {{TAVERN, 1}},
        {{MAGE_GUILD, 1}},
        {{FORTIFICATIONS, 3}}
    };

    //FORTIFICATIONS

    /// @brief Name of fortifications building at level 0.
    const std::string FORTIFICATIONS_NAME_LEVEL_0 = "Fortyfikacje (Niewybudowane)";

    /// @brief Name of fortifications building
    const std::string FORTIFICATIONS_NAME = "Fortyfikacje";

    /// @brief Vector of upgrades costs of fortifications. Each element describes cost of upgrading fortifications to i+1th level.
    const std::array<ResourceArray, constants::MAX_LEVEL_OF_FORTIFICATIONS> COST_OF_FORTIFICATIONS_UPGRADE = {
        generateResourceArray({{GOLD, 5000}}),
        generateResourceArray({{GOLD, 5000}, {ORE, 5}}),
        generateResourceArray({{GOLD, 5000}, {WOOD, 10}, {ORE, 10}})
    };

    /// @brief Percentage bonus of growth of troops in town, based on level of fortifications.
    const std::array<int, constants::MAX_LEVEL_OF_FORTIFICATIONS+1> CREATURE_GROWTH_MULTIPLIERS =  {100, 100, 150, 200};

    /// @brief Town army expected strength, based on level of fortifications.
    const std::array<int, constants::MAX_LEVEL_OF_FORTIFICATIONS+1> TOWN_ARMY_LEVELS = {3, 4, 5, 7};

    /// @brief Neutral town's army strength.
    const int NEUTRAL_TOWN_ARMY_LEVEL = 2;

    //DWELLINGS

    /// @brief Name of tavern building at level 0.
    const std::string DWELLINGS_NAME_LEVEL_0 = "Koszary (Niewybudowane)";

    /// @brief Name of tavern building
    const std::string DWELLINGS_NAME = "Koszary";


    //TAVERN

    /// @brief Name of tavern building at level 0.
    const std::string TAVERN_NAME_LEVEL_0 = "Karczma (Niewybudowana)";

    /// @brief Name of tavern building
    const std::string TAVERN_NAME = "Karczma";

    const std::array<ResourceArray, constants::MAX_LEVEL_OF_TAVERN> COST_OF_TAVERN_UPGRADE = {generateResourceArray({{GOLD, 500}, {WOOD, 5}})};

    /// @brief Maximum number of heroes available to buy in town.
    const int MAX_NUMER_OF_HEROES_IN_TAVERN = 3;

    const ResourceArray COST_OF_BUYING_HERO = generateResourceArray({{GOLD, 500}});

    /// Array representing situation, when no heroes are available to buy in tavern. It is used when creating basic tavern,
    /// and it is also used up until player upgrade tavern.
    const std::array<HeroType, MAX_NUMER_OF_HEROES_IN_TAVERN> NO_HEROES_IN_TAVERN = {INVALID_HERO, INVALID_HERO, INVALID_HERO};

    //MAGE GUILD

    /// @brief Name of mage guild building
    const std::string MAGE_GUILD_NAME = "Gildia Magów";

    /// @brief Vector of upgrades costs of mage guild. Each element describes cost of upgrading mage guild to i+1th level.
    const std::array<ResourceArray, constants::MAX_LEVEL_OF_MAGE_GUILD> COST_OF_MAGE_GUILD_UPGRADE = {
        generateResourceArray({{GOLD, 2000},{WOOD, 5},{ORE, 5}}),
        generateResourceArray({{GOLD, 1000},{WOOD, 5},{ORE, 5},{CRYSTAL, 6},  {JEWEL, 6}}),
        generateResourceArray({{GOLD, 1000},{WOOD, 5},{ORE, 5},{CRYSTAL, 10}, {JEWEL, 10}})
    };

    /// @brief Power and number of spells added whenever mage guild is levelled up.
    /// Each element is a pair, in which elements describes what is the level of new spells, and second describes number of newly added spells.
    const std::array<std::pair<int, int>,constants::MAX_LEVEL_OF_MAGE_GUILD> MAGE_GUILD_SPELLS_ADDED_ON_LEVEL_UP = {{{1, 4}, {2, 3}, {3, 2}}};

    /// @brief Maximum number of spells to buy in town.
    const int MAX_NUMER_OF_SPELLS_IN_TOWN = 9;

    const ResourceArray COST_OF_BUYING_SPELL = generateResourceArray({{GOLD, 250}});

    //CASTLE DATA

    /// @brief Troops types available to buy in castle town and troop types of which castle town's army consists of.
    const std::vector<TroopType> CASTLE_TROOPS = {PIKEMAN, ARCHER, GRIFFIN, SWORDSMAN, MONK, CAVALIER, ANGEL};

    /// @brief Probabilities, of which hero can be added to castle town's tavern to be available to buy by player.
    /// Each number's index corresponds to hero with given ID.
    const std::array<int, N_HERO_TYPES> CASTLE_TAVERN_PROBABILITIES = {4,4,4,4,1,1,1,1,1,1,1,1};

    /// @brief Probabilities, of which spell can be added to castle town's mage guild to be available to buy by player.
    /// Each number's index corresponds to spell with given ID.
    const std::array<int, N_SPELL_TYPES> CASTLE_MAGE_GUILD_PROBABILITIES = {4,4,4,4,3,3,3,3,1,1,1,1};


    /// @brief Vector of upgrades costs of castle's dwellings. Each element describes cost of upgrading dwellings to i+1th level.
    const std::array<ResourceArray, constants::MAX_LEVEL_OF_DWELLINGS> CASTLE_COST_OF_UPGRADING_DWELLING = {
        generateResourceArray({{GOLD, 500},     {ORE, 10},                                                   }),
        generateResourceArray({{GOLD, 1000},    {ORE, 5},       {WOOD, 5}                                    }),
        generateResourceArray({{GOLD, 1000},    {ORE, 5},                                                    }),
        generateResourceArray({{GOLD, 2000},    {ORE, 5},                                                    }),
        generateResourceArray({{GOLD, 3000},    {ORE, 5},       {WOOD, 5},      {JEWEL, 2},     {CRYSTAL, 2} }),
        generateResourceArray({{GOLD, 5000},                    {WOOD, 20}                                   }),
        generateResourceArray({{GOLD, 20000},                                   {JEWEL, 10},    {CRYSTAL, 10}}),
    };

    /// @brief Cost of buying castle town's troops. Each element describes costs of buying troop at i-th position of CASTLE_TROOPS.
    const std::array<ResourceArray, constants::MAX_LEVEL_OF_DWELLINGS> CASTLE_COST_OF_BUYING_TROOP = {
        generateResourceArray({{GOLD, 0},               }),
        generateResourceArray({{GOLD, 100},             }),
        generateResourceArray({{GOLD, 200},             }),
        generateResourceArray({{GOLD, 300},             }),
        generateResourceArray({{GOLD, 400},             }),
        generateResourceArray({{GOLD, 1000},            }),
        generateResourceArray({{GOLD, 3000}, {JEWEL, 1} }),
    };

    const std::vector<std::string> CASTLE_HALL_TEXTURES =           generateIncrementingStrings("Towns/castle/CastleHall", constants::MAX_LEVEL_OF_HALL, ".png");
    const std::vector<std::string> CASTLE_FORTIFICATIONS_TEXTURES = generateIncrementingStrings("Towns/castle/CastleFortifications", constants::MAX_LEVEL_OF_FORTIFICATIONS, ".png");
    const std::vector<std::string> CASTLE_TAVERN_TEXTURES =         generateIncrementingStrings("Towns/castle/CastleTavern", constants::MAX_LEVEL_OF_TAVERN, ".png");
    const std::vector<std::string> CASTLE_MAGE_GUILD_TEXTURES =     generateIncrementingStrings("Towns/castle/CastleMageGuild", constants::MAX_LEVEL_OF_MAGE_GUILD, ".png");
    const std::vector<std::string> CASTLE_DWELLINGS_TEXTURES =      generateIncrementingStrings("Towns/castle/CastleDwellings", constants::MAX_LEVEL_OF_DWELLINGS, ".gif");

    /// @brief Set of textures of castles visible from the world map.
    /// Each element is a vector, which represents castle in different stages of game, based on castle's hall level.
    const std::vector<std::vector<std::string>> CASTLE_TEXTURES = {
        generateIncrementingStrings("Towns/castle/Castle", constants::MAX_LEVEL_OF_HALL, "_0.png"),
        generateIncrementingStrings("Towns/castle/Castle", constants::MAX_LEVEL_OF_HALL, "_1.png"),
        generateIncrementingStrings("Towns/castle/Castle", constants::MAX_LEVEL_OF_HALL, "_2.png"),
        generateIncrementingStrings("Towns/castle/Castle", constants::MAX_LEVEL_OF_HALL, "_3.png"),

    };

    /// @brief Set of various file path to icons used in castle GUI. Each element is a vector, in which elements shows buildings in different levels.
    const std::array<std::vector<std::string>, N_BUILDING_TYPES> CASTLE_BUILDING_TEXTURES = {CASTLE_HALL_TEXTURES, CASTLE_FORTIFICATIONS_TEXTURES, CASTLE_TAVERN_TEXTURES, CASTLE_MAGE_GUILD_TEXTURES, CASTLE_DWELLINGS_TEXTURES};

    /// @brief TownData struct describing all data and initial resources of castle.
    const TownData CASTLE_DATA = {
        .town_troops = CASTLE_TROOPS,
        .hero_probabilities = CASTLE_TAVERN_PROBABILITIES,
        .spell_probabilities = CASTLE_MAGE_GUILD_PROBABILITIES,
        .cost_of_upgrading_dwelling = CASTLE_COST_OF_UPGRADING_DWELLING,
        .cost_of_buying_troop = CASTLE_COST_OF_BUYING_TROOP,
        .town_building_textures = CASTLE_BUILDING_TEXTURES,
        .town_textures = CASTLE_TEXTURES
    };

    /////////
    //RAMPART DATA

    /// @brief Troops types available to buy in rampart town and troop types of which rampart town's army consists of.
    const std::vector<TroopType> RAMPART_TROOPS = {CENTAUR, DWARF, WOOD_ELF, PEGASUS, DENDROID_GUARD, UNICORN, GREEN_DRAGON};

    /// @brief Probabilities, of which hero can be added to rampart town's tavern to be available to buy by player.
    /// Each number's index corresponds to hero with given ID.
    const std::array<int, N_HERO_TYPES> RAMPART_TAVERN_PROBABILITIES = {1,1,1,1,4,4,4,4,1,1,1,1};

    /// @brief Probabilities, of which spell can be added to rampart town's mage guild to be available to buy by player.
    /// Each number's index corresponds to spell with given ID.
    const std::array<int, N_SPELL_TYPES> RAMPART_MAGE_GUILD_PROBABILITIES = {3,3,3,3,3,3,3,3,2,2,2,2};

    /// @brief Vector of upgrades costs of rampart's dwellings. Each element describes cost of upgrading dwellings to i+1th level.
    const std::array<ResourceArray, constants::MAX_LEVEL_OF_DWELLINGS> RAMPART_COST_OF_UPGRADING_DWELLING = {
        generateResourceArray({{GOLD, 500},     {WOOD, 5},                                                   }),
        generateResourceArray({{GOLD, 1000},    {WOOD, 5},                                          }),
        generateResourceArray({{GOLD, 1500},    {WOOD, 10},                                                    }),
        generateResourceArray({{GOLD, 2000},    {CRYSTAL, 10},                                                    }),
        generateResourceArray({{GOLD, 2500},                                                        }),
        generateResourceArray({{GOLD, 4000},    {ORE, 5}, {WOOD, 5}, {JEWEL, 10},     }),
        generateResourceArray({{GOLD, 10000},   {ORE, 30}, {CRYSTAL, 20}}),
    };

    /// @brief Cost of buying rampart town's troops. Each element describes costs of buying troop at i-th position of RAMPART_TROOPS.
    const std::array<ResourceArray, constants::MAX_LEVEL_OF_DWELLINGS> RAMPART_COST_OF_BUYING_TROOP = {
        generateResourceArray({{GOLD, 90},                  }),
        generateResourceArray({{GOLD, 150},                 }),
        generateResourceArray({{GOLD, 225},                 }),
        generateResourceArray({{GOLD, 275},                 }),
        generateResourceArray({{GOLD, 425},                 }),
        generateResourceArray({{GOLD, 950},                 }),
        generateResourceArray({{GOLD, 4000}, {CRYSTAL, 2}   }),
    };

    const std::vector<std::string> RAMPART_HALL_TEXTURES =           generateIncrementingStrings("Towns/rampart/RampartHall", constants::MAX_LEVEL_OF_HALL, ".png");
    const std::vector<std::string> RAMPART_FORTIFICATIONS_TEXTURES = generateIncrementingStrings("Towns/rampart/RampartFortifications", constants::MAX_LEVEL_OF_FORTIFICATIONS, ".png");
    const std::vector<std::string> RAMPART_TAVERN_TEXTURES =         generateIncrementingStrings("Towns/rampart/RampartTavern", constants::MAX_LEVEL_OF_TAVERN, ".png");
    const std::vector<std::string> RAMPART_MAGE_GUILD_TEXTURES =     generateIncrementingStrings("Towns/rampart/RampartMageGuild", constants::MAX_LEVEL_OF_MAGE_GUILD, ".png");
    const std::vector<std::string> RAMPART_DWELLINGS_TEXTURES =      generateIncrementingStrings("Towns/rampart/RampartDwellings", constants::MAX_LEVEL_OF_DWELLINGS, ".gif");

    /// @brief Set of textures of rampart visible from the world map.
    /// Each element is a vector, which represents rampart in different stages of game, based on rampart's hall level.
    const std::vector<std::vector<std::string>> RAMPART_TEXTURES = {
        generateIncrementingStrings("Towns/rampart/Rampart", constants::MAX_LEVEL_OF_HALL, "_0.png"),
        generateIncrementingStrings("Towns/rampart/Rampart", constants::MAX_LEVEL_OF_HALL, "_1.png"),
        generateIncrementingStrings("Towns/rampart/Rampart", constants::MAX_LEVEL_OF_HALL, "_2.png"),
        generateIncrementingStrings("Towns/rampart/Rampart", constants::MAX_LEVEL_OF_HALL, "_3.png"),

    };

    /// @brief Set of various file path to icons used in rampart GUI. Each element is a vector, in which elements shows buildings in different levels.
    const std::array<std::vector<std::string>, N_BUILDING_TYPES> RAMPART_BUILDING_TEXTURES = {RAMPART_HALL_TEXTURES, RAMPART_FORTIFICATIONS_TEXTURES, RAMPART_TAVERN_TEXTURES, RAMPART_MAGE_GUILD_TEXTURES, RAMPART_DWELLINGS_TEXTURES};

    /// @brief TownData struct describing all data and initial resources of rampart.
    const TownData RAMPART_DATA = {
        .town_troops = RAMPART_TROOPS,
        .hero_probabilities = RAMPART_TAVERN_PROBABILITIES,
        .spell_probabilities = RAMPART_MAGE_GUILD_PROBABILITIES,
        .cost_of_upgrading_dwelling = RAMPART_COST_OF_UPGRADING_DWELLING,
        .cost_of_buying_troop = RAMPART_COST_OF_BUYING_TROOP,
        .town_building_textures = RAMPART_BUILDING_TEXTURES,
        .town_textures = RAMPART_TEXTURES

    };

    /////////
    //DUNGEON DATA

    /// @brief Troops types available to buy in dungeon town and troop types of which dungeon town's army consists of.
    const std::vector<TroopType> DUNGEON_TROOPS = {TROGLODYTE, HARPY, BEHOLDER, MEDUSA, MINOTAUR, MANTICORE, RED_DRAGON};

    /// @brief Probabilities, of which hero can be added to dungeon town's tavern to be available to buy by player.
    /// Each number's index corresponds to hero with given ID.
    const std::array<int, N_HERO_TYPES> DUNGEON_TAVERN_PROBABILITIES = {1,1,1,1,1,1,1,1,4,4,4,4};

    /// @brief Probabilities, of which spell can be added to dungeon town's mage guild to be available to buy by player.
    /// Each number's index corresponds to spell with given ID.
    const std::array<int, N_SPELL_TYPES> DUNGEON_MAGE_GUILD_PROBABILITIES = {2,2,2,2,1,1,1,1,4,4,4,4};

    /// @brief Vector of upgrades costs of dungeon's dwellings. Each element describes cost of upgrading dwellings to i+1th level.
    const std::array<ResourceArray, constants::MAX_LEVEL_OF_DWELLINGS> DUNGEON_COST_OF_UPGRADING_DWELLING = {
        generateResourceArray({{GOLD, 400},     {WOOD, 10},                                                   }),
        generateResourceArray({{GOLD, 1000},                                       }),
        generateResourceArray({{GOLD, 1000},    {WOOD, 1}, {ORE, 1}, {JEWEL, 1}, {CRYSTAL, 1},                                                 }),
        generateResourceArray({{GOLD, 2000},    {ORE, 10}, {WOOD, 5}                                                   }),
        generateResourceArray({{GOLD, 4000},    {ORE, 10},       {JEWEL, 10}, }),
        generateResourceArray({{GOLD, 5000},                    {WOOD, 5}, {ORE, 5}      }),
        generateResourceArray({{GOLD, 15000},     {WOOD, 15},    {ORE, 15}    }),
    };

    /// @brief Cost of buying dungeon town's troops. Each element describes costs of buying troop at i-th position of DUNGEON_TROOPS.
    const std::array<ResourceArray, constants::MAX_LEVEL_OF_DWELLINGS> DUNGEON_COST_OF_BUYING_TROOP = {
        generateResourceArray({{GOLD, 65},               }),
        generateResourceArray({{GOLD, 170},             }),
        generateResourceArray({{GOLD, 280},             }),
        generateResourceArray({{GOLD, 330},             }),
        generateResourceArray({{GOLD, 575},             }),
        generateResourceArray({{GOLD, 1050},            }),
        generateResourceArray({{GOLD, 4000}, {CRYSTAL, 2} }),
    };

    const std::vector<std::string> DUNGEON_HALL_TEXTURES =           generateIncrementingStrings("Towns/dungeon/DungeonHall", constants::MAX_LEVEL_OF_HALL, ".png");
    const std::vector<std::string> DUNGEON_FORTIFICATIONS_TEXTURES = generateIncrementingStrings("Towns/dungeon/DungeonFortifications", constants::MAX_LEVEL_OF_FORTIFICATIONS, ".png");
    const std::vector<std::string> DUNGEON_TAVERN_TEXTURES =         generateIncrementingStrings("Towns/dungeon/DungeonTavern", constants::MAX_LEVEL_OF_TAVERN, ".png");
    const std::vector<std::string> DUNGEON_MAGE_GUILD_TEXTURES =     generateIncrementingStrings("Towns/dungeon/DungeonMageGuild", constants::MAX_LEVEL_OF_MAGE_GUILD, ".png");
    const std::vector<std::string> DUNGEON_DWELLINGS_TEXTURES =      generateIncrementingStrings("Towns/dungeon/DungeonDwellings", constants::MAX_LEVEL_OF_DWELLINGS, ".gif");

    /// @brief Set of textures of dungeon visible from the world map.
    /// Each element is a vector, which represents dungeon in different stages of game, based on dungeon's hall level.
    const std::vector<std::vector<std::string>> DUNGEON_TEXTURES = {
        generateIncrementingStrings("Towns/dungeon/Dungeon", constants::MAX_LEVEL_OF_HALL, "_0.png"),
        generateIncrementingStrings("Towns/dungeon/Dungeon", constants::MAX_LEVEL_OF_HALL, "_1.png"),
        generateIncrementingStrings("Towns/dungeon/Dungeon", constants::MAX_LEVEL_OF_HALL, "_2.png"),
        generateIncrementingStrings("Towns/dungeon/Dungeon", constants::MAX_LEVEL_OF_HALL, "_3.png"),

    };

    /// @brief Set of various file path to icons used in dungeon GUI. Each element is a vector, in which elements shows buildings in different levels.
    const std::array<std::vector<std::string>, N_BUILDING_TYPES> DUNGEON_BUILDING_TEXTURES = {DUNGEON_HALL_TEXTURES, DUNGEON_FORTIFICATIONS_TEXTURES, DUNGEON_TAVERN_TEXTURES, DUNGEON_MAGE_GUILD_TEXTURES, DUNGEON_DWELLINGS_TEXTURES};

    /// @brief TownData struct describing all data and initial resources of dungeon.
    const TownData DUNGEON_DATA = {
        .town_troops = DUNGEON_TROOPS,
        .hero_probabilities = DUNGEON_TAVERN_PROBABILITIES,
        .spell_probabilities = DUNGEON_MAGE_GUILD_PROBABILITIES,
        .cost_of_upgrading_dwelling = DUNGEON_COST_OF_UPGRADING_DWELLING,
        .cost_of_buying_troop = DUNGEON_COST_OF_BUYING_TROOP,
        .town_building_textures = DUNGEON_BUILDING_TEXTURES,
        .town_textures = DUNGEON_TEXTURES
    };

    /////////

    /// @brief Array of TownData structs, where each elements corresponds to different town type's data.
    const std::array<TownData, N_TOWN_TYPES> TOWN_DATA = {CASTLE_DATA, RAMPART_DATA, DUNGEON_DATA};


    /**
     * @brief Retrieve the data associated with a specific town type.
     *
     * This function returns the TownData object corresponding to the provided TownType.
     *
     * @param town_type The type of the town for which data is requested.
     * @return TownData The data structure containing information about the specified town type.
     */
    inline TownData getTownData(TownType town_type){
        if(town_type < 0 || town_type >= N_TOWN_TYPES){
            throw InvalidIDError("TownType", town_type);
        }
        return TOWN_DATA.at(town_type);
    }

    /// @brief Vector of troop family types.
    const std::vector<TownType> TROOP_FAMILY_TYPES = {CASTLE, RAMPART, DUNGEON};

    /// @brief Vector of troop lists, which belong to same family.
    const std::vector<std::vector<TroopType>> TROOP_TYPES = {CASTLE_TROOPS, RAMPART_TROOPS, DUNGEON_TROOPS};



    //////////

    //TROOP DATA

    /**
     * @brief Array containing data for all troop types in the game.
     *
     * Each element of the array represents a specific troop type, including its battle statistics,
     * display name, and associated texture. The array is indexed by troop type.
     *
     */
    const std::array<TroopData, N_TROOP_TYPES> TROOP_DATA = {{
        {
            .troop_battle_data = {
                .level = 1,
                .attack = 4,
                .defense = 5,
                .speed = 4,
                .range = 1,
                .min_damage = 1,
                .max_damage = 3,
                .base_health = 10,
                .troop_power = 80,
                .growth = 14,
            },
            .troop_name = "Pikinier",
            .texture_name = "Troops/Pikeman.png"
        },
        {
            .troop_battle_data = {
                .level = 1,
                .attack = 5,
                .defense = 3,
                .speed = 6,
                .range = 1,
                .min_damage = 2,
                .max_damage = 3,
                .base_health = 8,
                .troop_power = 100,
                .growth = 14
            },
            .troop_name = "Centaur",
            .texture_name = "Troops/Centaur.png"
        },
        {
            .troop_battle_data = {
                .level = 1,
                .attack = 4,
                .defense = 3,
                .speed = 4,
                .range = 1,
                .min_damage = 1,
                .max_damage = 3,
                .base_health = 5,
                .troop_power = 59,
                .growth = 14
            },

            .troop_name = "Troglodyta",
            .texture_name = "Troops/Troglyte.png"
        },
        {
            .troop_battle_data = {
                .level = 2,
                .attack = 6,
                .defense = 3,
                .speed = 4,
                .range = 6,
                .min_damage = 2,
                .max_damage = 3,
                .base_health = 10,
                .troop_power = 126,
                .growth = 9,
            },

            .troop_name = "Łucznik",
            .texture_name = "Troops/Archer.png"
        },
        {
            .troop_battle_data = {
                .level = 2,
                .attack = 6,
                .defense = 7,
                .speed = 3,
                .range = 1,
                .min_damage = 2,
                .max_damage = 4,
                .base_health = 20,
                .troop_power = 138,
                .growth = 8
            },
            .troop_name = "Krasnolud",
            .texture_name = "Troops/Dwarf.png"
        },
        {
            .troop_battle_data = {
                .level = 2,
                .attack = 6,
                .defense = 5,
                .speed = 6,
                .range = 1,
                .min_damage = 1,
                .max_damage = 4,
                .base_health = 14,
                .troop_power = 154,
                .growth = 8
            },
            .troop_name = "Harpia",
            .texture_name = "Troops/Harpy.png"
        },
        {
            .troop_battle_data = {
                .level = 3,
                .attack = 8,
                .defense = 8,
                .speed = 6,
                .range = 12,
                .min_damage = 3,
                .max_damage = 6,
                .base_health = 25,
                .troop_power = 351,
                .growth = 7
            },

            .troop_name = "Gryf",
            .texture_name = "Troops/Griffin.png"
        },
        {
            .troop_battle_data = {
                .level = 3,
                .attack = 9,
                .defense = 5,
                .speed = 6,
                .range = 12,
                .min_damage = 3,
                .max_damage = 5,
                .base_health = 15,
                .troop_power = 234,
                .growth = 7
            },

            .troop_name = "Elf",
            .texture_name = "Troops/WoodElf.png"
        },
        {
            .troop_battle_data = {
                .level = 3,
                .attack = 9,
                .defense = 7,
                .speed = 5,
                .range = 6,
                .min_damage = 3,
                .max_damage = 5,
                .base_health = 22,
                .troop_power = 336,
                .growth = 7
            },

            .troop_name = "Złe oko",
            .texture_name = "Troops/Beholder.png"
        },
        {
            .troop_battle_data = {
                .level = 4,
                .attack = 10,
                .defense = 12,
                .speed = 5,
                .range = 1,
                .min_damage = 6,
                .max_damage = 9,
                .base_health = 35,
                .troop_power = 445,
                .growth = 4
            },

            .troop_name = "Zbrojny",
            .texture_name = "Troops/Swordsman.png"
        },
        {
            .troop_battle_data = {
                .level = 4,
                .attack = 9,
                .defense = 8,
                .speed = 8,
                .range = 1,
                .min_damage = 5,
                .max_damage = 9,
                .base_health = 30,
                .troop_power = 518,
                .growth = 5
            },

            .troop_name = "Pegaz",
            .texture_name = "Troops/Pegasus.png"
        },
        {
            .troop_battle_data = {
                .level = 4,
                .attack = 9,
                .defense = 9,
                .speed = 5,
                .range = 2,
                .min_damage = 6,
                .max_damage = 8,
                .base_health = 25,
                .troop_power = 517,
                .growth = 4
            },

            .troop_name = "Meduza",
            .texture_name = "Troops/Medusa.png"
        },
        {
            .troop_battle_data = {
                .level = 5,
                .attack = 6,
                .defense = 3,
                .speed = 4,
                .range = 12,
                .min_damage = 2,
                .max_damage = 3,
                .base_health = 10,
                .troop_power = 126,
                .growth = 9
            },

            .troop_name = "Mnich",
            .texture_name = "Troops/Monk.png"
        },
        {
            .troop_battle_data = {
                .level = 5,
                .attack = 9,
                .defense = 12,
                .speed = 3,
                .range = 1,
                .min_damage = 10,
                .max_damage = 14,
                .base_health = 55,
                .troop_power = 517,
                .growth = 3
            },

            .troop_name = "Drzewiec",
            .texture_name = "Troops/DendroidGuard.png"
        },
        {
            .troop_battle_data = {
                .level = 5,
                .attack = 14,
                .defense = 12,
                .speed = 6,
                .range = 1,
                .min_damage = 12,
                .max_damage = 20,
                .base_health = 50,
                .troop_power = 835,
                .growth = 3
            },
            .troop_name = "Minotaur",
            .texture_name = "Troops/Minotaur.png"
        },
        {
            .troop_battle_data = {
                .level = 6,
                .attack = 15,
                .defense = 15,
                .speed = 7,
                .range = 1,
                .min_damage = 15,
                .max_damage = 25,
                .base_health = 100,
                .troop_power = 1946,
                .growth = 2
            },
            .troop_name = "Kawalerzysta",
            .texture_name = "Troops/Cavalier.png"
        },
        {
            .troop_battle_data = {
                .level = 6,
                .attack = 15,
                .defense = 14,
                .speed = 7,
                .range = 1,
                .min_damage = 18,
                .max_damage = 22,
                .base_health = 90,
                .troop_power = 1806,
                .growth = 2
            },
            .troop_name = "Jednorożec",
            .texture_name = "Troops/Unicorn.png"
        },
        {
            .troop_battle_data = {
                .level = 6,
                .attack = 15,
                .defense = 13,
                .speed = 7,
                .range = 1,
                .min_damage = 14,
                .max_damage = 20,
                .base_health = 80,
                .troop_power = 1547,
                .growth = 2
            },
            .troop_name = "Mantykora",
            .texture_name = "Troops/Manticore.png"
        },
        {
            .troop_battle_data = {
                .level = 7,
                .attack = 20,
                .defense = 20,
                .speed = 12,
                .range = 1,
                .min_damage = 50,
                .max_damage = 50,
                .base_health = 200,
                .troop_power = 5019,
                .growth = 1
            },

            .troop_name = "Anioł",
            .texture_name = "Troops/Angel.png"
        },
        {
            .troop_battle_data = {
                .level = 7,
                .attack = 18,
                .defense = 18,
                .speed = 10,
                .range = 1,
                .min_damage = 40,
                .max_damage = 50,
                .base_health = 180,
                .troop_power = 4872,
                .growth = 1
            },

            .troop_name = "Zielony smok",
            .texture_name = "Troops/GreenDragon.png"
        },
        {
            .troop_battle_data = {
                .level = 7,
                .attack = 19,
                .defense = 19,
                .speed = 11,
                .range = 1,
                .min_damage = 40,
                .max_damage = 50,
                .base_health = 180,
                .troop_power = 4702,
                .growth = 1
            },
            .troop_name = "Czerwony smok",
            .texture_name = "Troops/RedDragon.png"
        }
    }};

    /**
     * @brief Retrieve the data associated with a specific troop.
     *
     * This function returns the TroopData object corresponding to the provided TroopType.
     *
     * @param town_type The type of the troop for which data is requested.
     * @return TroopData The data structure containing information about the specified troop type.
     */
    inline TroopData getTroopData(TroopType troop_type){
        if(troop_type < 0 || troop_type >= N_TROOP_TYPES){
            throw InvalidIDError("TroopType", troop_type);
        }
        return TROOP_DATA.at(troop_type);
    }

    //////////
    //SPELL DATA


    /**
     * @brief Array containing data for all available spells in the game.
     *
     * Each element of the array describes a single spell, including its
     * battle effects, name, family type, description, and icon.
     * The array is indexed by spell type.
     */
    const std::array<SpellData, N_SPELL_TYPES> SPELL_DATA = {{
        {
            .spell_battle_data = {
                .stats_added = {6,8,12},
                .cost = {5,4,4},
                .troop_stats_to_add = {TROOP_DEFENSE}
            },
            .spell_name = "Kamienna Skóra",
            .spell_family = SINGLE_TARGET_BUFF_SPELL,
            .spell_description = "Daj przyjaznej jednostce +6/8/12 do Obrony. Koszt: 5/4/4.",
            .texture_name = "Spells/StoneSkin.png"
        },
        {
            .spell_battle_data = {
                .stats_added = {4,6,8},
                .cost = {14,12,10},
                .troop_stats_to_add = {TROOP_ATTACK, TROOP_DEFENSE}
            },
            .spell_name = "Modlitwa",
            .spell_family = SINGLE_TARGET_BUFF_SPELL,
            .spell_description = "Daj przyjaznej jednostce +4/6/8 do Ataku i Obrony. Koszt: 14/12/10.",
            .texture_name = "Spells/Prayer.png",
        },
        {
            .spell_battle_data = {
                .stats_added = {6,8,12},
                .cost = {5,4,4},
                .troop_stats_to_add = {TROOP_ATTACK}
            },
            .spell_name = "Żądza Krwi",
            .spell_family = SINGLE_TARGET_BUFF_SPELL,
            .spell_description = "Daj przyjaznej jednostce +6/8/12 do Ataku. Koszt: 5/4/4.",
            .texture_name = "Spells/Bloodlust.png"
        },
        {
            .spell_battle_data = {
                .radius = {3,4,5},
                .stats_added = {3,4,6},
                .cost = {5,4,4},
                .troop_stats_to_add = {TROOP_DEFENSE}
            },
            .spell_name = "Obszarowa Kamienna Skóra",
            .spell_family = AREA_OF_EFFECT_BUFF_SPELL,
            .spell_description = "Daj przyjaznym jednostkom na obszarze o promieniu 3/4/5 +3/4/6 do Obrony. Koszt: 5/4/4.",
            .texture_name = "Spells/CollectiveStoneSkin.png"
        },
        {
            .spell_battle_data = {
                .radius = {3,4,5},
                .stats_added = {2,3,4},
                .cost = {14, 12, 10},
                .troop_stats_to_add = {TROOP_ATTACK, TROOP_DEFENSE}
            },
            .spell_name = "Obszarowa Modlitwa",
            .spell_family = AREA_OF_EFFECT_BUFF_SPELL,
            .spell_description = "Daj przyjaznym jednostkom na obszarze o promieniu 3/4/5 +2/3/4 do Ataku i Obrony. Koszt: 14/12/10.",
            .texture_name = "Spells/CollectivePrayer.png"
        },
        {
            .spell_battle_data = {
                .radius = {3,4,5},
                .stats_added = {3,4,6},
                .cost = {5,4,4},
                .troop_stats_to_add = {TROOP_ATTACK}
            },
            .spell_name = "Obszarowa Żądza Krwi",
            .spell_family = AREA_OF_EFFECT_BUFF_SPELL,
            .spell_description = "Daj przyjaznym jednostkom na obszarze \no promieniu 3/4/5 +3/4/6 do Ataku. Koszt: 5/4/4.",
            .texture_name = "Spells/CollectiveBloodlust.png",
        },
        {
            .spell_battle_data = {
                .damage_per_spell_power_point = {10,10,10},
                .base_damage = {10,20,30},
                .cost = {5,4,4}
            },
            .spell_name = "Magiczna Strzała",
            .spell_family = SINGLE_TARGET_DAMAGE_SPELL,
            .spell_description = "Zadaj 10/20/30 + (10 * Moc Zaklęć) punktów obrażeń wrogiej jednostce. Koszt: 5/4/4.",
            .texture_name = "Spells/MagicArrow.png"
        },
        {
            .spell_battle_data = {
                .damage_per_spell_power_point = {25,25,25},
                .base_damage = {10,20,50},
                .cost = {10,9,8}
            },
            .spell_name = "Piorun",
            .spell_family = SINGLE_TARGET_DAMAGE_SPELL,
            .spell_description = "Zadaj 10/20/50 + (25 * Moc Zaklęć) punktów obrażeń wrogiej jednostce. Koszt: 10/9/8.",
            .texture_name = "Spells/LightningBolt.png",
        },
        {
            .spell_battle_data = {
                .damage_per_spell_power_point = {40,40,40},
                .base_damage = {50,100,150},
                .cost = {20,19,17}
            },
            .spell_name = "Implozja",
            .spell_family = SINGLE_TARGET_DAMAGE_SPELL,
            .spell_description = "Zadaj 50/100/150 + (40 * Moc Zaklęć) punktów obrażeń wrogiej jednostce. Koszt: 20/19/17.",
            .texture_name = "Spells/Implosion.png"
        },
        {
            .spell_battle_data = {
                .damage_per_spell_power_point = {5, 5, 5},
                .radius = {2,2,3},
                .base_damage = {10, 15, 30},
                .cost = {12,11,10}
            },
            .spell_name = "Kula Ognia",
            .spell_family = AREA_OF_EFFECT_DAMAGE_SPELL,
            .spell_description = "Zadaj 10/15/30 + (5 * Moc Zaklęć) punktów obrażeń wrogim jednostkom na obszarze o promieniu 2/2/3. Koszt: 12/11/10.",
            .texture_name = "Spells/Fireball.png"
        },
        {
            .spell_battle_data = {
                .damage_per_spell_power_point = {10, 10, 10},
                .radius = {3,3,3},
                .base_damage = {15, 25, 50},
                .cost = {16,14,12}
            },
            .spell_name = "Deszcz meteorów",
            .spell_family = AREA_OF_EFFECT_DAMAGE_SPELL,
            .spell_description = "Zadaj 15/25/50 + (10 * Moc Zaklęć) punktów obrażeń wrogim jednostkom na obszarze o promieniu 3. Koszt: 16/14/12.",
            .texture_name = "Spells/MeteorShower.png"
        },
        {
            .spell_battle_data = {
                .damage_per_spell_power_point = {20, 20, 20},
                .radius = {6,7,8},
                .base_damage = {10, 15, 30},
                .cost = {32, 28, 24}
            },
            .spell_name = "Armagedon",
            .spell_family = AREA_OF_EFFECT_DAMAGE_SPELL,
            .spell_description = "Zadaj 10/15/30 + (20 * Moc Zaklęć) punktów obrażeń wrogim jednostkom na obszarze o promieniu 6/7/8. Koszt: 32/28/24.",
            .texture_name = "Spells/Armageddon.png"
        }

    }};

     /**
     * @brief Retrieve the data associated with a specific spell type.
     *
     * This function returns the SpellData object corresponding to the provided SpellType.
     *
     * @param town_type The type of the spell for which data is requested.
     * @return SpellData The data structure containing information about the specified spell type.
     */
    inline SpellData getSpellData(SpellType spell_type){
        if(spell_type < 0 || spell_type >= N_SPELL_TYPES){
            throw InvalidIDError("SpellType", spell_type);
        };
        return SPELL_DATA.at(spell_type);
    };

    //////////
    //HERO DATA

    /**
     * @brief Array containing data for all hero types in the game.
     *
     * Each element of HERO_DATA represents a specific hero and contains
     * initial battle statistics, name of the hero, path to the hero's texture image, initial army and spells data,
     * probabilities for increasing each stat on level up.
     *
     * The array is indexed by hero type.
     *
     */
    const std::array<HeroData, N_HERO_TYPES> HERO_DATA = {{
        {
            .hero_battle_data = {
                .attack = 1,
                .defense = 0,
                .spell_power = 2,
                .knowledge = 2,
                .morale = 0,
                .speed = 5,
                .max_moves = 15,
                .discovery_radius = 5
            },
            .hero_name = "Loynis",
            .texture_name = "Heroes/Loynis.png",
            .init_army = {{PIKEMAN, 10}, {ARCHER, 15}, {GRIFFIN, 2}},
            .init_spells = {PRAYER},
            .level_up_stat_probabilities = {4,4,1,1}
        },

        {
            .hero_battle_data = {
                .attack = 1,
                .defense = 0,
                .spell_power = 2,
                .knowledge = 2,
                .morale = 0,
                .speed = 5,
                .max_moves = 15,
                .discovery_radius = 5
            },
            .hero_name = "Rion",
            .texture_name = "Heroes/Rion.png",
            .init_army = {{PIKEMAN, 10}, {GRIFFIN, 2}},
            .init_spells = {AREA_STONE_SKIN},
            .level_up_stat_probabilities = {4,4,1,1}
        },

        {
            .hero_battle_data = {
                .attack = 1,
                .defense = 0,
                .spell_power = 2,
                .knowledge = 2,
                .morale = 0,
                .speed = 5,
                .max_moves = 15,
                .discovery_radius = 5
            },
            .hero_name = "Sanya",
            .texture_name = "Heroes/Sanya.png",
            .init_army = {{PIKEMAN, 10}, {ARCHER, 4}, {GRIFFIN, 2}},
            .init_spells = {AREA_PRAYER},
            .level_up_stat_probabilities = {1,1,4,4}
        },

        {
            .hero_battle_data = {
                .attack = 1,
                .defense = 0,
                .spell_power = 2,
                .knowledge = 2,
                .morale = 0,
                .speed = 5,
                .max_moves = 15,
                .discovery_radius = 5
            },
            .hero_name = "Caitlin",
            .texture_name = "Heroes/Caitlin.png",
            .init_army = {{PIKEMAN, 10}, {ARCHER, 4}, {GRIFFIN, 2}},
            .init_spells = {AREA_PRAYER},
            .level_up_stat_probabilities = {1,1,4,4}
        },
        {
            .hero_battle_data = {
                .attack = 0,
                .defense = 2,
                .spell_power = 1,
                .knowledge = 2,
                .morale = 0,
                .speed = 6,
                .max_moves = 18,
                .discovery_radius = 6
            },
            .hero_name = "Malcom",
            .texture_name = "Heroes/Malcom.png",
            .init_army = {{CENTAUR, 12}, {DWARF, 3}, {WOOD_ELF, 3}},
            .init_spells = {MAGIC_ARROW},
            .level_up_stat_probabilities = {1,1,4,4}
        },

        {
            .hero_battle_data = {
                .attack = 0,
                .defense = 0,
                .spell_power = 2,
                .knowledge = 3,
                .morale = 0,
                .speed = 5,
                .max_moves = 15,
                .discovery_radius = 5
            },
            .hero_name = "Halon",
            .texture_name = "Heroes/Halon.png",
            .init_army = {{CENTAUR, 10}, {DWARF, 3}, {WOOD_ELF, 2}},
            .init_spells = {STONE_SKIN},
            .level_up_stat_probabilities = {4,4,1,1}
        },

        {
            .hero_battle_data = {

                .attack = 1,
                .defense = 1,
                .spell_power = 2,
                .knowledge = 2,
                .morale = 0,
                .speed = 5,
                .max_moves = 15,
                .discovery_radius = 5
            },
            .hero_name = "Torosar",
            .texture_name = "Heroes/Torosar.png",
            .init_army = {{CENTAUR, 10}, {WOOD_ELF, 2}},
            .init_spells = {MAGIC_ARROW},
            .level_up_stat_probabilities = {1,1,4,4}
        },
        {
            .hero_battle_data = {
                .attack = 0,
                .defense = 2,
                .spell_power = 1,
                .knowledge = 2,
                .morale = 0,
                .speed = 6,
                .max_moves = 18,
                .discovery_radius = 6
            },

            .hero_name = "Elleshar",
            .texture_name = "Heroes/Elleshar.png",
            .init_army = {{CENTAUR, 12}, {DWARF, 3}, {WOOD_ELF, 3}},
            .init_spells = {AREA_STONE_SKIN},
            .level_up_stat_probabilities = {1,1,4,4}
        },

        {
            .hero_battle_data = {
                .attack = 1,
                .defense = 0,
                .spell_power = 2,
                .knowledge = 2,
                .morale = 0,
                .speed = 6,
                .max_moves = 18,
                .discovery_radius = 6
            },
            .hero_name = "Aislinn",
            .texture_name = "Heroes/Aislinn.png",
            .init_army = {{TROGLODYTE, 20}, {HARPY, 4}, {BEHOLDER, 4}},
            .init_spells = {FIREBALL},
            .level_up_stat_probabilities = {1,1,4,4}
        },

        {
            .hero_battle_data = {
                .attack = 1,
                .defense = 1,
                .spell_power = 2,
                .knowledge = 1,
                .morale = 0,
                .speed = 5,
                .max_moves = 15,
                .discovery_radius = 5
            },
            .hero_name = "Xarfax",
            .texture_name = "Heroes/Xarfax.png",
            .init_army = {{TROGLODYTE, 25}, {HARPY, 4}, {BEHOLDER, 3}},
            .init_spells = {FIREBALL},
            .level_up_stat_probabilities = {4,4,1,1}
        },

        {
            .hero_battle_data = {
                .attack = 0,
                .defense = 0,
                .spell_power = 3,
                .knowledge = 2,
                .morale = 0,
                .speed = 5,
                .max_moves = 15,
                .discovery_radius = 5
            },
            .hero_name = "Deemer",
            .texture_name = "Heroes/Deemer.png",
            .init_army = {{TROGLODYTE, 30}, {HARPY, 3}, {BEHOLDER, 3}},
            .init_spells = {FIREBALL},
            .level_up_stat_probabilities = {1,1,4,4}
        },

        {
            .hero_battle_data = {
                .attack = 0,
                .defense = 0,
                .spell_power = 3,
                .knowledge = 2,
                .morale = 0,
                .speed = 5,
                .max_moves = 15,
                .discovery_radius = 5
            },
            .hero_name = "Malekith",
            .texture_name = "Heroes/Malekith.png",
            .init_army = {{TROGLODYTE, 30}, {HARPY, 4}, {BEHOLDER, 3}},
            .init_spells = {AREA_BLOODLUST},
            .level_up_stat_probabilities = {4,4,1,1}
        }


    }};

    /**
     * @brief Retrieve the data associated with a specific hero type.
     *
     * This function returns the HeroData object corresponding to the provided HeroType.
     *
     * @param town_type The type of the hero for which data is requested.
     * @return HeroData The data structure containing information about the specified hero type.
     */
    inline HeroData getHeroData(HeroType hero_type){
        if(hero_type < 0 || hero_type >= N_HERO_TYPES){
            throw InvalidIDError("HeroType", hero_type);
        }
        return HERO_DATA.at(hero_type);
    }


    /**
     * @brief Get the name of a troop based on its type.
     *
     * @param troop_type The type of the troop whose name is to be retrieved.
     * @return std::string The name of the troop.
     */
    inline std::string getTroopName(const TroopType troop_type){
        return getTroopData(troop_type).troop_name;
    }

    /**
     * @brief Get the file path to the texture of a troop based on its type.
     *
     * @param troop_type The type of the troop whose texture path is to be retrieved.
     * @return std::string The file path to the texture of a troop
     */
    inline std::string getTroopTextureName(const TroopType troop_type){
        return getTroopData(troop_type).texture_name;
    }

    /**
     * @brief Create a description string for a spell, including its name and level.
     *
     * @param spell_type The type of the spell (as SpellType).
     * @param level_of_spell The current level of the spell.
     * @return std::string description containing the spell's name and level.
     */
    inline std::string createSpellDesc(SpellType spell_type, const int level_of_spell){
        return getSpellData(spell_type).spell_name + " (Poziom: " + std::to_string(level_of_spell) + ")";
    }
};

#endif