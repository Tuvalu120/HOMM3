/**
 * @file WorldScene.cc
 * @author Bartosz Han
 * @brief This file contains definition of class WorldScene.
 *
 */


#include "Scenes/WorldScene.h"
#include "GameWindow.h"
#include "UIUtilities.hpp"
#include "CanvasTileMapLabel.h"
#include "Managers/GraphicsManager.h"
#include "Managers/WorldManager.h"

using namespace std;
using namespace ui_constants;

WorldScene::WorldScene():
    CanvasTileMapScene(WORLD, GraphicsManager::getInstance().getScreenWidth() / TILE_SIZE * WORLD_MAP_WINDOW_SIZE_RATIO,
    GraphicsManager::getInstance().getScreenHeight() / TILE_SIZE * WORLD_MAP_WINDOW_SIZE_RATIO){
    for (int i=0 ; i<getCanvasWidth() * getCanvasHeight() ; ++i){
        canvas_tiles_.push_back(make_unique<WorldCanvasTileMapLabel>(*this));
    }

    for (int i = 0; i < getCanvasWidth(); ++i){
        for(int j=0; j < getCanvasHeight(); ++j){
            getCanvasTileAt(i, j).setFixedHeight(64);
            getCanvasTileAt(i, j).setFixedWidth(64);
            GraphicsManager::getInstance().getGameWindow()->getUI()->world_tile_map->addWidget(&getCanvasTileAt(i, j), j, i);
        }
    }
}

std::pair<int, int> WorldScene::calculateInitDrawingPos() const{
        WorldMap& world = WorldManager::getInstance().getWorldMap();
        int current_army_x_pos = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getXPos();
        int current_army_y_pos = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getYPos();
        int init_drawing_x_pos = current_army_x_pos - (getCanvasWidth() / 2) + GraphicsManager::getInstance().getWorldMapOffsetX();
        int init_drawing_y_pos = current_army_y_pos - (getCanvasHeight() / 2) + GraphicsManager::getInstance().getWorldMapOffsetY();

        init_drawing_x_pos = max(init_drawing_x_pos, 0);
        init_drawing_y_pos = max(init_drawing_y_pos, 0);

        init_drawing_x_pos = min(init_drawing_x_pos, world.getTileMapWidth()-getCanvasWidth());
        init_drawing_y_pos = min(init_drawing_y_pos, world.getTileMapHeight()-getCanvasHeight());

        return pair<int, int>(init_drawing_x_pos, init_drawing_y_pos);
}

pair<int, int> WorldScene::findTileMapPosOfCanvasTile(QLabel &tile) const{

    for (int i = 0; i< getCanvasWidth(); ++i){
        for(int j = 0; j < getCanvasHeight(); j++){
            if (&getCanvasTileAt(i, j) == &tile){
                pair<int,int> offset = calculateInitDrawingPos();
                return pair<int,int>(i+offset.first,j+offset.second);
            }
        }
    }
    return pair<int,int>(-1,-1);
}

void WorldScene::drawScene(){

    WorldMap& world = WorldManager::getInstance().getWorldMap();

    const auto& game_ui = GraphicsManager::getInstance().getGameWindow()->getUI();

    pair<int, int> init_drawing_pos = calculateInitDrawingPos();


    //Update offsets of world map, if they would cause drawing outside the world map.

    WorldArmy& current_army = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy();

    if(init_drawing_pos.first == 0 || init_drawing_pos.first == world.getTileMapWidth()-getCanvasWidth()){
        int new_offset_x = init_drawing_pos.first + (getCanvasWidth() / 2) - current_army.getXPos();
        GraphicsManager::getInstance().setWorldMapOffsetX(new_offset_x);
    }

    if(init_drawing_pos.second == 0 || init_drawing_pos.second == world.getTileMapHeight()-getCanvasHeight()){
        int new_offset_y = init_drawing_pos.second + (getCanvasHeight() / 2) - current_army.getYPos();
        GraphicsManager::getInstance().setWorldMapOffsetY(new_offset_y);
    }

    ////

    for (int i = 0; i< getCanvasWidth(); ++i){
        for(int j = 0; j < getCanvasHeight(); ++j){
            Tile& current_tile = world.getTileAt(i+init_drawing_pos.first, j+init_drawing_pos.second);
            if(WorldManager::getInstance().getCurrentPlayer().hasTileBeenVisited(
                    pair<int, int>(current_tile.getXPos(), current_tile.getYPos()))){
                drawOnTile(current_tile, i, j);
            }
            else{
                getCanvasTileAt(i, j).setPixmap(*GraphicsManager::getInstance().getTexture(FOG_OF_WAR_IMAGE));
            }
            getCanvasTileAt(i, j).show();
        }
    }


    drawArmyData(game_ui->world_troop_layout, game_ui->world_resource_label, game_ui->world_spell_layout);
    drawHeroData(game_ui->world_hero_ui);
    setTextOnLabel(GraphicsManager::getInstance().getGameWindow()->getUI()->hero_level_info,
        WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().getHeroLevelDescription());
}
