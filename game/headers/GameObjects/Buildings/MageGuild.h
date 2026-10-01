/**
 * @file MageGuild.h
 * @author Bartosz Han
 * @brief This file contains MageGuild class, which is one of the buildings of the town,
 * which is responsible for providing player and their heroes new spells.
 *
 */

#ifndef MAGE_GUILD_H
#define MAGE_GUILD_H

#include "GameObjects/Buildings/Building.h"

/**
 * @class MageGuild
 * @brief This class is one of the Buildings of the town,
 * which is responsible for providing player and their heroes new spells.
 *
 * As player levels up this building, this object will provide more spells with better levels for the player to buy.
 *
 */
class MageGuild : public Building{

private:
    void onLevelUp() override;

public:
    /**
     * @brief Construct a new MageGuild object.
     *
     * @param town_of_building Town, with which this building is associated.
     * @param building_sprites Vector of file paths to the images representing this building in different states (levels).
     *
     */
    MageGuild(Town& town_of_building, std::vector<std::string> building_sprites);
    const std::string getBuildingDescription() const override;
    const ResourceArray& getUpgradeCost() const override;

};

#endif