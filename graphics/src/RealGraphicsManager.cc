/**
 * @file RealGraphicsManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class RealGraphicsManager.
 *
 */


#include <QLabel>
#include <QPainter>
#include <QString>
#include <QThread>
#include <memory>
#include <iostream>

#include "RealGraphicsManager.h"
#include "Scenes/WorldDecisionMakingScene.h"
#include "Scenes/WorldScene.h"
#include "Scenes/BattleScene.h"
#include "Scenes/TownMainScene.h"
#include "Scenes/TownTroopBuyingScene.h"
#include "Scenes/TownHeroBuyingScene.h"
#include "Scenes/TownSpellBuyingScene.h"
#include "Scenes/WorldDecisionMakingScene.h"
#include "Scenes/EmptyScene.hpp"

#include "UIConstants.hpp"
#include "Managers/GameManager.h"
#include "Managers/PlayerManager.h"
#include "Data/ObjectDataEnums.hpp"
#include "Miscellaneous/GameExceptions.hpp"

using namespace std;
using namespace ui_constants;
using namespace constants;
using namespace game_data;

RealGraphicsManager::RealGraphicsManager():
    app_(new QApplication(GameManager::getInstance().getArgc(), GameManager::getInstance().getArgv())),
    game_window_(new GameWindow),
    current_scene_(WORLD), drawing_timer_(make_unique<DrawingTimer>()),
    world_map_offset_x_(0), world_map_offset_y_(0),
    flag_is_drawing_decision_scene_enabled_(false),
    flag_is_ui_active_(false),
    flag_is_ai_verbose_(false){};

QPixmap* RealGraphicsManager::getTexture(string texture_name){
    if(textures_.find(texture_name) == textures_.end()){

        QPixmap new_texture(QString::fromStdString(ASSETS_PATH + texture_name));
        if(new_texture.isNull()){
            throw TextureNotFoundError(ASSETS_PATH + texture_name);
        }
        textures_.insert(pair<string , unique_ptr<QPixmap>>(texture_name, make_unique<QPixmap>(new_texture)));
    }
    return textures_.at(texture_name).get();
}

GraphicsManagerType RealGraphicsManager::getGraphicsManagerType(){
    return REAL_GRAPHICS_MANAGER;
}

GameWindow* RealGraphicsManager::getGameWindow(){
    return game_window_;
}

void RealGraphicsManager::setWindowScene(SceneType game_scene){
    getGameWindow()->getUI()->scene_widget->setCurrentIndex(game_scene);
}

void RealGraphicsManager::drawCurrentScene(){

    getGameWindow()->getUI()->scene_widget->setCurrentIndex(current_scene_);
    if(!GameManager::getInstance().isGameFinished()){
        scenes_.at(current_scene_)->drawScene();
    }
    else{
        quitApplication();
    }

}

void RealGraphicsManager::setToScene(SceneType new_scene){
    setWindowScene(new_scene);
    current_scene_ = new_scene;
}

int RealGraphicsManager::getScreenWidth() const{
    return screen_width_;
}

int RealGraphicsManager::getScreenHeight() const{
    return screen_height_;
}

void RealGraphicsManager::startDrawing(){
    drawing_timer_->getTimer().start(FRAME_REDRAWING_TIME);
}

void RealGraphicsManager::setUpGameWindow(){

    game_window_->setWindowFlags(game_window_->windowFlags() & ~Qt::WindowMaximizeButtonHint);
    game_window_->setFixedSize(ui_constants::MAIN_WINDOW_WIDTH, ui_constants::MAIN_WINDOW_HEIGHT);

    screen_width_ = game_window_->geometry().width();
    screen_height_ = game_window_->geometry().height();

    scenes_.push_back(make_unique<WorldScene>());
    scenes_.push_back(make_unique<BattleScene>());
    scenes_.push_back(make_unique<TownMainScene>());
    scenes_.push_back(make_unique<TownTroopBuyingScene>());
    scenes_.push_back(make_unique<TownHeroBuyingScene>());
    scenes_.push_back(make_unique<TownSpellBuyingScene>());
    scenes_.push_back(make_unique<WorldDecisionMakingScene>());
    scenes_.push_back(make_unique<EmptyScene>());
}

void RealGraphicsManager::showWindow(){
    game_window_->show();
}

int RealGraphicsManager::execApp(){
    return app_->exec();
}

void RealGraphicsManager::quitApplication(){

    cout << "#########" << endl;
    cout << "GAME OVER" << endl;
    cout << "#########" << endl;
    vector<int> remaining_players = PlayerManager::getInstance().getRemainingPlayers();
    if(remaining_players.size() ==  1){
        cout << "WINNER:       Player " << remaining_players[0] << endl;
    }
    for (const int& player_id: PlayerManager::getInstance().getGameResults()){
        cout << "ELIMINATED:   Player " << player_id << endl;
    }

    app_->quit();
}

void RealGraphicsManager::processEvents(){
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    QThread::msleep(FRAME_PROCESSING_TIME_MS);
}

void RealGraphicsManager::addWorldMapOffsetX(int value){
    world_map_offset_x_ += value;
}

void RealGraphicsManager::addWorldMapOffsetY(int value){
    world_map_offset_y_ += value;
}

void RealGraphicsManager::resetWorldMapOffset(){
    world_map_offset_x_ = 0;
    world_map_offset_y_ = 0;
}

int RealGraphicsManager::getWorldMapOffsetX() const{
    return world_map_offset_x_;
}

int RealGraphicsManager::getWorldMapOffsetY() const{
    return world_map_offset_y_;
}

void RealGraphicsManager::setWorldMapOffsetX(int value){
    world_map_offset_x_ = value;
}

void RealGraphicsManager::setWorldMapOffsetY(int value){
    world_map_offset_y_ = value;
}

SceneType RealGraphicsManager::getCurrentScene() const{
    return current_scene_;
}

bool RealGraphicsManager::getFlagIsDrawingDecisionSceneEnabled() const{
    return flag_is_drawing_decision_scene_enabled_;
}

void RealGraphicsManager::setFlagIsDrawingDecisionSceneEnabled(bool new_value){
    flag_is_drawing_decision_scene_enabled_ = new_value;
}

void RealGraphicsManager::enableSpellLayout(){
    getGameWindow()->enableSpellLayout();
}

void RealGraphicsManager::disableSpellLayout(){
    getGameWindow()->disableSpellLayout();
}

bool RealGraphicsManager::getFlagIsUIActive() const{
    return flag_is_ui_active_;
}

void RealGraphicsManager::setFlagIsUIActive(bool new_value){
    flag_is_ui_active_ = new_value;
}

void RealGraphicsManager::setFlagIsAIVerbose(const bool value){
    flag_is_ai_verbose_ = value;
}

bool RealGraphicsManager::getFlagIsAIVerbose() const{
    return flag_is_ai_verbose_;
}