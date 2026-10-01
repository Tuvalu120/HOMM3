/**
 * @file BattleObject.h
 * @author Bartosz Han
 * @brief This file contains BattleObject class, which is a class used for representing objects actively participating in the battle.
 *
 */
#ifndef BATTLE_OBJECT_H
#define BATTLE_OBJECT_H

#include <string>
#include <vector>

#include "GameObjects/BaseObjects/OwnedByPlayer.h"
#include "Data/ObjectDataEnums.hpp"

class Tile;
class DrawableObject;


/**
 * @class BattleObject
 * @brief Abstract base class representing an object that participates in battles.
 *
 * This class represents objects, which take turns during battle, and can do some action during it.
 * During turn of battle object, player can steer this object and do actions with it, using method attack().
 * The order of action is determined by battle priority of battle objects.
 *
 * This class also has few methods for displaying battle object data into UI.
 *
 * This class is mainly used by stacks and heroes.
*/
class BattleObject: public OwnedByPlayer{

private:
    const ObjectType object_type_;
    const int battle_priority_;

public:
    /**
     * @brief Construct a new BattleObject object.
     *
     * @param object_type Type of newly created battle object.
     * @param player_id ID of player owning this object.
     * @param battle_priority Battle queue's priority of the object.
     */
    BattleObject(ObjectType object_type, const int player_id, const int battle_priority);

    /**
     * @brief Do some action relation to the battle, with target designated at provided tile.
     *
     * Tile is used to determine, which type of action will take - based on the object,
     * it can be going to tile, dealing damage to stack on tile, or casting spell with target as this tile.

     * @param event_tile Tile&, which is used as a destination oor target of the action.
     * @return true if action was performed successfully, false is something wrong happen (for example, no stack is on the tile).
     */
    virtual bool attack(Tile& event_tile) = 0;

    /**
     * @brief Get battle priority of object.
     *
     * Battle priority determines order of objects to take its turns during the battle.
     * The lower the value, the faster will this object perform its action.
     *
     * @return int Battle priority of object.
     */
    int getBattlePriority() const;

    /// @brief Compare two battle objects, based on their battle priority.
    bool operator<(const BattleObject& other) const;

    /**
     * @brief get file path to the texture to be shown on UI.
     *
     * @return std::string file path to the texture to be shown on UI.
     */
    virtual const std::string getUITextureName() const = 0;

    /**
     * @brief Get the battle data of the object, as a lists of strings.
     *
     * This method will return two vectors of string: first vector has names of statistics to dis played,
     * and second vector has values of these statistics.
     *
     * @return const std::pair<std::vector<std::string>, std::vector<std::string>> Data of battle object to be displayed in GUI.
     */
    virtual const std::pair<std::vector<std::string>, std::vector<std::string>> getDataText() const = 0;

    /**
     * @brief Get short description of the object.
     *
     * The description will contain most important data about object, mainly its type, ID, and owner of this object.
     *
     * @return const std::string Description of the object.
     */
    virtual const std::string toString() const = 0;

    /**
     * @brief Get ObjectType ID of this object.
     *
     * @return ObjectType Type of this object.
     */
    ObjectType getBattleObjectType() const;

    BattleObject(const BattleObject&) = delete;
    BattleObject& operator=(const BattleObject&) = delete;

};

#endif