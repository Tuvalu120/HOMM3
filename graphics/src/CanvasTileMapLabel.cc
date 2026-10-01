/**
 * @file CanvasTileMapLabel.cc
 * @author Bartosz Han
 * @brief This file contains definition of class CanvasTileMapLabel.
 *
 */


#include "CanvasTileMapLabel.h"
#include "Managers/WorldManager.h"
#include "Managers/GraphicsManager.h"
#include "Managers/BattleManager.h"


using namespace std;

CanvasTileMapLabel::CanvasTileMapLabel(CanvasTileMapScene& scene, QWidget* parent):
QLabel(parent), tile_scene_(scene){};

///

WorldCanvasTileMapLabel::WorldCanvasTileMapLabel(CanvasTileMapScene& scene, QWidget* parent):
    CanvasTileMapLabel(scene, parent){};

void WorldCanvasTileMapLabel::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::LeftButton &&
        WorldManager::getInstance().getCurrentPlayer().getPlayerController().getControlType() == HUMAN){

        pair<int,int> world_pos = tile_scene_.findTileMapPosOfCanvasTile(*this);
        GraphicsManager::getInstance().resetWorldMapOffset();
        WorldManager::getInstance().getCurrentPlayer().getPlayerController().setDecisionVars(
            world_pos.first, world_pos.second);
    }
};

///

BattleCanvasTileMapLabel::BattleCanvasTileMapLabel(CanvasTileMapScene& scene, QWidget* parent):
    CanvasTileMapLabel(scene, parent){};

void BattleCanvasTileMapLabel::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton &&
        GraphicsManager::getInstance().getFlagIsUIActive()){

        pair<int,int> battle_pos = tile_scene_.findTileMapPosOfCanvasTile(*this);
        BattleManager::getInstance().getCurrentBattle().getCurrentBattleController().setDecisionVars(
            battle_pos.first, battle_pos.second);

    }
}