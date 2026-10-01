/**
 * @file MockGraphicsManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class MockGraphicsManager.
 *
 */

#include "Managers/MockGraphicsManager.h"


MockGraphicsManager::MockGraphicsManager(){};

GraphicsManagerType MockGraphicsManager::getGraphicsManagerType(){
    return MOCK_GRAPHICS_MANAGER;
}

QPixmap* MockGraphicsManager::getTexture(std::string){
    return nullptr;
}

void MockGraphicsManager::setToScene(SceneType new_scene_type){
    scene_type_ = new_scene_type;
}

SceneType MockGraphicsManager::getCurrentScene() const{
    return scene_type_;
}

GameWindow* MockGraphicsManager::getGameWindow(){
    return nullptr;
}

int MockGraphicsManager::getScreenWidth() const{
    return 0;
}

int MockGraphicsManager::getScreenHeight() const{
    return 0;
}

int MockGraphicsManager::getWorldMapOffsetX() const{
    return 0;
}

int MockGraphicsManager::getWorldMapOffsetY() const{
    return 0;
}

int MockGraphicsManager::execApp(){
    return 0;
}

bool MockGraphicsManager::getFlagIsUIActive() const{
    return false;
}

bool MockGraphicsManager::getFlagIsDrawingDecisionSceneEnabled() const{
    return false;
}

bool MockGraphicsManager::getFlagIsAIVerbose() const{
    return false;
}
