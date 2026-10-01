/**
 * @file Army.h
 * @author Bartosz Han
 * @brief This file contains Army class, which is one of componenets of the game. Players can have armies, and battle against other armies in order to
 * get resources, or achieve advantage in the game.
 *
 */
#ifndef ARMY_H
#define ARMY_H

#include "GameObjects/BaseObjects/BattleElement.h"
#include "GameObjects/Hero.h"
#include "GameObjects/Stack.h"
#include "GameObjects/BaseObjects/InteractiveObject.h"
#include "Data/ObjectDataEnums.hpp"
#include "Miscellaneous/IPainter.h"

/**
 * @class Army
 * @brief This class is one of componenets of the game. Players can have armies, and battle against other armies in order to
 * get resources, or achieve advantage in the game.
 *
 * Armies are mina object, which are used during battle stages of game.
 * Army consists of Stacks, and optionally Hero. Army is considered defeated when it does not any stack in it.
 * Armies are owned by players, or can be neutral (with player ID of -1).
 *
 */
class Army: virtual public InteractiveObject, public OwnedByPlayer, public BattleElement{

protected:
    int army_experience_reward_;
    const bool flag_is_world_army_;

    /**
     * @brief Create a new Stack object, based on provided data.
     *
     * This function will extract data of the troop of the stack from game_data::getTroopData() function, based on provided troop type.
     * Newly created stack will not belong to any army.
     *
     * @param troop_type TroopType (or ID) of troop to create.
     * @param quantity Initial number of troops, which will Stack object will consist of.
     * @param player_id ID of player which will own this stack.
     * @return std::shared_ptr<Stack> Shared pointer to newly created Stack.
     */
    static std::shared_ptr<Stack> createStack(const TroopType troop_type, unsigned const int quantity, const int player_id);

    std::vector<std::shared_ptr<Stack>> stacks_;
    int bonus_experience_points_;

public:

    /**
     * @brief Construct a new Army object.
     *
     * @param texture_name File path to texture, which will be used as graphical representation of army.
     * @param init_pos_x Initial X position of army on world map.
     * @param init_pos_y Initial Y position of army on world map.
     * @param is_world_army Flag, whether or not this army is a instance of WorldArmy object.
     */
    Army(const std::string texture_name, const int init_pos_x, const int init_pos_y, const bool is_world_army = true);

        /**
     * @brief Construct a new Army object.
     *
     * In this constructor, army will not have any texture associated to it.
     *
     * @param init_pos_x Initial X position of army on world map.
     * @param init_pos_y Initial Y position of army on world map.
     * @param is_world_army Flag, whether or not this army is a instance of WorldArmy object.
     *
     */
    Army(const int init_pos_x, const int init_pos_y, const bool is_world_army = true);

    /**
     * @brief Construct a new Army object.
     * In this constructor, army will not have any position on world map (it will have pos of (-1, -1)).
    */
    Army();

    void interact() override;

    /**
     * @brief Get the reference of the vector of pointer of stack, which belong to this army.
     *
     * @return const std::vector<std::shared_ptr<Stack>>& Vector of pointers to stacks.
     */
    const std::vector<std::shared_ptr<Stack>>& getArmyStacks() const;

    /**
     * @brief Get the vector of all BattleObjects, which army consists of.
     *
     * BattleObjects of army are all Stacks, and Hero if army has it.
     *
     * @return std::vector<std::shared_ptr<BattleObject>> Vector of all battle objects.
     */
    virtual std::vector<std::shared_ptr<BattleObject>> getBattleObjectsOfArmy() const;

    /**
     * @brief Add new Stack object to the army, or increase number of troops of existing Stack object.
     *
     * This method will check, if army have already troops of provided TroopType.
     * If they do have, it will increase their quantity by a number provided of this method.
     * If they do not have, it will crate new Stack object, and initialize it will with data provided by this method.
     *
     * @param troop_type TroopType of Troop to increase quantity.
     * @param quantity Quantity by which the Stack's troop quantity will be increased.
     *
     * @return Ture, if adding new troops was successful,
     * false, if new troops couldn't be add to this army (for example, when army have already maximum number of stacks)
     */
    virtual bool addTroops(const TroopType troop_type, const unsigned int quantity);

    /// @brief Returns true, if army contains at least one stack.
    bool isArmyActive() const;

    /**
     * @brief Remove provided stack from army.
     *
     * This method will try to find the stack, based on address of objects.
     * This method will return iterator to Army's vector of stacks,
        where the removed stack previously was (iterator is provided by erase() method.)
     *
     * @param stack_to_remove Stack to remove from vector of stacks.
     * @return std::vector<std::shared_ptr<Stack>>::iterator Iterator to the Army's vector of stacks, where the removed stack previously was.
     */
    std::vector<std::shared_ptr<Stack>>::iterator removeStackFromArmy(Stack& stack_to_remove);

    /// @brief Get pointer to the hero of this army, or nullptr, if this object does not have hero associated with it.
    virtual std::shared_ptr<Hero> getArmyHeroPtr() const;

    /**
     * @brief Generate new army object, with will consist of stacks with troops provided in troop_type_array.
     *
     * This method will create new army object, and will add new stacks with troops with IDs
     * from troop_type_array with probability of each ID stored in probabilities vector.
     * This method will add new stacks, until power of new army will surpass value of game_data::PATTERN_ARMY_POWER[army_level - 1].
     * This army will initially be neutral.
     *
     * This method in used various armies associated with objects on the world map.
     *
     * @param army_level Expected level power of army. Method will be adding new troops until army new have enough power, provided with this value.
     * @param troop_type_array TroopType of, of which new army will consist of.
     * @param probabilities Probabilities, of which troops of troop_type_array will be added to the vector.
     * @return std::shared_ptr<Army> Pointer to the new army.
     */
    static std::shared_ptr<Army> generatePatternArmy(const unsigned int army_level, const std::vector<TroopType> troop_type_array, std::vector<int> probabilities={});

    /**
     * @brief Generate new army object, with will consist of stacks with troops provided in troop_type_array.
     *
     * This method will create new army object, and will add new stacks with troops with IDs
     * from troop_type_array with probability of each ID stored in probabilities vector.
     * This method will add new stacks, until power of new army will surpass value of game_data::PATTERN_ARMY_POWER[army_level - 1].
     * This army will initially be neutral.
     *
     * This method in used various armies associated with objects on the world map.
     *
     * @param army_level Expected level power of army. Function will be adding new troops until army new have enough power, provided with this value.
     * @param troop_type Single troop type, which will new army consist of.
     * @return std::shared_ptr<Army> Pointer to the new army.
     */
    static std::shared_ptr<Army> generatePatternArmy(const unsigned int army_level, const TroopType troop_type);

    /**
     * @brief Get the value of power of army.
     *
     * Power of army is based on stacks of army and this power statistic. The more and better troops army have, the bigger this value is.
     *
     * @return int Power of army.
     */
    int getArmyPower() const;

    /// @brief Returns true, if this object is instance of WorldArmy class. False otherwise.
    bool isWorldArmy() const;

    /// @brief Set army experience reward value, which indicates, how many experience points will enemy get if it defeats this army.
    /// The value set is solely based on the stacks of this army.
    virtual void setArmyExperienceReward();

    /// @brief Get army experience reward value, which indicates, how many experience points will enemy get if it defeats this army.
    int getArmyExperienceReward() const;

    /// @brief Set bonus experience points, which enemy will get if it defeats this army, to provided value.
    /// As setArmyExperienceReward() only adds experience points based on stacks of army,
    /// this method can override this value by adding additional points.
    void setBonusExperiencePoints(const unsigned int value);

    /// @brief Clear the vector of stacks of this army.
    void removeAllStacksFromArmy();

    void drawAdditionalData(IPainter& painter) const override;

    /**
     * @brief Call Stack::resetTroopData() from all stackss of this army.
     *
     * This method resets statistics of stackss into its default state.
     * This method should be called after battles.
     *
     */
    void resetStacksData();

    /// @brief Generate string, containing essential data about army.
    /// This method will return containing owner of this army, and its ID in player's army vector.
    virtual const std::string toString() const;

    virtual void registerBattle(Battle* battle) override;
    virtual void deregisterBattle() override;

};

#endif