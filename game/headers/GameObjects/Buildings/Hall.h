/**
 * @file Hall.h
 * @author Bartosz Han
 * @brief This file contains Hall class, which is one of the buildings of the town,
 * which is responsible for providing player with additional Resources.
 *
 */

#ifndef HALL_H
#define HALL_H

#include "GameObjects/Buildings/Building.h"

class Player;

/**
 * @class Hall
 * @brief This class is one of the Buildings of the town,
 * which is responsible for providing player with additional Resources.
 *
 * This object should provide player with small amount of gold each day.
 * As player levels up this building, this object will provide more gold to the player.
 *
 */
class Hall : public Building{

private:
    void onLevelUp() override;
    /**
     * @brief Check, if other building of the town of this Hall have enough level to level up this building,
     * based on level on level of this building.
     *
     * Hall building has unique requrirements to when it can be levelled up.
     * It needs several other building of this town to be at least at appropiate level to enable
     * leveling pu this building. This method will check, if town has all required building at given level.
     *
     * @return True if all other buildings have meet the criteria of level up this building, false otherwise.
     *
     */
    bool checkBuildingRequirements();

public:
    /**
     * @brief Construct a new Hall object.
     *
     * @param town_of_building Town, with which this building is associated.
     * @param building_sprites Vector of file paths to the images representing this building in different states (levels).
     *
     */
    Hall(Town& town_of_building, std::vector<std::string> building_sprites);
    bool canLevelUp() override;
    const std::string getBuildingDescription() const override;
    const ResourceArray& getUpgradeCost() const override;


};

#endif