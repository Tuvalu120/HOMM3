/**
 * @file GraphicsManager.h
 * @author Bartosz Han
 * @brief This file contains GraphicsManager class, which is used as a interface between logic and display of the game.
 *
 */

#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "Managers/SubManager.h"
#include "Managers/IGraphicsManager.h"
#include <memory>

/**
 * @class GraphicsManager
 * @brief This class is used as an interface between logic and display of the game. It is used, so that GUI can have access to
 * the data of the game.
 *
 * This class is used mainly to access appropiate graphics manager interface. As this game should be used in both non-GUI and GUI cases,
 * there should be two different graphics managers to serve different cases of running this game. GraphicsManager class is used to control, which version
 * is used and gives access to appropiate manager.
 *
 */
class GraphicsManager: public SubManager{

    static std::unique_ptr<IGraphicsManager> manager_;

public:
    /**
     * @brief Get the appropiate graphics manager interface.
     *
     * This method should used in the same way as getting managers in other classes.
     *
     * @return IGraphicsManager& Reference to appropiate manager.
     */

    static IGraphicsManager& getInstance();

    /// @brief Register instance of used IGraphicsManager.
    static void registerInstance(IGraphicsManager* manager);

    /// @brief Get type of used graphics manager.
    static GraphicsManagerType getGraphicsManagerType();


};

#endif