/**
 * @file BattleScene.h
 * @author Bartosz Han
 * @brief This file contains BattleScene class, which is used for displaying state of the game of the when two armies are battling each other.
 *
 */

#ifndef BATTLE_SCENE_H
#define BATTLE_SCENE_H

#include <QLabel>

#include "Scenes/Scene.h"
#include "GameObjects/TileMaps/Tile.h"

/**
 * @class BattleScene
 * @brief This class used for displaying state of the game of the when two armies are battling each other.
 *
 */
class BattleScene: public CanvasTileMapScene{

public:
    BattleScene();
    void drawScene() override;
    std::pair<int, int> findTileMapPosOfCanvasTile(QLabel& tile) const override;
    void drawOnTile(Tile& tile_to_draw, const int canvas_tile_x_pos, const int canvas_tile_y_pos) override;

};

#endif