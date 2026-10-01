/**
 * @file Stack.h
 * @author Bartosz Han
 * @brief This file contains Stack class. Objects of these class are essential part of the armies. They are taking part in battles,
 * in which they are dealing damage points to each other.
 *
 */
#ifndef STACK_H
#define STACK_H

#include <memory>
#include <functional>

#include "GameObjects/BaseObjects/BattleElement.h"
#include "GameObjects/BaseObjects/BattleObject.h"
#include "GameObjects/BaseObjects/InteractiveObject.h"
#include "GameObjects/BaseObjects/MovableObject.h"
#include "Data/GameData.hpp"
#include "Miscellaneous/IPainter.h"

class Tile;
class Army;
class Battle;

/**
 * @class Stack
 * @brief Objects of this class essential part of the armies. They are taking part in battles,
 * in which they are dealing damage points to each other.
 *
 * During battle, player can steer their stacks to either walk to given tile, or attack enemy stacks.
 * Each Stack is an abstraction of squad same troops, with given quantity. By receiveing damage, Stack object might lose some of the
 * troops, and if quantity of troops drops to 0, this Stack object is considered defeated and removed from game.
 *
 * Stack objects contains several data and statistics related to the troops.
 *
 */
class Stack : public MovableObject, public BattleObject, public BattleElement{
private:
    const TroopType troop_type_;
    const std::string troop_name_;
    std::array<int, N_TROOP_STATS> statistics_;
    std::shared_ptr<std::array<int, N_HERO_STATS>> hero_statistics_;
    int quantity_;
    int current_health_;
    TroopBattleData troop_battle_data_;
    static std::function<float(Stack&, Stack&)> stack_damage_function_;

    /**
     * @brief Move this Stack object to the provided tile.
     *
     * This method will find the path to the given tile, or will stop at tile if it will be close enough to stack's range.
     * In this method, stack will not try to attack enemies.
     * Event tile must be empty in order to make this function work.
     *
     * @param event_tile Tile&, to which stack should move to.
     * @param stop_when_in_range Flag, determining if stack should stop when is in appropiate distance (stack's range), or only
     * when player is on event tile.
     * @return True, if player successfully walked into tile, or was close enough to the event tile, or at least got close to the event tile.
     * False, if there isn't any path to event tile.
     */
    bool walkTo(Tile& event_tile, bool stop_when_in_range = false);

    /**
     * @brief Attempt to attack enemy stack at event tile, by fiest geting close to it, and later dealing damage to it.
     *
     * This method will try to find path to such tile, which is in range of attack to enemy tile.
     * when stack will be in range, it will attack the enemy stack, by interacting with them.
     * There must be an enemy stack at event tile, otherwise the method will do nothing.
     * This method might make stack interact with each other, if at the end of path stack are not close enough.
     *
     * @param event_tile Tile&, to which stack should move to in order to attack enemy stack.
     * @return True, if player successfully walked or was close enough and attacked enemy stack, or at least got close to the event stack.
     * False, if there isn't any path to event tile. The act of attacking does not affect the return value of this method.
     */
    bool walkAndAttack(Tile& event_tile);

protected:


    static const std::array<int, N_HERO_STATS> null_hero_statistics_;

    /**
     * @brief Get current health of this object.
     *
     * Health of this object is sum of all healths of all troops of this object.
     * This is the value, which is decremented when another stack is dealing damage to this object.
     * When this value is equal or below zero, this object is considered defeated and it should be removed from game.
     *
     */
    int getCurrentHealth() const;

    virtual int calculateDamage(const Stack& enemy_stack, const std::function<float()> damage_per_troop_function) const;

    /**
     * @brief Calculate the number of hit points, which this stack should deal to the given enemy stack.
     *
     * The amount of damage is based on statistics of both stack, as well as heroes of their armies.
     * This method does not deal damage to enemy stack.
     *
     * @param event_stack Stack&, to which we want to deal damage.
     * @return Number of hit points, which this stack should deal to the enemy stack.
     */
    virtual int calculateRandomStackDamage(const Stack& event_stack) const;

public:

    /**
     * @brief Construct a new Stack object
     *
     * Additional data about stack, such as its statistics, is generated via game_data::getTroopData() function.
     *
     * @param troop_type TroopType of this stack, based on which additional data is being generated.
     * @param player_id ID of player, who owns this object.
     * @param quantity Initial quantity of troops of this object.
     */
    Stack(const TroopType troop_type, const int player_id,
        const int quantity);

    bool attack(Tile& event_tile) override;
    void interact() override {};

    /// @brief Perform interaction with another stack, by calculating and dealing damage to it.
    void interact(Stack& event_stack);

    /**
     * @brief Get the quantity of troops of this object.
     *
     * Each Stack object is an abstraction of squad of some type troops.
     * When this value is equal or below zero, this object is considered defeated and it should be removed from game.
     *
     * @return * int
     */
    int getQuantity() const;

    /// @brief Add additional troops to this object, with provided quantity.
    void addQuantity(const int value_to_add);

    ///@brief Get quantity of stack multiplied by health points of single.
    /// This value is not equal to current health of stack.
    int getCurrentKnownHealth() const;

    /**
     * @brief Decrement the amount of health of this object.
     *
     * This method will also decrement number of troops based on damage dealt, and, if the amount of troops drops to 0,
     * set the flag indicating that this stack is defeated.
     */
    void receiveDamage(const int damage);


    /**
     * @brief Returns the quantity of this stack, if it would receive provided amount of damage.
     */
    int receiveMockDamage(const int damage) const;

    /// @brief Return average damage points single troop of this stack inflict.
    float getAverageDamagePerTroop() const;


    /// @brief Set the pointer to the statistics of hero of army of this stack.
    void setHeroStatistics(std::shared_ptr<std::array<int, N_HERO_STATS>> statistics);

    /// @brief Get the reference to the statistics of hero of army of this stack
    const std::array<int, N_HERO_STATS>& getHeroStatistics() const;
    int getHeroStatistic(HeroStat stat) const;

    /// @brief Get reference to the array of statistics of this stack.
    const std::array<int, N_TROOP_STATS>& getStatistics() const;
    int getStatistic(TroopStat stat) const;

    /// @brief Increase the value of given statistic by given amount.
    void addStatistic(TroopStat stat_to_add, int amount);

    /// @brief Get TroopType of this stack.
    TroopType getTroopType() const;

    const std::string getUITextureName() const override;

    /// @brief Get name of this stack. Used in UI.
    const std::string getTroopName() const;
    const std::pair<std::vector<std::string>, std::vector<std::string>> getDataText() const override;
    void drawAdditionalData(IPainter& painter) const override;
    const std::string toString() const override;

    /// @brief Reset the statistics of the troop, by seting them to values, which were passed when construing this object.
    /// During battle, statistics of stacks can change. This method should be called after battle, to remove all enhancements, which stack got during the battle.
    void resetTroopData();


    /// @brief Get current average stack damage points, provided it attacks enemy, which has defense points equal to 0.
    float getCurrentAverageStackDamage() const;

    /// @brief Get current average stack damage points, provided it attacks enemy, which has defense points equal to 0,
    /// if this stack had provided quantity of troops.
    float getCurrentAverageStackDamage(const int scenario_quantity) const;

    /// @brief Get number of damage points this stack would inflict provided enemy.
    float calculateAverageStackDamage(const Stack& enemy_stack) const;

    /// @brief Get minium amount of damage this stack would inflict provided enemy
    int calculateMinimumDamage(const Stack& enemy_stack) const;

    /// @brief Check, if this stack would be able to attack enemy stack this turn, which would be at provided coordinates.
    bool canAttackEnemyStackThisTurn(const int x_pos, const int y_pos) const;

    /// @brief Check, if this stack would be able to attack enemy stack this turn, which would be at provided coordinates,
    /// if this stack would follow of moves in path_to_attack.
    bool canAttackEnemyStackThisTurn(const int x_pos, const int y_pos, const std::vector<WorldMove> path_to_attack) const;

    /// @brief Check, if this stack would be able to attack enemy (object) stack this turn.
    bool canAttackEnemyStackThisTurn(const InteractiveObject& object) const;

    /// @brief Check, if this stack would be able to get closer to enemy stack this turn, if enemy would be at provided coordinates.
    bool canGetCloserToEnemyStackThisTurn(const int enemy_x_pos, const int enemy_y_pos) const;

    /// @brief Check, if this stack would be able to get closer to enemy stack (enemy_object) this turn.
    bool canGetCloserToEnemyStackThisTurn(const InteractiveObject& enemy_object) const;

    /// @brief Returns number of average damage points this stack would lost if it received provided amount of damage.
    int calculateDecreaseInAttackIfDamaged(const int damage) const;

    /// @brief Returns damage multiplier against enemy stack, based on attack and defense statistics of this stack, enemy and both heroes.
    float calculateDamageMultiplierTowardsEnemyStack(const Stack& enemy_stack) const;

    /// @brief Returns quantity of this stack, if it had total of provided health points.
    int healthToQuantity(const int health) const;

    /// @brief Returns damage function, in which every troop of stack deals random amount of damage from range [min_damage; max_damage].
    static std::function<float(Stack&, Stack&)> randomStackDamageFunction();

    /// @brief Returns damage function, in which every troop of stack deals (min_damage + max_damage)/2 damage points.
    static std::function<float(Stack&, Stack&)> avarageStackDamageFunction();

    /// @brief Register function, which will be used in program.
    /// There are two such functions: every troop deals random damage, or every troop deals its average damage.
    static void registerStackDamageFunction(std::function<float(Stack&, Stack&)> function);

};

#endif