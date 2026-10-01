/**
 * @file SubManager.h
 * @author Bartosz Han
 * @brief This file contains the definition of SubManager class, which is base class
 * for all managers.
 *
 */

#ifndef SUB_MANAGER_H
#define SUB_MANAGER_H

/**
 * @class SubManager
 * @brief This class is base class for all managers, which are responsible for managing the
 * individual components of the game loop and resources.
 *
 */
class SubManager{

protected:
    SubManager();

public:
    /**
     * @brief Reset the resources of the manager to the initial state.
     *
     */
    virtual void resetManager(){};

    SubManager(const SubManager&) = delete;
    SubManager& operator=(const SubManager&) = delete;
};

#endif