/**
 * @file Town.h
 * @author Bartosz Han
 * @brief This file contains Town class. And TownResources struct.
 * Objects of Town class are used by player to generate for themselves additional resources, such as heroes, troops
 * and spells.
 *
 */

#ifndef TOWN_H
#define TOWN_H

#include <memory>

#include "Data/GameData.hpp"
#include "Data/ObjectDataEnums.hpp"
#include "GameObjects/Buildings/Building.h"
#include "GameObjects/BaseObjects/OwnedByPlayer.h"

class TownInteractiveObject;
class Army;


/**
 * @struct TownResources
 * @brief Represents the resources currently available to buy in a town.
 *
 * This structure holds information about the heroes currently available to buy in the town,
 * the spells present in the mage guild, and the troops available for purchase in the town.
 *
 * Members:
 * - heroes_in_tavern_: An array containing the types of heroes available in the tavern.
 * - spells_in_mage_guild_: A vector of pairs, where each pair consists of a SpellType and
 *   an integer representing the level of the spell available in the town.
 * - troops_available_to_buy_: A vector of pairs, where each pair consists of a TroopType and
 *   an integer representing the quantity of that troop type available for purchase.
 */
struct TownResources{

    std::array<HeroType, game_data::MAX_NUMER_OF_HEROES_IN_TAVERN> heroes_in_tavern_;
    std::vector<std::pair<SpellType, int>> spells_in_mage_guild_;
    std::vector<std::pair<TroopType, int>> troops_available_to_buy_;

};

/**
 * @class Town
 * @brief Objects of Town class are used by player to generate for themselves additional resources, such as heroes, troops
 * and spells.
 *
 * Towns consists of several Buildings. Each building provides different resources and other enhancements to the player and their armies,
 * and each building can be upgraded to further help player in battle.
 *
 * Upon interacting with friendly town (the same player IDs), player can buy additional troops and spells for the army, or can buy new armies for them.
 * When interacting with enemy town (with different player ID), the battle is initialised. When attacker wins, the town now belongs to the player.
 * Players can attack both neutral towns, and towns of other players.
 *
 * If player does not control at least one town, it si eliminated from the game. Player must start with at least one town controlled by them.
 *
 */
class Town: public OwnedByPlayer{

private:
    const TownData town_data_;
    int town_id_;
    std::array<int, N_TOWN_STATS> statistics_;
    TownType town_type_;
    const int init_x_pos_, init_y_pos_;

    /**
     * @brief Generate TownInteractiveObjects associated with this town into world map.
     *
     * Upon interacting with these objects, player will begin either visiting this town and buying troops,
     * or - in case of enemy town - attack on this town.
     *
     * This method should be called only once.
     */
    void generateInteractiveObjects();


    std::array<std::unique_ptr<Building>, N_BUILDING_TYPES> buildings_;

    /// @brief Give player, who is controling this town, additional GOLD Resource with quantity provided by TownStat GOLD_GENERATED.
    /// This method called once per day. If no player control this town, nothing is happening.
    void addGoldToPlayer();

    std::shared_ptr<Army> town_army_;
    TownResources town_resources_;
    bool flag_leaving_town_;
    bool flag_was_town_visited_this_turn_;

    /// @brief Initialize town_resources_ strcut with default values.
    void initializeTownResources();

    /// @brief Generate Buildings objects of this Town.
    void generateBuildings();

    /**
     * @brief Method to call, when friendly player is interacting with this town.
     * This method will be awaiting players' decisions, by calling PlayerController::makeTownDecision() until player decides to leave town.
     */
    void visitTown();

    /// @brief Method to call, when friendly player is interacting with this town.
    /// This method will perform logic related to beginning interaction and visiting the Town.
    void onEnteringTown();

    /// @brief Method to call, when friendly player is decides to leave this town.
    /// This method will perform logic related to leaving the town, and returning to world map.
    void leaveTown();

    /**
     * @brief Attempt to buy troop, which is at provided index of town's troops available to buy list.
     *
     * This method will check, if player meets the criteria of buying the troop, and, if check passes,
     * will do logic related to buying it, including spending player resources, and reducing town's available troops to buy.
     *
     * @param index Index, at which in town's troop available to buy list there is troop player wants to buy.
     * @return True, if player successfully buy new troop, false, if player wasn't able to buy troop (for example, they didn't have enough resources).
     */
    bool buyTroop(const int index);

    /**
     * @brief Attempt to buy hero, which is at provided index of town's heroes available to buy list.
     *
     * This method will check, if player meets the criteria of buying the hero, and, if check passes,
     * will do logic related to buying it, including spending player resources, and removing hero from town's resources lists.
     *
     * @param index Index, at which in town's heroes list there is hero player wants to buy.
     * @return True, if player successfully buy new hero, false, if player wasn't able to buy hero (for example, they didn't have enough resources).
     */
    bool buyHero(const int index);

    /**
     * @brief Attempt to buy spell, which is at provided index of town's spells available to buy list.
     *
     * This method will check, if player meets the criteria of buying new spell, and, if check passes,
     * will do logic related to buying it, including spending player resources, and removing spell from town's resources lists.
     *
     * @param index Index, at which in town's spells list there is spell player wants to buy.
     * @return True, if player successfully buy new spell, false, if player wasn't able to buy spell (for example, they didn't have enough resources).
     */
    bool buySpell(const int index);

    /**
     * @brief Attempt to upgrade one of buildings of this town, with given building type.
     *
     * This method will check, if player meets the criteria of upgrading the building, and, if check passes,
     * will do logic related to upgrading it, including spending player resources.
     *
     * @param building BuildingType of building player wants to upgrade.
     * @return True, if player successfully upgrades building, false, if player wasn't able to upgrade the building
     * (for example, they didn't have enough resources).
     */
    bool upgradeBuilding(const BuildingType building);

    /**
     * @brief Check, if player owning this town has enough resources to spend the amount of resources as provided in resources array.
     *
     * This method does not change the quantities of resource.
     *
     * @param resources Amount to resources expected to spend.
     * @return True, if player have enough resources to spent on every resource in resources_quantity array, false otherwise.
     *
     * @note This method uses WorldManager::getCurrentPlayer() to determine the owner of this Town, as only the player
     * who is currently taking its turns on world map can enter friendly town.
     */
    bool canPlayerAfford(const ResourceArray& resources) const;

    /**
     * @brief Attempt to decrease the amount of owner of this town Resources, based on the amounts provided in resources array.
     *
     * If owner of this town does not have enough resources, this method does nothing.
     *
     * @return True, if resources were successfully decreased, false otherwise.
     *
     * @note This method uses WorldManager::getCurrentPlayer() to determine the owner of this Town, as only the player
     * who is currently taking its turns on world map can enter friendly town.
     */
    bool spendPlayerResources(const ResourceArray& resources);


public:


    /**
     * @brief Constructs a new Town object.
     *
     * @param town_data TownData structure containing information about the town.
     * @param town_type TownType of the town.
     * @param town_id   The unique within the game identifier for the town.
     * @param init_x_pos The initial X coordinate position of the town on the map.
     * @param init_y_pos The initial Y coordinate position of the town on the map.
     * Coordinates of the town are the upper left position of the object associated with this town.
     *
     */
    Town(const TownData town_data, const TownType town_type, const int town_id, const int init_x_pos, const int init_y_pos);

    /**
     * @brief Attempt to interact with town.
     *
     * If player, who is interacting with the town, is controlling this town, and is interacting with it for the first time,
     * the town visit logic is instanced. If friendly player does this for the second and next time,
     * nothing happens, as player can only visit particular town
     * only once per town.
     *
     * If player does not control town, a battle is initialized and logic is done based on result of the battle.
     *
     */
    void interactWithTown();

    /// @brief Get ID of this town
    int getTownID() const;

    /// @brief Get the coordinates of this this, as a form of pair of integers.
    /// Coordinates of the town are the upper left position of the object associated with this town.
    const std::pair<int, int> getInitPosOfTown() const;

    /**
     * @brief Get the coordinates, at which new world army created in this town should spawn at the world map.
     *
     * When new world army is created, it should spawn near the tow at which hero of army was bought.
     *
     * This method is looking for the first valid and unoccupied tile, at which hero can spawn.
     *
     * @return std::pair<int, int> Coordinates on which new world army can spawn.
     */
    std::pair<int, int> getArmyStartingPos() const;

    /// @brief Do the logic related to the event when new day is beginning.
    void onNewDay();

    /// @brief Do the logic related to the event when new week is beginning.
    void onNewWeek();

    /// @brief Get reference to the building of provided BuildingType.
    Building& getBuilding(BuildingType building) const;

    /// @brief Get value of one of town's statistic.
    int getStatistic(const TownStat stat) const;

    /// @brief Set value of one of town's statistic.
    void setStatistic(const TownStat stat, const int new_value);

    /// @brief Get army of town. Army of town is used for battles against enemy world armies.
    Army& getArmy() const;

    /// @brief Destroy previous one, and create new army, with power level based on the state of the town.
    void resetArmy();

    void setPlayerID(const int new_player_id) override;

    /// @brief Get reference TownData struct of town. This struct contains data about resources (but not availability of them) in town.
    const TownData& getTownData() const;

    /// @brief Get reference TownResources struct of town. This struct contains data about resources currently available to get in town.
    const TownResources& getTownResources() const;

    /// @brief Returns true, if player can buy spell with provided index in town's spell list.
    bool canBuyTroopAtIndex(const int index) const;

    /// @brief Add new spell, with provided brief data of new_spell (its ID and initial level) to the resources of town.
    void addSpellToResources(const std::pair<SpellType, int> new_spell);

    /// @brief Reset heroes available in town, by removing previous list of heroes and randomly generating new one.
    /// Only works if buying heroes in town is enabled for this town.
    void resetHeroesInTavern();

    /// @brief Increase the number of all troops available to buy in town, based on growth of troops in towns and state of town.
    void increaseTroopsAvailableToBuy();

    /// @brief Increase the number of single troop type available to buy in town, based on growth of troops in towns and state of town.
    /// Troop, whose quantity should be increased should be at provided index of town's troop list.
    void increaseSingleTroopAvailableToBuy(const int index);

    /**
     * @brief Attempt to perform decision related to the town.
     *
     * Possible decisions are: buying hero, troop, or spell, upgrading building, or leaving town.
     *
     * @param decision TownDecision to perform.
     * @return True, if decision was performed successfully, false otherwise.
     */
    bool attemptToDo(TownDecision decision);


    /// @brief Set the flag, indicating that this town was already visited by friendly player this turn, to false.
    void resetFlagWasTownVisitedThisTurn();

    Town(const Town&) = delete;
    Town& operator=(const Town&) = delete;
};

#endif
