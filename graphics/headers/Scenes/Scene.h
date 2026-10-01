/**
 * @file Scene.h
 * @author Bartosz Han
 * @brief This file contains Scene and CanvasTileMapScene class declarations.
 * Scene class is responsible for displaying GUI representing current state of the game.
 *
 * During the game, player and their armies might, while interacting with components the game,
 * change the state of the game. Armies notably can explore world
 * map, battle with another army, or visit town. Scene class is responsible for
 * displaying one of the states of the game. CanvasTileMapScene is a special type of scene,
 * which is responsible for displaying locations which uses tile maps (mainly world scene or battle scene).
 *
 */

#ifndef SCENE_H
#define SCENE_H

#include <QLabel>

#include "Data/ObjectDataEnums.hpp"
#include "GameObjects/TileMaps/Tile.h"

/**
 * @class Scene
 * @brief This class is responsible for displaying GUI representing current state of the game.
 *
 * During the game, player and their armies might, while interacting with components the game, change the state of the game.
 * Armies notably can explore world map, battle with another army, or visit town.
 * Scene class is responsible for displaying one of the states of the game.
 *
 */
class Scene{

private:
    SceneType scene_type_;

public:

    /**
     * @brief Construct a new Scene object
     *
     * @param scene_type SceneType enum, which indicates which type of scene is being created.
     */
    explicit Scene(SceneType scene_type);

    /**
     * @brief Show data into display, regarding current state of the game.
     *
     */
    virtual void drawScene() = 0;

    /// @brief Get the SceneType of Scene
    SceneType getSceneType() const;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
};
//////

/**
 * @class CanvasTileMapScene
 * @brief This class is represents type of Scene, which is for displaying game states which are represented by tile map.
 *
 * This class, in addition to normal Scene, contains vector of QLabels, each representing one tile on certain TileMap.
 *
 * @note Tiles in canvas_tiles_ vector are ordered by lowest y pos first, and later by lowest x pos.
 *
 */
class CanvasTileMapScene: public Scene{

protected:
    std::vector<std::unique_ptr<QLabel>> canvas_tiles_;
    int canvas_width_;
    int canvas_height_;

    /**
     * @brief Draw data of Tile into specific canvas tile of this scene.
     *
     * This method varies between scenes, as different scene might have different logic and data needed to draw on canvas tile.
     *
     * @param tile_to_draw Game Tile&, whose data should be drawn
     * @param canvas_tile_x_pos x position of canvas tile, on which data should be drawn.
     * @param canvas_tile_y_pos y position of canvas tile, on which data should be drawn.
     *
     * @note tile_to_draw is independent from canvas_tile, and tile_to_draw's position on game map is not used in drawing data into this scene.
     */
    virtual void drawOnTile(Tile& tile_to_draw, const int canvas_tile_x_pos, const int canvas_tile_y_pos);

public:

    /**
     * @brief Construct a new CanvasTileMapScene object
     *
     * @param scene_type SceneType enum, which indicates which type of scene is being created.
     * @param canvas_width Width of tile map, as a number of tiles.
     * @param canvas_height Height of tile map, as a number of tiles.
     */
    CanvasTileMapScene(SceneType scene_type, const int canvas_width, const int canvas_height);

    /**
     * @brief Get the pair, representing tile's coordinates on game tile map (not canvas tile map), which is associated with given QLabel.
     *
     * @param tile QLabel, of which position on game tile map should be given.
     * @return std::pair<int, int> Position of QLabel on game tile map.
     */
    virtual std::pair<int, int> findTileMapPosOfCanvasTile(QLabel& tile) const = 0 ;

    /**
     * @brief Get the QLabel& object, which is used as a tile at given coordinates in canvas tile map.
     *
     * @param x X position canvas tile to search.
     * @param y Y position canvas tile to search.
     * @return QLabel& representing tile at given coordinates in canvas map.
     */
    QLabel& getCanvasTileAt(const int x, const int y) const;

    /// @brief Get the height of canvas map, as a number of tiles.
    int getCanvasHeight() const;

    /// @brief Get the width of canvas map, as a number of tiles.
    int getCanvasWidth() const;

    /**
     * @brief Generates a QPixmap representation of a given Tile.
     *
     * This function creates a QPixmap based on the provided Tile object.
     * Optionally, a specific texture can be additionally applied to draw upon the tile,
     * image can be flipped horizontally, and a darkening effect can be applied.
     *
     * @param tile The Tile object to generate the pixmap from.
     * @param object_texture_name Optional, additional texture to use, as a path to image.
     * @param flip_horizontally If true, the resulting pixmap will be flipped horizontally.
     * @param darken_factor A value between 0.0f (no darkening) and 1.0f (completely dark).
     * @return QPixmap The generated pixmap representing the tile.
     */
    static QPixmap generateTilePixmap(const Tile& tile, std::string object_texture_name = "",
        bool flip_horizontally = false, float darken_factor = 0.0f);

};
#endif