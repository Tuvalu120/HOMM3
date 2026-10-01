/**
 * @file WorldResource.h
 * @author Bartosz Han
 * @brief This contains WorldResource class, which is one of WorldGeneratedInteractiveObjects.
 * Players can get resources, when iteracting with objects of this class.
 *
 */
#ifndef WORLD_RESOURCE_H
#define WORLD_RESOURCE_H

#include "GameObjects/BaseObjects/WorldGeneratedInteractiveObject.h"
#include "Data/ObjectDataEnums.hpp"

class GameManager;

/**
 * @class WorldResource
 * @brief This class is one of WorldGeneratedInteractiveObjects. Players can get resources, when iteracting with objects of this class.
 *
 * When player interacts with this object, it receives certain amount of one type of resource. After that, this object is removed from world map.
 *
 */
class WorldResource : public WorldGeneratedInteractiveObject{
    private:
    unsigned int quantity_;
    const Resource resource_type_;

    /// @brief Set quantity of resources, which object will give to player upon interacting
    /// @param value New quantity of resources.
    void setQuantity(const int value);


    public:
    /**
     * @brief Construct a new World Resource object
     *
     * @param initial_texture_name File path to texture of resource, which will be shown on world map.
     * @param resource_type Type of resource, which this object will give player upon interacting.
     * @param quantity Amount of resources, which this object will give player upon interacting.
     */
    WorldResource(const std::string initial_texture_name,
                    const Resource resource_type, const int quantity);
    WorldResource();

    /// @brief Get resource type, which this object is giving the player.
    Resource getResourceType() const;

    /// @brief Get amount of resources, which this object is giving the player.
    unsigned int getQuantity() const;
    void interact() override;

    /// @brief check, if two objects have same position and are giving player same type of resource with same amount.
    bool operator==(const WorldResource& other) const;

};

#endif