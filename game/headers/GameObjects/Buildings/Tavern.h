/**
 * @file Tavern.h
 * @author Bartosz Han
 * @brief This file contains Tavern class, which is one of the buildings of the town,
 * which is responsible for providing player with new heroes and armies to buy.
 *
 */

#ifndef TAVERN_H
#define TAVERN_H

#include "GameObjects/Buildings/Building.h"

class Player;

/**
 * @class Tavern
 * @brief This class is one of the Buildings of the town,
 * which is responsible for providing player with new heroes and armies to buy.
 *
 * This building should only be upgraded once. Upon level up, this building will provide player with
 * new heroes to buy. Buying new hero will create new world army for player to command.
 *
 * There can only certain number of heroes to be available to buy in tavern. Each new week,
 * old heroes in town to buy will be replaced by new ones.
 *
 */
class Tavern : public Building{

private:
    void onLevelUp() override;

public:
    /**
     * @brief Construct a new Tavern object.
     *
     * @param town_of_building Town, with which this building is associated.
     * @param building_sprites Vector of file paths to the images representing this building in different states (levels).
     *
     */
    Tavern(Town& town_of_building, std::vector<std::string> building_sprites);
    const std::string getBuildingDescription() const override;
    const ResourceArray& getUpgradeCost() const override;

};

#endif