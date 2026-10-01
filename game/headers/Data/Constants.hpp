/**
 * @file Constants.hpp
 * @author Bartosz Han
 * @brief This file contains constants namespace, which is used for declaring various constant values related to the game and its resources.
 *
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdexcept>
#include <string>
#include <array>
#include <vector>

#include "Data/ObjectDataEnums.hpp"

/**
 * @namespace constants
 * @brief This namespace contains various constant values related to the game and its resources.
 *
 */
namespace constants{

    /// @brief Maximum number of players in the game.
    const int MAX_NUMBER_OF_PLAYERS = 4;

    /// @brief Maximum number of different stacks types in single army.
    const int MAX_NUMBER_OF_STACKS = 8;

    /// @brief Maximum number of armies for one player to have.
    const int MAX_NUMBER_OF_ARMIES = 4;

    /// @brief Maximum number of spells for one hero to have.
    const int MAX_NUMBER_OF_SPELLS = 4;

    /// @brief Maximum possible level of spell.
    const int MAX_LEVEL_OF_SPELL = 3;

    /// @brief Number of towns on world map.
    const int NUMBER_OF_TOWNS = 12;

    /// @brief Height (size in tiles) of town on world map.
    const int TOWN_WORLD_MAP_HEIGHT = 2;

    /// @brief Width (size in tiles) of town on world map.
    const int TOWN_WORLD_MAP_WIDTH = 2;

    /**
     * @brief X offset of position of spawning new world army in map, with respect of town position.
     * Town position is declared as upper left tile of town.
     */
    const int ARMY_STARTING_POS_OFFSET_X = 2;

    /**
     * @brief Y offset of position of spawning new world army in map, with respect of town position.
     * Town position is declared as upper left tile of town.
     */
    const int ARMY_STARTING_POS_OFFSET_Y = 1;

    /// @brief Priority of armies objects, when another world army is attempting to interact with objects near them.
    const int ARMY_PRIORITY = 3;

    /// @brief Priority of towns objects, when another world army is attempting to interact with objects near them.
    const int TOWN_PRIORITY = 2;

    /// @brief Default priority objects, when another world army is attempting to interact with objects near them.
    const int OTHER_OBJECT_PRIORITY = 1;

    /// @brief Percentage increase of attack values for each attack point of attacking army's hero.
    const float ATTACK_POINT_TO_PERCENTAGE_RATIO = 0.05;

    /// @brief Percentage decrease of attack values for each defense point of attacked army's hero.
    const float DEFENSE_POINT_TO_PERCENTAGE_RATIO = 0.025;

    const std::map<TroopStat, float> TROOP_BATTLE_POINTS_TO_PERCENTAGE = {
        {TROOP_ATTACK, ATTACK_POINT_TO_PERCENTAGE_RATIO},
        {TROOP_DEFENSE, DEFENSE_POINT_TO_PERCENTAGE_RATIO}
    };

    /// @brief Maximum number of attack points for hero to have.
    const int MAX_ATTACK_POINTS = 60;

    /// @brief Maximum number of defense points for hero to have.
    const int MAX_DEFENSE_POINTS = 28;

    const int MAX_NUMBER_OF_TURNS_IN_BATTLE = 1000;

    /// @brief Maximum possible level of town's hall.
    const int MAX_LEVEL_OF_HALL = 3;

    /// @brief Maximum possible level of town's tavern.
    const int MAX_LEVEL_OF_TAVERN = 1;

    /// @brief Maximum possible level of town's fortifications.
    const int MAX_LEVEL_OF_FORTIFICATIONS = 3;

    /// @brief Maximum possible level of town's mage guild.
    const int MAX_LEVEL_OF_MAGE_GUILD = 3;

    /// @brief Maximum possible level of town's dwellings.
    const int MAX_LEVEL_OF_DWELLINGS = 7;

    /// @brief Maximum number of decision option's additional data values.
    const int MAX_NUMBER_OF_DECISION_OPTION_VARIABLES = 3;

    /// @brief Maximum number of options one decision might have.
    const int MAX_NUMBER_OF_DECISION_OPTIONS = 3;

    /// @brief Number of hero's statistics which can be increased when hero is leveling up.
    const int N_OF_HERO_LEVEL_UP_STATISTICS = 4;

    /// @brief List of all hero's statistics which can be given to player when it is leveling up.
    const std::array<HeroStat, constants::N_OF_HERO_LEVEL_UP_STATISTICS> HERO_LEVEL_UP_STATISTICS = {HERO_ATTACK, HERO_DEFENSE, SPELL_POWER, KNOWLEDGE};

    const int NUMBER_OF_DAYS_IN_WEEK = 7;

    /// @brief Number of different terrain types.
    const int N_TERRAIN_TYPES = 4;

    /// @brief Size (width in tiles) of battlefield.
    const int BATTLEFIELD_WIDTH = 20;

    /// @brief Size (height in tiles) of battlefield.
    const int BATTLEFIELD_HEIGHT = 10;

    const int BATTLEFIELD_NO_PATH_DISTANCE = BATTLEFIELD_WIDTH * BATTLEFIELD_HEIGHT;

    /// @brief File path to .txt file with world map data.
    const std::string WORLD_MAP_FILE_PATH = "Maps/OverworldMap.txt";

    /// @brief List of tile types, which are considered as non-walkable walls on world map.
    const std::vector<TileType> BORDER_TILE_TYPES = {
        GRASS_BORDER_TILE,
        ROUGH_BORDER_TILE,
        SNOW_BORDER_TILE,
        SWAMP_BORDER_TILE,
        WORLD_BORDER_TILE};

    /// @brief Resource array with all resource quantities set to 0.
    const ResourceArray EMPTY_RESOURCE_ARRAY = generateResourceArray({});

    /// @brief Position of neutral army generator object in container of all generators in world manager.
    const int NEUTRAL_ARMY_VECTOR_POS = 1;

    /// @brief Position of mine generator object in container of all generators in world manager.
    const int MINE_GENERATOR_VECTOR_POS = 0;

    /// @brief Number of mana points given to a hero for every knowledge point this hero has.
    const int KNOWLEDGE_MANA_POINTS_MULTIPLIER = 10;

    /**
     * @brief Pair used in player controllers decision variables.
     * Value indicator that player does not want to do any action this turn and wants to skip its turn.
     *
     */
    const std::pair<int, int> END_TURN_INDICATOR = {-2, -2};

    /// @brief Maximum level of hero.
    const int MAX_LEVEL_OF_HERO = 20;


    /// @brief Number of decision options when player is leveling up.
    const int N_LEVEL_UP_DECISION_OPTIONS = 2;

    /// @brief Number of days needed for game progress value to be increased by one.
    const int GAME_PROGRESS_GROWTH_DAYS = 14;

    /// @brief Maximum game progress value.
    const int MAX_GAME_PROGRESS_VALUE = constants::MAX_LEVEL_OF_DWELLINGS;


    /// @brief Object type of world generated resources.
    const ObjectType RESOURCE_OBJECT_TYPE                = RESOURCE;

    /// @brief Object type of neutral troops.
    const ObjectType NEUTRAL_TROOPS_OBJECT_TYPE          = NEUTRAL_TROOPS;

    /// @brief Object type of scholars.
    const ObjectType SCHOLAR_OBJECT_TYPE                 = SCHOLAR;

    /// @brief Object type of treasure chests.
    const ObjectType CHESTS_OBJECT_TYPE                  = TREASURE_CHEST;

    /// @brief Object type of mines.
    const ObjectType MINES_OBJECT_TYPE                   = MINE;

    const std::string EMPTY_IMAGE = "Miscellaneous/EmptyImage.png";
    const std::string ASSETS_PATH = "assets/";

    /// @brief Color of neutral resources.
    const std::array<int, 3> NEUTRAL_COLOR = {128, 128, 128};

        /// @brief Players colors. Index of color represents player ID associated with this color.
    const std::array<std::array<int, 3>, constants::MAX_NUMBER_OF_PLAYERS> PLAYER_COLOR = {{
        {0, 0, 255},
        {255, 0, 0},
        {0, 100, 0},
        {128, 128, 0}
    }};

    /// @brief Get color of player by its index.
    inline const std::array<int, 3> playerIDToQColor(const int player_id){
        if (player_id == -1){
            return constants::NEUTRAL_COLOR;
        }
        if(player_id < static_cast<int>(PLAYER_COLOR.size())){
            return PLAYER_COLOR.at(player_id);
        }
        throw std::invalid_argument("Invalid player ID to get color from player color vector");
    }

}

#endif