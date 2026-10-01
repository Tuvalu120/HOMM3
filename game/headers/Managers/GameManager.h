/**
 * @file GameManager.h
 * @author Bartosz Han
 * @brief This file contains the definition of the GameManager class, which is responsible for managing the game state.
 *
 */


#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>

#include "Managers/SubManager.h"
#include "Singleton.hpp"

class SubManager;

/**
 * @class GameManager
 * @brief This class is responsible for managing the game state and lifecycle.
 *
 * The GameManager is a singleton that initializes game resources,
 * manages the game loop, and controls the overall game state.
 * It also handles the registration and lifecycle of all sub-managers.
 */
class GameManager : public Singleton<GameManager> {

    friend class Singleton<GameManager>;

private:
    int argc_;
    char** argv_;
    GameManager();
    std::vector<SubManager*> sub_managers_;
    bool flag_game_manager_finishes_game_ = true;

public:
    /**
     * @brief Initializes the game manager and all other managers and their resources.
     * @param argc Number of command-line arguments.
     * @param argv Command-line argument values.
     */
    void initialize(int argc, char *argv[]);

    /**
     * @brief Starts the game loop.
     * This method must be called after the initialize() method.
     */
    void play();

    /**
     * @brief Checks whether the game has finished, by checking the number of active players left.
     * @return True if there is only one player left, false otherwise.
     */
    bool isGameFinished() const;

    /**
     * @brief Executes the application. In case of GUI mode, this method will start the GUI event loop.
     * @return Exit code of the application.
     */
    int execApp();

    /// @brief Returns a reference to the argc value.
    int& getArgc();

    /// @brief Returns the argv pointer.
    char** getArgv() const;

    /**
     * @brief Registers a sub-manager to the game manager.
     * This is mainly used for resetting the game state.
     * @param sub_manager Pointer to a sub-manager instance.
     */
    void registerSubManager(SubManager* sub_manager);

    /**
     * @brief Resets the game state and all sub-manager resources.
     */
    void resetGame();

    /**
     * @brief Set flag, informing the program
     * GameManager is not responsible for
     * managing game's end. This method
     * is used in battle example program.
     *
     * @param value
     */
    void setFlagGameManagerFinishesGame(bool value);
};


#endif