/**
 * @file Battlefield.h
 * @author Bartosz Han
 * @brief This file contains Battlefield class, which is type of TileMap used during battles.
 *
 */

#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "Data/Constants.hpp"
#include "GameObjects/BaseObjects/BattleElement.h"
#include "GameObjects/TileMaps/TileMap.hpp"
#include "GameObjects/TileMaps/BattlefieldTile.hpp"
#include "GameObjects/Stack.h"

class Battle;

class Army;

/**
 * @class Battlefield
 * @brief This class is a version of TileMap, which is used to create map needed for battlefield.
 *
 * This map, in addition to normal tile map, also stores data about armies (and related objects, which are on the tile map) which are
 * currently battling with each other.
 * This class also contains few method related to the battle logic of this tile map.
 *
 * This class uses BattlefieldTile as tiles.
 */
class Battlefield : public TileMap<BattlefieldTile>, public BattleElement{

private:
    Army& attacking_army_;
    Army& defending_army_;


    int current_additional_texture_index_ = 0;
    const TileType battlefield_style_;
    std::array<std::pair<int, int>, constants::MAX_NUMBER_OF_STACKS> init_attacker_spawn_tiles_;
    std::array<std::pair<int, int>, constants::MAX_NUMBER_OF_STACKS> init_defender_spawn_tiles_;
    void addTileToMap(const char tile_type, const int x_pos, const int y_pos) override;

    /**
     * @brief Register into vector of stack's spawning positions new
     * position to later spawn stack at given index of given army.
     *
     * @param spawn_for_attacking_army  True, if registered position should belong to attacking army, false if  this position should be registered to defending army.
     * @param stack_id Position of Stack in vector of stacks of army. Stacks of given type of army at the same index will always spawn at same location, provided
     * the battlefield is the same.
     * @param x_pos X coordinate of spawning position.
     * @param y_pos Y coordinate of spawning position.
     */
    void addStackSpawnPos(const bool spawn_for_attacking_army, const int stack_id,
        const int x_pos, const int y_pos);
    std::pair<int, int> attacker_hero_spawning_pos_;
    std::pair<int, int> defender_hero_spawning_pos_;

    /**
     * @brief Check, if in straight line between one tile and another tile, there are any tiles marked as wall.
     *
     * This method is used as a 'line of sight' check - it checks, if ranged stack can attack (shot) target at given position,
     * or there are some obstacles preventing them from hitting the target.
     *
     * This method only takes into consideration tiles marked as walls - if there is another object on given tile, then it is not used in check.
     *
     * @param x_stack X coordinate of check start.
     * @param y_stack Y coordinate of check start.
     * @param x_target X coordinate of check end.
     * @param y_target Y coordinate of check end.
     * @return True if there is obstacle on the line of sight, false if there are no obstacles between stack and target.
     */
    bool checkForObstaclesOnPath(const int x_stack, const int y_stack, const int x_target, const int y_target) const;

public:

    /**
     * @brief Construct a new Battlefield object
     *
     * @param attacking_army Army& initaliasing battle.
     * @param defending_army Army& which was attacked by attacking army.
     */
    // Battlefield(Army& attacking_army, Army& defending_army, Battle* battle);

    Battlefield(Army& attacking_army, Army& defending_army, const TileType battlefield_style);

    /**
     * @brief Spawn stacks of both armies into battlefield.
     *
     * This method will add objects on given tiles of battlefield
     * representing stacks of both army.
     *
     * @param attacker reference to attacking army
     * @param defender reference to defending army
     */
    void spawnStacksOnBattlefield(Army& attacker, Army& defender);

    /// @brief Get the type of battlefield, as a one of TileTypes. Battlefield style is inheritted from world map tile, on which battle occurs.
    TileType getBattlefieldStyle() const;
    void initializeMap() override;

    /// @brief Get vector of coordinates of possible positions, on which attacking army stacks can spawn.
    /// Stacks will spawn at tile, whose coordinates have same index in vector, as has given stack.
    const std::array<std::pair<int, int>, constants::MAX_NUMBER_OF_STACKS> getInitAttackerStacksSpawnPos() const;

    /// @brief Get vector of coordinates of possible positions, on which defending army Stacks can spawn.
    /// Stacks will be spawn at tile, whose coordinates have same index in vector, as has given stack.
    const std::array<std::pair<int, int>, constants::MAX_NUMBER_OF_STACKS> getInitDefenderStacksSpawnPos() const;

    /// @brief Get position, at which attacking army hero should spawn. Heroes do not take part in battle and this method is mainly used in GUI.
    const std::pair<int, int> getDefenderHeroSpawningPos() const;

    /// @brief Get position, at which defending army hero should spawn. Heroes do not take part in battle and this method is mainly used in GUI.
    const std::pair<int, int> getAttackerHeroSpawningPos() const;

    /**
     * @brief Check, if current stack can successfully perfrom its attack(), if they were on (x_stack, y_stack) position,
     * and their target would be at (x_target, y_target).
     *
     * This method checks, if stack, which is currently taking its battle turn,
     * can stop on tile, is close enough to target to attack and the if target is on line of sight on stack.
     *
     * This function is used for object pathfinding algorithm.
     *
     * @param x_stack X position, where stack would attack the target.
     * @param y_stack Y position, where stack would attack the target.
     * @param x_target X of target of stack.
     * @param y_target Y of target of stack.
     * @return true
     * @return false
     */
    bool canCurrentStackAttackFrom(const int x_stack, const int y_stack, const int x_target, const int y_target) const;
    bool canStackAttackFrom(const Stack& stack, const int x_stack, const int y_stack, const int x_target, const int y_target) const;

    bool canCurrentStackAttackEnemyStackThisTurn(const int x_pos, const int y_pos) const;
    bool canCurrentStackAttackEnemyStackThisTurn(const InteractiveObject& object) const;

    /**
     * @brief Check, if target of the stack is within range (distance of stack's RANGE statistic) of its attack.
     *
     * @param x_stack X position, where stack would attack the target.
     * @param y_stack Y position, where stack would attack the target.
     * @param x_target X of target of stack.
     * @param y_target Y of target of stack.
     * @param epsilon Small float value to fix float comparison issues.
     * @return True if stack's target is within stack's range, false otherwise.
     */
    bool checkDistanceFromTarget(const Stack& stack_to_check, const int x_stack,
        const int y_stack, const int x_target, const int y_target) const;

    /// @brief Get reference to attacking Army, which initialized the battle.
    Army& getAttackingArmy() const;

    /// @brief Get reference to defending Army, which was attacked by another army.
    Army& getDefendingArmy() const;

    std::vector<std::pair<int, int>> getTilesWhereObjectCanGoTo(bool include_starting_tile = true);

    std::pair<int, int> getCoverValueOfTile(const int tile_x_pos, const int tile_y_pos);
    float getSumOfDistancesBetweenTileAndCurrentArmy(const int tile_x_pos, const int tile_y_pos) const;

    /// @brief Generate std::function from canCurrentStackAttackFrom() method, which can be used in object pathfinding algorithm.
    std::function<bool(const int, const int, const int, const int)> generateCurrentStackEndConditionFunction() const;

    /// @brief Generate std::function from canStackAttackFrom() method, which can be used in object pathfinding algorithm.
    std::function<bool(const int, const int, const int, const int)> generateStackEndConditionFunction(const Stack& stack) const;
};

#endif