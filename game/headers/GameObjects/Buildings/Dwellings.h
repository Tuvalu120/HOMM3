/**
 * @file Dwellings.h
 * @author Bartosz Han
 * @brief This file contains Dwellings class, which is one of the buildings of the town,
 * which is responsible for providing player with additional troops to their armies.
 *
 */

#ifndef DWELLINGS_H
#define DWELLINGS_H

#include "GameObjects/Buildings/Building.h"

/**
 * @class Dwellings
 * @brief This class is one of the Buildings of the town,
 * which is responsible for providing player with additional troops to their armies.
 *
 * As player levels up this building, this object provides player with new and better troops, which can be bought by the player.
 *
 */
class Dwellings : public Building{

private:
    void onLevelUp() override;

public:

    /**
     * @brief Construct a new Dwellings object.
     *
     * @param town_of_building Town, with which this building is associated.
     * @param building_sprites Vector of file paths to the images representing this building in different states (levels).
     *
     */
    Dwellings(Town& town_of_building, std::vector<std::string> building_sprites);
    const std::string getBuildingDescription() const override;
    const ResourceArray& getUpgradeCost() const override;

};

#endif