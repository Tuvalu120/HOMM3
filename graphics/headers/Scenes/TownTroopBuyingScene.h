/**
 * @file TownTroopBuyingScene.h
 * @author Bartosz Han
 * @brief This file contains TownTroopBuyingScene class, which is used for displaying state of the game of the when army entered the town,
 * and is deciding upon buying additional troops into their army.
 *
 */

#ifndef TOWN_TROOP_BUYING_SCENE_H
#define TOWN_TROOP_BUYING_SCENE_H

#include <QLayout>

#include "Scenes/Scene.h"

/**
 * @class TownTroopBuyingScene
 * @brief This class is used for displaying state of the game of the when army entered the town,
 * and is deciding upon buying additional troops into their army.
 */
class TownTroopBuyingScene: public Scene{
private:
    /**
     * @brief Draw available in town troop data into layout.
     *
     * This method will draw troop's image, name and its data into layout.
     * In addition, this function will give player information about possibility of buying this troop.
     *
     * @param layout QLayout on which troop data should be drawn.
     * QLayout must have specific structure: it must have 4 empty QLabels for data, and one QPosuButton as
     * last widget.
     * @param index Index of troop in vector of troops available to buy in visited town.
     */
    void drawTownTroopDataSingleLayout(QLayout* layout, const int index);

public:
    TownTroopBuyingScene();
    void drawScene() override;

};

#endif