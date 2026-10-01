/**
 * @file Fortifications.h
 * @author Bartosz Han
 * @brief This file contains Fortifications class, which is one of the buildings of the town,
 * which is responsible for providing several enhancements to the town's troops and defences.
 *
 */

#ifndef FORTIFICATIONS_H
#define FORTIFICATIONS_H

#include "GameObjects/Buildings/Building.h"

class Player;

/**
 * @class Fortifications
 * @brief This class is one of the Buildings of the town,
 * which is responsible for providing several enhancements to the town's troops and defences.
 *
 * As player levels up this building, this object will increase the power of army of this town
 * and rate, at which troops are being added to the resources of the town will increase.
 *
 */
class Fortifications : public Building{

private:
    void onLevelUp() override;

public:
    /**
     * @brief Construct a new Fortifications object.
     *
     * @param town_of_building Town, with which this building is associated.
     * @param building_sprites Vector of file paths to the images representing this building in different states (levels).
     *
     */
    Fortifications(Town& town_of_building, std::vector<std::string> building_sprites);
    const std::string getBuildingDescription() const override;
    const ResourceArray& getUpgradeCost() const override;

};

#endif