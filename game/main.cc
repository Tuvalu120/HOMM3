/**
 * @file game/main.cc
 * @author Bartosz Han
 * @brief This file contains main function of the game.
 */

#include <QApplication>

#include "Managers/GameManager.h"
#include "Managers/GraphicsManager.h"
#include "RealGraphicsManager.h"


int main(int argc, char *argv[]){

    GraphicsManager::registerInstance(new RealGraphicsManager());
    GameManager::getInstance().initialize(argc, argv);

    GameManager::getInstance().play();

    return GameManager::getInstance().execApp();

}