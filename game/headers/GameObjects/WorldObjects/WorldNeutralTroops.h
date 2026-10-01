/**
 * @file WorldNeutralTroops.h
 * @author Bartosz Han
 * @brief This file contains WorldNeutralTroops class, which is one of WorldGeneratedInteractiveObjects.
 * Players can interact with this object, in order to either fight with army to get additional experience points,
 * or to buy new troops to the army.
 *
 */
#ifndef WORLD_NEUTRAL_TROOPS_H
#define WORLD_NEUTRAL_TROOPS_H

#include "GameObjects/BaseObjects/WorldGeneratedInteractiveObject.h"
#include "GameObjects/Army.h"
#include "Miscellaneous/IPainter.h"

class QPixmap;

/**
 * @class WorldNeutralTroops
 * @brief This class is one of WorldGeneratedInteractiveObjects. Players can interact with this object,
 * in order to either fight with army to get additional experience points, or to buy new troops to the army.
 *
 * This class contains army, which consists of only one type of troops. The power of armies generated grows as the game progress.
 * After either defeating army of this object, or buying this army, this object is removed from world map.
 *
 * WorldNeutralTroops can only be visited once per turn of player. Player, upon interacting with this object, can choose to skip making decision.
 *
 */
class WorldNeutralTroops : public WorldGeneratedInteractiveObject{
    private:
    std::shared_ptr<Army> object_army_;
    bool was_army_visited_this_turn_;
    int power_;

    public:
    WorldNeutralTroops();

    void interact() override;

    /// @brief Get reference to the Army object, with which player can fight.
    Army& getObjectArmy() const;

    /// @brief Create army of this object, with provided army power.
    /// Army will be created only if this object does not have any army associated with it, and will contain only one type of troop.
    void setUpArmy(const int army_power);

    /// @brief Set the was_army_visited_this_turn_ to true.
    /// This object can only be interacted once per army turn. If player decided to interact with this object, but decided to no do any action, this flag is set to true.
    void resetVisitFlag();

    void drawAdditionalData(IPainter& painter) const override;
};

#endif
