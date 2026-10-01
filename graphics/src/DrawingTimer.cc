/**
 * @file DrawingTimer.cc
 * @author Bartosz Han
 * @brief This file contains definition of class DrawingTimer.
 *
 */


#include "DrawingTimer.h"
#include "Managers/GraphicsManager.h"

DrawingTimer::DrawingTimer(){
    drawing_timer_ = new QTimer(this);

    connect(drawing_timer_, &QTimer::timeout, [](){
        GraphicsManager::getInstance().drawCurrentScene();
    });
}

QTimer& DrawingTimer::getTimer() const{
    return *drawing_timer_;
}
