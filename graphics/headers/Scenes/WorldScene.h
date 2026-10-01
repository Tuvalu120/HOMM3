/**
 * @file WorldScene.h
 * @author Bartosz Han
 * @brief This file contains WorldScene class, which is used for displaying state of the game of the when player is exploring the world map.
 */

#ifndef WORLD_SCENE_H
#define WORLD_SCENE_H

#include <QLabel>

#include "Scenes/Scene.h"

/**
 * @class WorldScene
 * @brief This class is used for displaying state of the game of the when player is exploring the world map.
 *
 */
class WorldScene: public CanvasTileMapScene{

public:
    WorldScene();
    void drawScene() override;
    std::pair<int, int> findTileMapPosOfCanvasTile(QLabel& tile) const override;


    /**
     * @brief Calculates the initial drawing position for the scene.
     * The position returned by this method is a world map coordinates
     * and indicates which world map tile should be represented at canvas tile at (0, 0).
     *
     * This function determines the starting coordinates (x, y) for displaying
     * elements within the world scene, with respect to current army's world position and offset given by player and stoer in graphics manager.
     *
     * @return std::pair<int, int> The coordinates representing initial drawing position.
     * Coordinates given by this function determines, which tile at world map should be represented by canvas tile at position (0, 0).
     */
    std::pair<int, int> calculateInitDrawingPos() const;

};

#endif