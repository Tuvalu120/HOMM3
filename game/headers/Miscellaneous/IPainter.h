/**
 * @file IPainter.h
 * @author Bartosz Han
 * @brief This file contains IPainter class,
 * which is an interface to various methods
 * related to drawing data on the screen.
 *
 */

#ifndef I_PAINTER_H
#define I_PAINTER_H


#include <array>
#include "Data/Constants.hpp"

using Color = std::array<int, 3>;

/**
 * @class IPainter
 * @brief This class is an interface to various methods
 * related to drawing data on the screen.
 */
class IPainter{

protected:
    IPainter() = default;

public:
    ~IPainter() = default;

    /**
    * @brief Draw into QPixmap a number in its lower left corner. This number will be covered by a small rectangle with given color.
    * @param number int Number to draw into pixmap.
    * @param color QColor Color of rectangle on which a number will be drawn.
    */
    virtual void drawNumberOnPixmap(const int number,
        const Color& color=constants::NEUTRAL_COLOR) = 0;

    /**
    * @brief Set certain pixels on the QPixmap into given color representing player.
    *
    * The pixels, which should be changed, must have a certain color - ui_constants::TEXTURE_PLAYER_COLOR_PLACEHOLDER.
    * This method is used to represent resources on the map, which belong to some player.
    *
    * @param pixmap Pixmap& Initial pixmap to change.
    * @param color QColor Color to change placeholder pixels.
    */
    virtual void replacePlayerColorOnPixmap(const Color& color=constants::NEUTRAL_COLOR) = 0;

    /**
     * @brief Draw a square of given color in one of the corners of the QPixmap.
     *
     * This method is used to represent resources on the map, which belong to some player.
     *
     * @param pixmap Pixmap& Pixmap to draw square on.
     * @param color QColor COlor of the newly drawn square.
     * @param position int Number representing corner in which square will be drawn. 0 - upper left; 1 - upper right; 2- lower right; 3 - lower left.
     * @param size int Size (in pixels) of newly drawn square.
     *
     */
    virtual void drawSquareOnPixmap(const Color& color = constants::NEUTRAL_COLOR, const int position=3, const int size=8) = 0;

};

#endif
