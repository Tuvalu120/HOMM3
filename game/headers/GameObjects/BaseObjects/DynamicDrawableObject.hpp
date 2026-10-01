/**
 * @file DynamicDrawableObject.hpp
 * @author Bartosz Han
 * @brief This file contains DynamicDrawableObject, which is used for objects, which should have associated several textures to it,
 * and should have easy access to other textures.
 *
 */

#ifndef DYNAMIC_DRAWABLE_OBJECT_H
#define DYNAMIC_DRAWABLE_OBJECT_H

#include "GameObjects/BaseObjects/DrawableObject.h"
#include <vector>
#include <string>

/**
 * @class DynamicDrawableObject
 * @brief This abstract class represents objects, which should have more than one associated texture,
 * and should have easy access to them.
 *
 * The key difference between this class and DrawableObject is that is class, instead of storing the path to texture,
 * stores vector of them.
 *
 */
class DynamicDrawableObject : public DrawableObject{

private:
    const std::vector<std::string> textures_;

protected:
    /**
     * @brief Get the file path to texture at given index of vector of textures.
     *
     * @param texture_index Index of texture path at textures vector.
     * @return std::string File path to texture at given index.
     */
    const std::string getTextureAt(const int texture_index) const{
        return textures_.at(texture_index);
    }

public:
    /**
     * @brief Construct a new DynamicDrawableObject.
     *
     * @param initial_textures Vector of textures associated with object.
     */
    DynamicDrawableObject(const std::vector<std::string> initial_textures): textures_(initial_textures){};

    /**
     * @brief Construct a DynamicDrawableObject by coping list of textures of different DynamicDrawableObject.
     *
     * @param other DynamicDrawableObject to copy list of textures from.
     */
    DynamicDrawableObject(const DynamicDrawableObject& other) : DrawableObject(), textures_(other.textures_){};

    const std::string getCurrentTextureName() const override = 0;

};

#endif