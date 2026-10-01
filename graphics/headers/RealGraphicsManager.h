/**
 * @file RealGraphicsManager.hpp
 * @author Bartosz Han
 * @brief This file contains RealGraphicsManager class, which is used in GUI version of the app as a interface between game resources and GUI.
 *
 */

#ifndef REAL_GRAPHICS_MANAGER_H
#define REAL_GRAPHICS_MANAGER_H

#include <memory>
#include <map>
#include <QPixmap>
#include <QTimer>
#include <QApplication>
#include <QPointer>

#include "Managers/IGraphicsManager.h"
#include "GameWindow.h"
#include "DrawingTimer.h"
#include "Data/ObjectDataEnums.hpp"
#include "Scenes/Scene.h"

/**
 * @class RealGraphicsManager
 * @brief This class is used in GUI version of the app as a interface between game resources and GUI.
 *
 * This class is responsible for generating and drawing data from the game into display. It is also responsible for managing
 * resources related to the GUI.
 *
 */
class RealGraphicsManager: public IGraphicsManager{

private:
    QApplication* app_;
    GameWindow* game_window_;
    SceneType current_scene_;

    std::map<std::string, std::unique_ptr<QPixmap>> textures_;
    std::unique_ptr<DrawingTimer> drawing_timer_;
    std::vector<std::unique_ptr<Scene>> scenes_;

    int screen_width_;
    int screen_height_;

    int world_map_offset_x_;
    int world_map_offset_y_;

    bool flag_is_drawing_decision_scene_enabled_;
    bool flag_is_ui_active_;
    bool flag_is_ai_verbose_;

    /**
     * @brief Set the layer displayed on the game window to the layer at the given index.
     *
     * @param game_scene Index of the layer to display.
     */
    void setWindowScene(SceneType game_scene);

public:
    RealGraphicsManager();
    GraphicsManagerType getGraphicsManagerType() override;
    QPixmap* getTexture(std::string texture_name) override;
    SceneType getCurrentScene() const override;
    void setUpGameWindow() override;

    GameWindow* getGameWindow() override;
    void showWindow() override;
    int execApp() override;
    void processEvents() override;
    void quitApplication() override;

    void drawCurrentScene() override;
    void setToScene(SceneType new_scene) override;
    int getScreenWidth() const override;
    int getScreenHeight() const override;
    void startDrawing() override;

    void addWorldMapOffsetX(int value) override;
    void addWorldMapOffsetY(int value) override;
    void resetWorldMapOffset() override;
    int getWorldMapOffsetX() const override;
    int getWorldMapOffsetY() const override;
    void setWorldMapOffsetX(int value) override;
    void setWorldMapOffsetY(int value) override;

    bool getFlagIsDrawingDecisionSceneEnabled() const override;
    void setFlagIsDrawingDecisionSceneEnabled(bool new_value) override;

    void enableSpellLayout() override;
    void disableSpellLayout() override;

    bool getFlagIsUIActive() const override;
    void setFlagIsUIActive(bool new_value) override;

    bool getFlagIsAIVerbose() const override;
    void setFlagIsAIVerbose(const bool value) override;


};

#endif