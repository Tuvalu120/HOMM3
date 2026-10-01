/**
 * @file IGraphicsManager.h
 * @author Bartosz Han
 * @brief This fils contains IGraphicsManager interface, which is used in graphics manager classes.
 *
 */

#ifndef I_GRAPHICS_MANAGER_H
#define I_GRAPHICS_MANAGER_H

#include <string>

#include "Data/ObjectDataEnums.hpp"
#include "Managers/SubManager.h"

class QPixmap;

/**
 * @enum GraphicsManagerType
 * Used types of graphics managers, used in programs.
 */
enum GraphicsManagerType{
    REAL_GRAPHICS_MANAGER,
    MOCK_GRAPHICS_MANAGER
};

class GameWindow;

/**
 * @class IGraphicsManager
 *
 * @brief This class is a interface, providing virtual methods needed to implement GraphicsManager
 *
 * This interface is used in such way, that other components of the game could use GraphicsManager - whether or not they are using
 * GUI version of manager, or not. There should be two versions of graphics managers, depending whether or not GUI should be used.
 * In case of GUI verion, graphics manager provides access to GUI elements and is used to manipulate them. In case of non-GUI version,
 * graphics manager does nothing - it is a mock version of real graphics manager.
 *
 * @note All of documentation of methods in this class are describing behaviour of RealGraphicsManager. In case of MockGraphicsManager,
 * with exception of setToScene() and getCurrentScene(), these methods do nothing and their return values should be ignored.
 *
 */
class IGraphicsManager: public SubManager{

protected:
    IGraphicsManager() = default;

public:
    virtual ~IGraphicsManager() = default;

    /// @brief Returns GraphicsManagerType of this manager.
    virtual GraphicsManagerType getGraphicsManagerType() = 0;

    /**
     * @brief Get the QPixmap object of image stored in given file.
     *
     * This method loads new QPixmap to the game, if this texture is not used
     * in game yet and returns reference to it, or gives directly reference to already loaded texture.
     *
     * @param texture_name Name of file of texture
     * @return QPixmap& reference to QPixmap which contains texture in
     * provided file
     */
    virtual QPixmap* getTexture(std::string texture_name) = 0;

    /**
     * @brief Get the SceneType enum representing the state in which
     * game is right now.
     *
     * This is mainly used to draw data representing current game state,
     * but can also be used directly as a method to get current game state.
     *
     * @return SceneType enum representing game state.
     */
    virtual SceneType getCurrentScene() const = 0;

    /**
     * @brief Generate and set up resources related GUI and game window.
     *
     * This method should be used before beginning the game loop.
     */
    virtual void setUpGameWindow() = 0;

    /**
     * @brief Get GameWindow object, representing the GUI window and
     * their elements.
     *
     * @return GameWindow* pointer to the object representing game window.
     */
    virtual GameWindow* getGameWindow() = 0;

    /**
     * @brief Draw the game window into display.
     *
     * This method will run show() method of GameWindow object.
     */
    virtual void showWindow() = 0;


    /**
     * @brief Run the window application.
     *
     * This method calls QApplication::exec() and returns value of this method.
     *
     * @return int Retrun of QApplication::exec().
     */
    virtual int execApp() = 0;

    /**
     * @brief Process events related to GUI input provided by the app user.
     *
     * This method runs QCoreApplication::processEvents() to process user input.
     *
     */
    virtual void processEvents() = 0;

    /**
     * @brief Do GUI logic related to ending the game,
     * and call QApplication::quit() at the end.
     *
     * This method is used and should be used only when game is finished in proper way,
     * which is when only one player is left.
     *
     */
    virtual void quitApplication() = 0;

    /**
     * @brief Draw on the display thedata related to the current state of the game.
     *
     */
    virtual void drawCurrentScene() = 0;

    /**
     * @brief Set the new scene value, which represents game state.
     *
     * @param new_scene New SceneType value, representing change in game state.
     */
    virtual void setToScene(SceneType new_scene) = 0;

    /**
     * @brief Get the width (in pixels) of the window on display, in
     * which game is being drawn.
     *
     */
    virtual int getScreenWidth() const = 0;

    /**
     * @brief Get the height (in pixels) of the window on display, in
     * which game is being drawn.
     *
     */
    virtual int getScreenHeight() const = 0;

    /**
     * @brief Start the QTimer, which will from now on draw on display
     * current game state in certain time periods.
     *
     */
    virtual void startDrawing() = 0;

    /**
     * @brief Increase the current value of the X offset of tiles on
     * world map by given value.
     *
     * @param value int Value to increase X offset.
     */
    virtual void addWorldMapOffsetX(int value) = 0;

    /**
     * @brief Increase the current value of the Y offset of tiles on
     * world map by given value.
     *
     * @param value int Value to increase Y offset.
     */
    virtual void addWorldMapOffsetY(int value) = 0;

    /**
     * @brief Set the current value of the X offset of tiles on
     * world map by given value.
     *
     * @param value int Value to set X offset.
     */
    virtual void setWorldMapOffsetX(int value) = 0;

    /**
     * @brief Set the current value of the Y offset of tiles on
     * world map by given value.
     *
     * @param value int Value to set Y offset.
     */
    virtual void setWorldMapOffsetY(int value) = 0;


    /**
     * @brief Set the current offset of the tiles on world map
     * to be (0, 0).
     *
     */
    virtual void resetWorldMapOffset() = 0;

    /**
     * @brief Get the world map offset X value.
     *
     * Normally, on world map, manager draws data nad tiles in such
     * way that current army is in the center of the tiles drawn.
     * This behaviour can be overriden by WorldMapOffset values, which
     * will set the centre of tiles drawn by given values with regard to
     * the position of current army.
     *
     * @return int world map offset X value.
     */
    virtual int getWorldMapOffsetX() const = 0;

    /**
     * @brief Get the world map offset Y value.
     *
     * Normally, on world map, manager draws data nad tiles in such
     * way that current army is in the center of the tiles drawn.
     * This behaviour can be overriden by WorldMapOffset values, which
     * will set the centre of tiles drawn by given values with regard to
     * the position of current army.
     *
     * @return int world map offset Y value.
     */
    virtual int getWorldMapOffsetY() const = 0;

    /**
     * @brief Get the value of flag_is_drawing_decision_scene_enabled_.
     *
     * This flag is used for informing WorldDecisionMakingScene, that it can draw scene into window.
     * This flag should be set to true whenever decision on world map is being made, and should be set to false by Scene
     * associated with this event to indicate, that this scene has been drawn.
     */
    virtual bool getFlagIsDrawingDecisionSceneEnabled() const = 0;

    /**
     * @brief Set the value of flag_is_drawing_decision_scene_enabled_.
     *
     * @param new_value Ew value of the flag.
     */
    virtual void setFlagIsDrawingDecisionSceneEnabled(bool new_value) = 0;

    virtual void enableSpellLayout() = 0;
    virtual void disableSpellLayout() = 0;

    /**
     * @brief Get the flag, informing program, that
     * buttons and other elements in GUI can now be used by user.
     * This flag should be set to true whenever human player is taking its turn,
     * and set to false when computer is taking its turn.
     */
    virtual bool getFlagIsUIActive() const = 0;

    /**
     * @brief Set the flag, informing program, that
     * buttons and other elements in GUI can now be used by user.
     * This flag should be set to true whenever human player is taking its turn,
     * and set to false when computer is taking its turn.
     */
    virtual void setFlagIsUIActive(bool new_value) = 0;

    /**
     * @brief Set the flag, informing program to print performed by AI actions in terminal.
     * Used in battle example program.
     */
    virtual void setFlagIsAIVerbose(const bool value) = 0;

    /**
     * @brief Get the flag, informing program to print performed by AI actions in terminal.
     * Used in battle example program.
     */
    virtual bool getFlagIsAIVerbose() const = 0;
};

#endif