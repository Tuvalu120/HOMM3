/**
 * @file TownMainScene.h
 * @author Bartosz Han
 * @brief This file contains TownHeroBuyingScene class, which is used for displaying state of the game of the when army entered the town,
 * and is deciding upon buying additional heroes and armies for them.
 *
 */

#ifndef TOWN_HERO_BUYING_SCENE_H
#define TOWN_HERO_BUYING_SCENE_H

#include "Scenes/Scene.h"


/**
 * @class TownHeroBuyingScene
 * @brief this class is used for displaying state of the game of the when army entered the town,
 * and is deciding upon buying additional heroes and armies for them.
 *
 */
class TownHeroBuyingScene: public Scene{

private:
    /**
     * @brief Draw available in town hero data into layout.
     *
     * This method will draw hero's image, name, and cost into layout.
     * In addition, this function will give player information about possibility of buying this hero.
     *
     * @param layout QLayout on which hero data should be drawn.
     * QLayout must have specific structure: it must have 3 empty QLabels for data, and one QPosuButton as
     * last widget.
     * @param index Index of hero in container of heroes available to buy in visited town.
     */
    void drawTownHeroDataSingleLayout(QLayout* layout, HeroType hero_type);

public:
    TownHeroBuyingScene();
    void drawScene() override;
};

#endif