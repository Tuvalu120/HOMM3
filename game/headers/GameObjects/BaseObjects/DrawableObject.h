/**
 * @file DrawableObject.h
 * @author Bartosz Han
 * @brief This file constains DrawableObject class, which is a class for objects, which should have some kind of texture associated with it.
 *
 */
#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include <string>

/**
 * @class DrawableObject
 * @brief This is an class used by the object, which should have some texture, icon or image associated with it.
 * This class contains string storing data about current texture, and contains getters and setters or this string.
 *
 */
class DrawableObject{

protected:
    DrawableObject(){};
    std::string current_texture_name_;

public:
    /**
     * @brief Construct a new DrawableObject.
     * @param initial_texture_name File path of texture, icon or image to be associated with this object.
     */
    explicit DrawableObject(const std::string initial_texture_name);

    /**
     * @brief Set new file path to new texture.
     *
     * @param new_texture_name File path of new texture.
     */
    void setCurrentTextureName(const std::string new_texture_name);

    /// @brief Get file path to texture currently associated with this object.
    virtual const std::string getCurrentTextureName() const;

};

#endif