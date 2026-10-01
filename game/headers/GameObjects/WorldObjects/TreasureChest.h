/**
 * @file TreasureChest.h
 * @author Bartosz Han
 * @brief This file contains TreasureChest class, which is one of WorldGeneratedInteractiveObjects.
 * Player can interact with this object, in order to get additional resources, or experience points.
 *
 */

#ifndef TREASURE_CHEST_H
#define TREASURE_CHEST_H

#include "GameObjects/BaseObjects/WorldGeneratedInteractiveObject.h"

/**
 * @class TreasureChest
 * @brief This class is one of WorldGeneratedInteractiveObjects.
 * Player can interact with this object, in order to get additional resources, or experience points.
 *
 * Upon interacting, player can decide if it wants to get additional gold (treasure chests always gives player gold), or experience points.
 *
 */
class TreasureChest : public WorldGeneratedInteractiveObject{
    public:
    TreasureChest();
    void interact() override;

};
#endif