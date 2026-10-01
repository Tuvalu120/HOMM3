/**
 * @file Painter.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Painter.
 *
 */
#include <QColor>
#include <QPainter>
#include <QColor>

#include "Painter.h"
#include "UIConstants.hpp"
#include "UIUtilities.hpp"


Painter::Painter(QPixmap& pixmap):pixmap_(pixmap){};

void Painter::drawNumberOnPixmap(const int number, const Color& color){

    QPainter painter(&pixmap_);
    QColor q_color = arrayToQColor(color);

    painter.setFont(QFont(QString::fromStdString(ui_constants::GAME_TEXT_MAIN_FONT), ui_constants::TILE_TEXT_SIZE));
    QString number_text = QString::number(number);
    QFontMetrics metrics(painter.font());
    int text_width = metrics.horizontalAdvance(number_text);
    int text_height = metrics.height();

    int rect_x_pos = 0;
    int rect_y_pos = pixmap_.height() - text_height;

    painter.setBrush(q_color);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect_x_pos, rect_y_pos, text_width, text_height);

    painter.setPen(Qt::white);
    painter.drawText(rect_x_pos, rect_y_pos, text_width, text_height, Qt::AlignLeft | Qt::AlignTop, number_text);
}

void Painter::replacePlayerColorOnPixmap(const Color& color){

    QColor source_color = ui_constants::TEXTURE_PLAYER_COLOR_PLACEHOLDER;
    QColor q_color = arrayToQColor(color);

    QImage image = pixmap_.toImage();

    for(int y=0; y<image.height(); ++y){
        for(int x=0; x<image.width(); ++x){
            QColor current_color = image.pixelColor(x,y);
            if(current_color == source_color){
                image.setPixelColor(x,y,q_color);
            }
        }
    }
    pixmap_ = QPixmap::fromImage(image);

}

void Painter::drawSquareOnPixmap(const Color& color, const int position, const int size){

    QPainter painter(&pixmap_);
    QColor q_color = arrayToQColor(color);

    int sq_x_pos = 0;
    int sq_y_pos = 0;

    switch(position){
        case 0: //upper left
            sq_x_pos = 0;
            sq_y_pos = 0;
            break;

        case 1: //upper right
            sq_x_pos = pixmap_.width() - size;
            sq_y_pos = 0;
            break;

        case 2: //lower right
            sq_x_pos = pixmap_.width() - size;
            sq_y_pos = pixmap_.height() - size;
            break;

        case 3: //lower left
            sq_x_pos = 0;
            sq_y_pos = pixmap_.height() - size;
            break;

        default:
            break;
    }
    painter.setBrush(q_color);
    painter.setPen(Qt::NoPen);
    painter.drawRect(sq_x_pos, sq_y_pos, size, size);
}
