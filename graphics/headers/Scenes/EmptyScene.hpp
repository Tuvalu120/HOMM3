/**
 * @file EmptyScene.hpp
 * @author Bartosz Han
 * @brief This file contains EmptyScene class,
 * which represents scene in game during state, when no
 * other scene should be drawn.
 *
 */

#include "Data/ObjectDataEnums.hpp"
#include "Scene.h"

#ifndef EMPTY_SCENE_HPP
#define EMPTY_SCENE_HPP

/**
 * @brief This class represents scene in game during state, when no
 * other scene should be drawn.
 *
 */
class EmptyScene: public Scene{
    public:
    EmptyScene(): Scene(EMPTY_SCENE){};
    void drawScene() override {};
};

#endif