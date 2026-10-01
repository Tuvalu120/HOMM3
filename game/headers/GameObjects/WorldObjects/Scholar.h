/**
 * @file Scholar.h
 * @author Bartosz Han
 * @brief This file contains Scholar class, which is one of WorldGeneratedInteractiveObjects.
 * Player can interact with this object, in order to increase statistics of army's hero, or give it new spells to learn.
 *
 */

#ifndef SCHOLAR_H
#define SCHOLAR_H

#include "GameObjects/BaseObjects/WorldGeneratedInteractiveObject.h"

/**
 * @class TreasureChest
 * @brief This class is one of WorldGeneratedInteractiveObjects.
 * Player can interact with this object, in order to increase statistics of army's hero, or give it new spells to learn.
 *
 * Upon interacting, player can choose from increase randomly chosen hero's statistic, increase level of one of spells, which hero currently have,
 * or make hero learn new spell, which hero does not currently have.
 *
 */
class Scholar: public WorldGeneratedInteractiveObject{
    public:
    Scholar();
    void interact() override;

};

#endif