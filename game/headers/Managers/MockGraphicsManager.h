/**
 * @file MockGraphicsManager.hpp
 * @author Bartosz Han
 * @brief This file contains MockGraphicsManager class, which is used in non-GUI version of the app to mock behaviour of GraphicsManager.
 *
 */

#ifndef MOCK_GRAPHICS_MANAGER_H
#define MOCK_GRAPHICS_MANAGER_H

#include "Managers/IGraphicsManager.h"

/**
 * @class MockGraphicsManager
 * @brief This class is a non-GUI verion of graphic manager and it is used to mock behaviour of real GraphicsManager.
 *
 * @note Methods of this class, which exceptions of setToScene() and getCurrentScene(), do nothing and return values of these values should be ignored.
 * The two mentioned methods work in such way that they are getter and setter of current scene type variable value.
 */
class MockGraphicsManager: public IGraphicsManager{

private:

SceneType scene_type_;

public:

    MockGraphicsManager();
    void setUpGameWindow() override{};
    void processEvents() override{};
    void drawCurrentScene() override{};
    void startDrawing() override{};
    void addWorldMapOffsetX(int) override{};
    void addWorldMapOffsetY(int) override{};
    void resetWorldMapOffset() override{};
    void showWindow() override{};
    void quitApplication() override{};
    void setWorldMapOffsetX(int) override{};
    void setWorldMapOffsetY(int) override{};
    void setFlagIsDrawingDecisionSceneEnabled(bool) override{};
    void enableSpellLayout() override{};
    void disableSpellLayout() override{};
    void setFlagIsUIActive(bool) override{};
    void setFlagIsAIVerbose(const bool) override{};


    GraphicsManagerType getGraphicsManagerType() override;
    QPixmap* getTexture(std::string) override;
    void setToScene(SceneType new_scene_type) override;
    SceneType getCurrentScene() const override;
    GameWindow* getGameWindow() override;
    int getScreenWidth() const override;
    int getScreenHeight() const override;
    int getWorldMapOffsetX() const override;
    int getWorldMapOffsetY() const override;
    bool getFlagIsDrawingDecisionSceneEnabled() const override;
    bool getFlagIsUIActive() const override;
    bool getFlagIsAIVerbose() const override;
    int execApp() override;

};

#endif