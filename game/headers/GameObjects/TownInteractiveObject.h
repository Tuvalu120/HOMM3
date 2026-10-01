/**
 * @file TownInteractiveObject.h
 * @author Bartosz Han
 * @brief This file contains TownInteractiveObject, which is an InteractiveObject, which is present at world map, and with which players can
 * interacts with Towns.
 *
 */

#ifndef TOWNINTERACTIVEOBJECT_H
#define TOWNINTERACTIVEOBJECT_H

#include "GameObjects/BaseObjects/InteractiveObject.h"
#include "GameObjects/Town.h"
#include "Miscellaneous/IPainter.h"

class QPixmap;

/**
 * @class TownInteractiveObject
 * @brief This class is a type of InteractiveObject, which is present at world map, and with which players can
 * interacts with Towns.
 *
 * Objects of this class are always associated with one town. When player interacts with one of this object,
 * it will either visit town (if event player is friendly), and initialize battle (if event player is hostile).
 *
 */
class TownInteractiveObject : public InteractiveObject, public OwnedByPlayer{
private:
    Town& town_of_object_;
    const int object_inner_pos_;
    const std::vector<std::string> town_texture_names_;

public:


    /**
     * @brief Constructs a new TownInteractiveObject.
     *
     * @param town_of_object Reference to the Town object this object is associated with.
     * @param texture_names A vector of texture file names used to render the object on the world map.
     * @param x_pos X-coordinate position of the object on the world map.
     * @param y_pos Y-coordinate position of the object on the world map.
     * @param object_pos Identifier of the object within the town.
     */
    TownInteractiveObject(Town& town_of_object, std::vector<std::string> texture_names, const int x_pos, const int y_pos, const int object_pos);

    void interact() override;
    void drawAdditionalData(IPainter& painter) const override;
    const std::string getCurrentTextureName() const override;
    int getPlayerID() const override;

    /// @brief get reference to the Town object associated with this object.
    const Town& getTownOfObject() const;

};

#endif