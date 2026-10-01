/**
 * @file Painter.h
 * @author Bartosz Han
 * @brief This file contains Painter class,
 * which is used in RealGraphicsManager to
 * draw data on objects on screen.
 *
 */
#ifndef PAINTER_H
#define PAINTER_H

#include "Miscellaneous/IPainter.h"

#include <QPixmap>
#include <QPainter>

/**
 * @class Painter
 * @brief This class is used in RealGraphicsManager to
 * draw data on objects on screen.
 */
class Painter : public IPainter{
    private:
    QPixmap& pixmap_;

    public:
    Painter(QPixmap& pixmap);

    void drawNumberOnPixmap(const int number, const Color& color=constants::NEUTRAL_COLOR) override;
    void replacePlayerColorOnPixmap(const Color& color=constants::NEUTRAL_COLOR) override;
    void drawSquareOnPixmap(const Color& color = constants::NEUTRAL_COLOR, const int position=3, const int size=8) override;


    ~Painter() = default;
};

#endif