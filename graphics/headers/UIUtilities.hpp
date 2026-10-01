/**
 * @file UIUtilities.hpp
 * @author Bartosz Han
 * @brief This file contains various method used in other method and classes
 * related to generating and managing GUI.
 */

#ifndef UI_UTILITIES_H
#define UI_UTILITIES_H

#include <QLayout>
#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QPainter>

#include "UIConstants.hpp"
#include "Managers/GraphicsManager.h"
#include "GameObjects/Army.h"
#include "GameObjects/Stack.h"
#include "Managers/WorldManager.h"
#include "Managers/BattleManager.h"
#include "GameObjects/Town.h"
#include "Painter.h"


/**
 * @brief Make layout invisible in window.
 *
 * @param layout Layout to disable
 */
inline void disableLayout(QLayout* layout){
    for(int i = 0; i < layout->count(); ++i){
        QLayoutItem* item = layout->itemAt(i);

        if(QWidget* widget = item->widget()){
            widget->hide();
        }

        else if(QLayout* child_layout = item->layout()){
            disableLayout(child_layout);
        }
    }
}

/**
 * @brief Make layout visible in window.
 *
 * @param layout Layout to enable
 */
inline void enableLayout(QLayout* layout){
    for(int i = 0 ; i < layout->count(); ++i){
        QLayoutItem* item = layout->itemAt(i);

        if(QWidget* widget = item->widget()){
            widget->show();

        }
        else if(QLayout* child_layout = item->layout()){
            enableLayout(child_layout);
        }
    }
}

/**
 * @brief Set the QPixmap on given QLabel.
 *
 * @param label QLabel* pointer to the label to set image to.
 * @param pixmap QPixmap& reference to image to set on the label.
 */
inline void setImageOnLabel(QLabel* label, const QPixmap& pixmap){
    label->setPixmap(pixmap);
    label->setAlignment(Qt::AlignCenter);
}

/**
 * @brief Set the QPixmap, generated from given image file path onto QLabel.
 *
 * @param label QLabel* pointer to the label to set image to.
 * @param image_name std::string Filepath to the image to set onto label.
 */
inline void setImageOnLabel(QLabel* label, const std::string image_name){
    setImageOnLabel(label, *GraphicsManager::getInstance().getTexture(image_name));
}

/**
 * @brief Set the text on given label.
 *
 * @param label QLabel* Label to set text into.
 * @param text std::string String to display on label.
 * @param alignment Qt::AlignmentFlag Alignment of text on label.
 */
inline void setTextOnLabel(QLabel* label, const std::string text, Qt::AlignmentFlag alignment = Qt::AlignCenter){
    label->setText(QString::fromStdString(text));
    label->setAlignment(alignment);
}

/**
 * @brief Add new empty labels into layout.
 *
 * @param layout QLayout* Pointer to layout to add empty labels.
 * @param quantity int Number of empty labels to add.
 */
inline void addLabelsToLayout(QLayout* const layout, const int quantity){
    for(int i=0; i<quantity; ++i){
        QLabel* newLabel = new QLabel();
        layout->addWidget(newLabel);
    }
}

/**
 * @brief Make QLayoutItem to become QLabel item.
 *
 * @param item QLayoutItem* Pointer to QLayoutItem to change.
 * @return QLabel* Poniter to QLabel item, which is the same object to QLayoutItem* given in this function.
 */
inline QLabel* layoutItemToQLabel(QLayoutItem* item){
    return static_cast<QLabel*>(item->widget());
}

/**
 * @brief Remove all elements of layout, so that only labels with no additional elements on them will remain in it.
 *
 * @param layout QLayout* to clean. This layout must have QPushbutton as a last element.
 */
inline void clearLayout(QLayout* layout){
    for(int i=0; i<layout->count()-1; ++i){
        layoutItemToQLabel(layout->itemAt(i))->clear();
    }
    static_cast<QPushButton*>(layout->itemAt(layout->count()-1)->widget())->setText(QString::fromStdString(ui_constants::CANNOT_DO_BUTTON_TEXT));
}

inline QColor arrayToQColor(const Color& color){
    return QColor(color.at(0), color.at(1), color.at(2));
}

/**
 * @brief Generate QPixmap representing Stack and its data.
 *
 * This method will draw texture of the player into the pixmap, as well as other resources related to this stack.
 *
 * @param stack Stack& of which the pixmap should be generated.
 * @param add_background If true, the background of the pixmap will be the same as game color. If false, the background will be transparent.
 * @return QPixmap Pixmap representing stack.
 */
inline QPixmap generateStackImage(const Stack& stack, const bool add_background){
    QPixmap stack_pixmap(ui_constants::TILE_SIZE, ui_constants::TILE_SIZE);
    if(add_background){
        stack_pixmap.fill(ui_constants::GAME_MAIN_COLOR);
    }
    QPainter q_painter(&stack_pixmap);
    q_painter.drawPixmap(0, 0, *GraphicsManager::getInstance().getTexture(stack.getCurrentTextureName()));
    q_painter.end();

    Painter painter(stack_pixmap);
    stack.drawAdditionalData(painter);
    return stack_pixmap;
}

/**
 * @brief Pads a given int with leading spaces to ensure it meets a specified length.
 *
 * @param number int The integer to be padded.
 * @param padding int The desired total length of the resulting string.
 * @return std::string A string created from the number, padded with leading spaces if necessary.
 */
inline const std::string padNumber(const int number, const int padding){
    std::string final_string = std::to_string(number);
    while(final_string.length() < static_cast<size_t>(padding)){
        final_string = " " + final_string;
    }
    return final_string;
};


/**
 * @brief Generates a QPixmap representation of a resource array with optional scaling and filtering.
 *
 * This function creates a graphical representation of a resource array, where each resource is
 * displayed as an image alongside its quantity.
 *
 * @param resource_array The array of resource quantities to be displayed.
 * @param scale A scaling factor for the size of the images and text.
 * @param draw_with_no_resources If true, all resources will be displayed even if their quantity is zero.
 * If false, resources with quantity zero are skipped.
 * @param draw_only_gold If true, only the gold resource will be displayed.
 * @return QPixmap A pixmap containing the graphical representation of the resource array.
 *
 * @note If both `draw_with_no_resources` and `draw_only_gold` are true, only the gold resource is displayed.
 */
inline QPixmap generateResourceArrayImage(const ResourceArray& resource_array, float scale = 1.0f,
    bool draw_with_no_resources = false, bool draw_only_gold = false){

    int resource_counter = 0;

    if(draw_with_no_resources && draw_only_gold){
        resource_counter = 1;
    }
    else if(draw_with_no_resources){
        resource_counter = resource_array.size();

    }
    else{

        for(const unsigned int& quantity: resource_array){
            if(quantity > 0){
                ++resource_counter;
            }
        }
    }

    QFont font(QString::fromStdString(ui_constants::GAME_TEXT_MAIN_FONT), ui_constants::RESOURCE_ARRAY_FONT_HEIGHT * scale);
    QFontMetrics metrics(font);
    QString metrics_text = "88888"; //QString to calculate width of the text
    int text_width = metrics.horizontalAdvance(metrics_text) + 2 * ui_constants::RESOURCE_ARRAY_MARGIN * scale;
    QPixmap resource_pixmap((text_width + ui_constants::RESOURCE_ARRAY_QPIXMAP_SIZE * scale) * resource_counter, ui_constants::RESOURCE_ARRAY_QPIXMAP_SIZE * scale);
    resource_pixmap.fill(ui_constants::GAME_MAIN_COLOR);
    QPainter painter(&resource_pixmap);
    painter.setFont(font);
    painter.setPen(Qt::white);
    int x_margin = 0;
    int y_image_margin = 0;
    int y_text_margin = (ui_constants::RESOURCE_ARRAY_QPIXMAP_SIZE - (ui_constants::RESOURCE_ARRAY_QPIXMAP_SIZE - ui_constants::RESOURCE_ARRAY_FONT_HEIGHT) / 2) * scale;
    int loop_size = draw_only_gold ? 1 : N_RESOURCES;
    for(int i=0; i<loop_size; ++i){
        if(resource_array.at(i) <= 0 && !draw_with_no_resources){
            continue;
        }
        QPixmap current_resource_pixmap(*GraphicsManager::getInstance().getTexture(ui_constants::RESOURCES_IMAGE[i]));
        current_resource_pixmap = current_resource_pixmap.scaled(ui_constants::RESOURCE_ARRAY_QPIXMAP_SIZE * scale, ui_constants::RESOURCE_ARRAY_QPIXMAP_SIZE * scale);
        painter.drawPixmap(x_margin, y_image_margin, current_resource_pixmap);
        x_margin += current_resource_pixmap.width();
        std::string quantity_string = padNumber(resource_array.at(i), ui_constants::RESOURCE_ARRAY_MAX_N_DIGITS);
        x_margin += ui_constants::RESOURCE_ARRAY_MARGIN * scale;
        QString pad_number = QString::fromStdString(quantity_string);
        painter.drawText(x_margin, y_text_margin, pad_number);
        x_margin += text_width - ui_constants::RESOURCE_ARRAY_MARGIN * scale;


    }
    painter.end();
    return resource_pixmap;
}

/**
 * @brief Draw current army data into given widgets.
 *
 * This function will draw army's stacks and their info, army player's resources and army's hero and its spells in
 * appropiate widgets.
 *
 * @param stacks_layout QLayout* to draw data about stacks.
 * @param resources_label QLabel* to draw current player's resources.
 * @param spell_layout QGridLayout* to draw data regarding spells.
 */
inline void drawArmyData(QLayout* stack_layout, QLabel* resources_label, QGridLayout* spell_layout){

    setImageOnLabel(resources_label, generateResourceArrayImage(WorldManager::getInstance().getCurrentPlayer().getPlayerResources(), 1.5f, true));
    QPixmap* empty_stack_pixmap = new QPixmap(ui_constants::TILE_SIZE, ui_constants::TILE_SIZE);
    QPixmap* empty_spell_pixmap = new QPixmap(ui_constants::TILE_SIZE, ui_constants::TILE_SIZE);
    empty_stack_pixmap->fill(ui_constants::GAME_MAIN_COLOR);
    empty_spell_pixmap->fill(ui_constants::GAME_MAIN_COLOR);

    Army& current_army = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy();
    int widget_counter = 0;
    for(const auto& stack: current_army.getArmyStacks()){
        QPixmap *new_stack_image = new QPixmap(generateStackImage(*stack, true));
        setImageOnLabel(layoutItemToQLabel(stack_layout->itemAt(widget_counter)), *new_stack_image);
        ++widget_counter;
    }
    for(int i = widget_counter; i<stack_layout->count(); ++i){
        setImageOnLabel(layoutItemToQLabel(stack_layout->itemAt(i)), *empty_stack_pixmap);
    }

    const auto& hero_spells = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().getHeroSpells();
    int width = spell_layout->columnCount();
    int height = spell_layout->rowCount();
    for(int i=0; i<height; ++i){
        for(int j=0; j<width; ++j){
            int index = i*width + j;
            if(hero_spells.at(index) != nullptr){
                QPixmap pixmap(ui_constants::TILE_SIZE, ui_constants::TILE_SIZE);
                pixmap.fill(ui_constants::GAME_MAIN_COLOR);
                QPainter q_painter(&pixmap);
                q_painter.drawPixmap(0, 0, *GraphicsManager::getInstance().getTexture(hero_spells.at(index)->getCurrentTextureName()));
                q_painter.end();
                Painter painter(pixmap);
                hero_spells.at(index)->drawAdditionalData(painter);
                setImageOnLabel(static_cast<QLabel*>(spell_layout->itemAt(index)->widget()), pixmap);
            }
            else{

                setImageOnLabel(static_cast<QLabel*>(spell_layout->itemAt(index)->widget()), *empty_spell_pixmap);
            }
        }
    }
}

/**
 * @brief Draw in display data regarding current army's hero, especially its current statistics.
 *
 * This function will draw hero's name, texture, its main statistics and moves left.
 *
 * @param hero_data_layout QLayout* to draw hero's data. This layout must be earlier appropriately initialized with empty elements.
 */
inline void drawHeroData(QLayout* hero_data_layout){
    if(hero_data_layout->count() != game_data::HERO_STATISTICS_DISPLAY_COUNT + 3){ //"+ 3" for three widgets used for: hero's name, texture and moves left.
        throw std::invalid_argument("Error: the number of Widgets in Hero Data Layout is incorrect");
    }
    setImageOnLabel(layoutItemToQLabel(hero_data_layout->itemAt(0)), WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().getCurrentTextureName());
    setTextOnLabel(layoutItemToQLabel(hero_data_layout->itemAt(1)), WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().getHeroName());
    QFont name_font(QString::fromStdString(ui_constants::GAME_TEXT_MAIN_FONT), ui_constants::UI_NAME_FONT_SIZE);
    layoutItemToQLabel(hero_data_layout->itemAt(1))->setFont(name_font);


    for (int i=0 ; i<game_data::HERO_STATISTICS_DISPLAY_COUNT+1; ++i){
        QFont font(QString::fromStdString(ui_constants::GAME_TEXT_MAIN_FONT), ui_constants::HERO_STATISTIC_TEXT_HEIGHT);
        QFontMetrics metrics(font);
        QString metrics_text = "8888"; //QString to calculate width of the text
        int text_width = metrics.horizontalAdvance(metrics_text);
        QPixmap stat_pixmap(text_width + ui_constants::HERO_STATISTIC_ICON_WIDTH, ui_constants::HERO_STATISTIC_ICON_HEIGHT);
        stat_pixmap.fill(ui_constants::GAME_MAIN_COLOR);
        QPainter painter(&stat_pixmap);
        painter.setFont(font);
        painter.setPen(Qt::white);
        QRect image_rect(0,0, ui_constants::HERO_STATISTIC_ICON_WIDTH, ui_constants::HERO_STATISTIC_ICON_HEIGHT);
        QString hero_stat;
        if(i != game_data::HERO_STATISTICS_DISPLAY_COUNT){
            painter.drawPixmap(image_rect, *GraphicsManager::getInstance().getTexture(game_data::HERO_STATISTICS_IMAGE[i]));
            hero_stat = QString::number(WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().getStatistic(game_data::HERO_DISPLAY_STAT.at(i)));
        }
        else{
            painter.drawPixmap(image_rect, *GraphicsManager::getInstance().getTexture(ui_constants::MOVES_LEFT_ICON));
            hero_stat = QString::number(WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getMovesLeft());
        }
        QRect text_rect(ui_constants::HERO_STATISTIC_ICON_WIDTH, 0, text_width, ui_constants::HERO_STATISTIC_ICON_HEIGHT);
        painter.drawText(text_rect, Qt::AlignCenter, hero_stat);
        setImageOnLabel(layoutItemToQLabel(hero_data_layout->itemAt(2+i)), stat_pixmap);
    }
}

/**
 * @brief Draw current battle's order of turns into layout.
 *
 * This function will draw textures of all battle objects into the layout.
 * In addition, it will also draw rectangle player indicator on each of the textures.
 *
 * @param battle_order_layout QLayout* Layout to draw battle order
 */
inline void drawBattleOrder(QLayout* battle_order_layout){
    const auto& battle_order = BattleManager::getInstance().getCurrentBattle().getBattleOrder();
    int max_index = battle_order.size();
    int current_index = 0;

    for(int i=0; i<ui_constants::BATTLE_ORDER_UI_DISPLAY_COUNT; ++i){
        const BattleObject& current_battle_object = *battle_order.at(current_index);
        QPixmap current_pixmap(*GraphicsManager::getInstance().getTexture(current_battle_object.getUITextureName()));
        QPainter painter(&current_pixmap);
        QRect player_indicator(0,0, ui_constants::BATTLE_OBJECT_PLAYER_INDICATOR_SQUARE_SIZE, ui_constants::BATTLE_OBJECT_PLAYER_INDICATOR_SQUARE_SIZE);
        painter.fillRect(player_indicator, arrayToQColor(constants::playerIDToQColor(current_battle_object.getPlayerID())));
        setImageOnLabel(layoutItemToQLabel(battle_order_layout->itemAt(i)), current_pixmap);
        ++current_index;
        if(current_index >= max_index){
            current_index=0;
        }
    }
}


/**
 * @brief Draw data to display information about a specific town building.
 *
 * This function will display the current state building in a town, including its image, description,
 * and upgrade information.
 *
 * If the building can be upgraded, the upgrade cost and a button to upgrade are displayed.
 * Otherwise, appropiate information is displayed.
 *
 * @param layout Pointer to the QLayout that contains the UI elements for displaying the building data.
 *               The layout is expected to have exactly four items: 3 QLabels and one QPushButton as the last element.
 * @param building_type The BuildingType of the building to display information for.
 * @param town The Town& object containing the building to display.
 */
inline void drawTownBuildingDataSingleLayout(QLayout* layout, const BuildingType& building_type, const Town& town){
    Building& building = town.getBuilding(building_type);
    setImageOnLabel(layoutItemToQLabel(layout->itemAt(0)), building.getCurrentTextureName());
    setTextOnLabel(layoutItemToQLabel(layout->itemAt(1)), building.getBuildingDescription());
    if(building.getBuildingLevel() != building.getMaxBuildingLevel()){
        setImageOnLabel(layoutItemToQLabel(layout->itemAt(2)), generateResourceArrayImage(building.getUpgradeCost()));
        static_cast<QPushButton*>(layout->itemAt(3)->widget())->setText(QString::fromStdString(ui_constants::TOWN_BUILDING_BUTTON_TEXT));
    }
    else{
        layoutItemToQLabel(layout->itemAt(2))->clear();
                static_cast<QPushButton*>(layout->itemAt(3)->widget())->setText(QString::fromStdString(ui_constants::TOWN_MAX_LEVEL_OF_BUILDING_TEXT));
    }
}

#endif