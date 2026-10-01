/**
 * @file DrawingTimer.h
 * @author Bartosz Han
 * @brief This file contains DrawingTimer class, which is used for periodically drawing scenes.
 *
 */

#ifndef DRAWING_TIMER_H
#define DRAWING_TIMER_H

#include <QObject>
#include <QTimer>

/**
 * @class DrawingTimer
 * @brief This class is used for periodically drawing scenes, by calling GraphicsManager::drawCurrentScene().
 *
 * Upon creating instance of this class, this object creates QTimer, which is connected to the method responsible for drawing whole current scene.
 * Then, QTimer is started in GraphicsManager, which will cause window to be redrawn every several milliseconds.
 * This class is used as a interface between QTimer responsible for drawing window and other components of the game.
 *
 */
class DrawingTimer: public QObject{

    Q_OBJECT

private:
    QTimer* drawing_timer_;

public:
    DrawingTimer();

    /// @brief Return QTimer object, which is used in redrawing game window.
    QTimer& getTimer() const;

};

#endif