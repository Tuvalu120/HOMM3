/**
 * @file CanvasTileMapLabel.h
 * @author Bartosz Han
 * @brief This file contains class CanvasTileMapLabel, which is a QLabel representing one tile on canvas map of CanvasTileMapScene.
 *
 */
#ifndef CANVAS_TILE_MAP_LABEL_H
#define CANVAS_TILE_MAP_LABEL_H

#include <QLabel>
#include <QMouseEvent>

#include "Scenes/Scene.h"

/**
 * @class CanvasTileMapLabel
 * @brief This is a QLabel representing one tile on canvas map of CanvasTileMapScene.
 *
 */
class CanvasTileMapLabel : public QLabel
{
    Q_OBJECT

protected:

    CanvasTileMapScene& tile_scene_;

    /**
     * @brief Do the logic regarding player clicking this tile.
     *
     * This method will translate this tile position on canvas map into TileMap tile, which currently associated with this tile,
     * and will set decision variables of player, which is currently taking turns, into coordinates of TileMap tile.
     *
     */
    virtual void mousePressEvent(QMouseEvent *event) override = 0;

public:

    /**
     * @brief Construct a new CanvasTileMapLabel object.
     *
     * @param scene CanvasTileMapScene& which will contain this label.
     */
    explicit CanvasTileMapLabel(CanvasTileMapScene& scene, QWidget* parent = nullptr);

};

/**
 * @class WorldCanvasTileMapLabel
 * @brief This is a QLabel representing one tile on canvas map of WorldScene.
 *
 */
class WorldCanvasTileMapLabel : public CanvasTileMapLabel{

protected:
    void mousePressEvent(QMouseEvent *event) override;

public:

    /**
     * @brief Construct a new WorldCanvasTileMapLabel object.
     *
     * @param scene CanvasTileMapScene& which will contain this label.
     */
    explicit WorldCanvasTileMapLabel(CanvasTileMapScene& scene, QWidget* parent = nullptr);

};

/**
 * @class BattleCanvasTileMapLabel
 * @brief This is a QLabel representing one tile on canvas map of BattleScene.
 *
 */
class BattleCanvasTileMapLabel : public CanvasTileMapLabel{

protected:
    void mousePressEvent(QMouseEvent *event) override;

public:
    /**
     * @brief Construct a new BattleCanvasTileMapLabel object.
     *
     * @param scene CanvasTileMapScene& which will contain this label.
     */
    explicit BattleCanvasTileMapLabel(CanvasTileMapScene& scene, QWidget* parent = nullptr);

};

#endif