/**
 * @file Building.h
 * @author Bartosz Han
 * @brief This file contains Building class, which is an abstract class used by various other objects,
 * which are responsible for managing town resources and interacting between towns and players and armies.
 *
 */

#ifndef BUILDING_H
#define BUILDING_H

#include "GameObjects/BaseObjects/DynamicDrawableObject.hpp"
#include "Data/ObjectDataEnums.hpp"

class Player;
class Town;

/**
 * @class Building
 * @brief This class is an abstract class used by various other objects,
 * which are responsible for managing town resources and interacting between towns and players and armies.
 *
 * Building are one of the mechanics of the game. During the game, player can interact with different buildings,
 * which can provide them with various resources. Building can also be upgraded, which enables them to provide player with better
 * resources.
 */
class Building: public DynamicDrawableObject{

protected:
    /// @brief Get reference to the Player, which is owner of this building.
    Player& getOwnerOfBuilding();
    Town& town_of_building_;
    int building_level_;
    const int max_building_level_;
    const std::vector<std::string> building_sprites_;

    /// @brief Do the logic related to the event, in which player upgraded this building.
    virtual void onLevelUp() = 0;

    /**
     * @brief Generate string, describing this building, which will be used in GUI.
     *
     * @param building_name Name of this building.
     * @return const std::string Description of this building.
     */
    const std::string generateBuildingDescription(const std::string building_name) const;

public:
    /**
     * @brief Construct a new Building object
     *
     * @param town_of_building Town, with which this building is associated.
     * @param max_level Maximum possible level of this building.
     * @param building_sprites Vector of file paths to the images representing this building in different states (levels).
     *
     * @note All building starts from level 0.
     */
    Building(Town& town_of_building, int max_level, std::vector<std::string> building_sprites);

    /**
     * @brief Check, if building meet all the criteria to be upgraded.
     *
     * this check includes checking, if player has enough resources, as well as other, additional requirements.
     *
     * @return true, if player meets all the criteria to upgrade this building, false otherwise.
     */
    virtual bool canLevelUp();

    /**
     * @brief Attempt to upgrade this building by one level.
     *
     * This method will check, if player meets all the criteria to upgrade this building
     * and if so, this method will perform actions related to upgrading this building, such as incrementing the
     * level of this building, calling onLevelUp() method, and reducing the resources of player.
     *
     * @return true if building has been successfully upgraded, false if player did not meet the criteria to upgrade the building.
     */
    virtual bool attemptToLevelUp();

    /// @brief Get current building level.
    int getBuildingLevel() const;

    /// @brief Get the maximum possible level of building.
    int getMaxBuildingLevel() const;

    const std::string getCurrentTextureName() const override;

    /// @brief Get the string, shortly describing this building. Used in GUI.
    virtual const std::string getBuildingDescription() const = 0;

    /// @brief Generate the ResourceArray, describing resources needed to further upgrade this building by one level.
    const virtual ResourceArray& getUpgradeCost() const = 0;

    Building(const Building&) = delete;
    Building& operator=(const Building&) = delete;

};

#endif