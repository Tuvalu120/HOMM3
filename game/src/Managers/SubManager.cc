/**
 * @file SubManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class SubManager.
 *
 */


#include "Managers/SubManager.h"
#include "Managers/GameManager.h"

SubManager::SubManager(){
    GameManager::getInstance().registerSubManager(this);
}
