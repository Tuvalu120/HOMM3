/**
 * @file GraphicsManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class GraphicsManager.
 *
 */

#include <memory>
#include <stdexcept>

#include "Managers/GraphicsManager.h"
#include "Managers/IGraphicsManager.h"
#include "Managers/MockGraphicsManager.h"

using namespace std;

unique_ptr<IGraphicsManager> GraphicsManager::manager_ = make_unique<MockGraphicsManager>();

IGraphicsManager& GraphicsManager::getInstance(){
    return *manager_;
}

GraphicsManagerType GraphicsManager::getGraphicsManagerType(){
    return manager_->getGraphicsManagerType();
}


void GraphicsManager::registerInstance(IGraphicsManager* manager){
    static bool was_registered = false;
    if(!was_registered){
        if(manager == nullptr){
            throw invalid_argument("Tried to register nullptr as GraphicsManager");
        }
        manager_.reset(manager);
        was_registered = true;
    }
    else{
        throw runtime_error("Cannot register GraphicsManager more than once");
    }
}