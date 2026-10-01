/**
 * @file TownMainScene.h
 * @author Bartosz Han
 * @brief This file contains TownMainScene class, which is used for displaying state of the game of the when army entered the town,
 * and is deciding upon upgrading buildings of town, or buying resources to army.
 *
 */

#ifndef TOWN_MAIN_SCENE_H
#define TOWN_MAIN_SCENE_H

#include "Scenes/Scene.h"

/**
 * @class TownMainScene
 * @brief This class is used for displaying state of the game of the when army entered the town,
 * and is deciding upon upgrading buildings of town, or buying resources to army.
 *
 */
class TownMainScene: public Scene{
public:
    TownMainScene();
    void drawScene() override;
};

#endif