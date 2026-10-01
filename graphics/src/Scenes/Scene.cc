/**
 * @file Scene.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Scene.
 *
 */


#include <QPainter>
#include <QLabel>

#include "Scenes/Scene.h"
#include "Managers/GraphicsManager.h"
#include "GameObjects/TileMaps/Tile.h"
#include "Painter.h"

using namespace std;

Scene::Scene(SceneType scene_type):scene_type_(scene_type){};

SceneType Scene::getSceneType() const{
    return scene_type_;
}

CanvasTileMapScene::CanvasTileMapScene(SceneType scene_type, int canvas_width, int canvas_height):
    Scene(scene_type),
    canvas_width_(canvas_width),canvas_height_(canvas_height){}

int CanvasTileMapScene::getCanvasHeight() const{
    return canvas_height_;
}

int CanvasTileMapScene::getCanvasWidth() const{
    return canvas_width_;
}

QLabel& CanvasTileMapScene::getCanvasTileAt(const int x, const int y) const{
    return *canvas_tiles_.at(x + y * canvas_width_);
}

void CanvasTileMapScene::drawOnTile(Tile& tile_to_draw,
    const int canvas_tile_x_pos, const int canvas_tile_y_pos){

        QPixmap pixmap;

        if (tile_to_draw.hasInteractiveObject()){
            pixmap = CanvasTileMapScene::generateTilePixmap(tile_to_draw, tile_to_draw.getObjectOnTile().getCurrentTextureName());
            Painter painter(pixmap);
            tile_to_draw.getObjectOnTile().drawAdditionalData(painter);
        }

        else{
            pixmap = CanvasTileMapScene::generateTilePixmap(tile_to_draw, tile_to_draw.getAdditionalTextureName());
        }

        getCanvasTileAt(canvas_tile_x_pos, canvas_tile_y_pos).setPixmap(pixmap);
}

QPixmap CanvasTileMapScene::generateTilePixmap(const Tile& tile, string object_texture_name, bool flip_horizontally, float darken_factor){
    QPixmap tile_pixmap(*GraphicsManager::getInstance().getTexture(tile.getCurrentTextureName()));

    if(object_texture_name != ""){

        QPixmap object_pixmap(*GraphicsManager::getInstance().getTexture(object_texture_name));

        if(flip_horizontally){
            object_pixmap = object_pixmap.transformed(QTransform().scale(-1,1));
        }

        QPainter painter(&tile_pixmap);
        painter.drawPixmap(0, 0, object_pixmap);
        painter.fillRect(tile_pixmap.rect(), QColor(0, 0, 0, darken_factor * 255));

        return tile_pixmap;
    }
    else{
        return tile_pixmap;
    }
}
