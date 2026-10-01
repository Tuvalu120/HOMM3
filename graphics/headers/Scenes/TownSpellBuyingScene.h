/**
 * @file TownSpellBuyingScene.h
 * @author Bartosz Han
 * @brief This file contains TownSpellBuyingScene class, which is used for displaying state of the game of the when army entered the town,
 * and is deciding upon buying additional spells for their hero.
 *
 */

#ifndef TOWN_SPELL_BUYING_SCENE_H
#define TOWN_SPELL_BUYING_SCENE_H

#include "Scenes/Scene.h"

/**
 * @class TownSpellBuyingScene
 * @brief This class is used for displaying state of the game of the when army entered the town,
 * and is deciding upon buying additional spells for their hero.
 */
class TownSpellBuyingScene: public Scene{
private:

    /**
     * @brief Draw available in town spell data into layout.
     *
     * This method will draw spell's icon, name, description and cost into layout.
     * In addition, this function will give player information about possibility of buying this spell.
     *
     * @param layout QLayout on which spell data should be drawn.
     * QLayout must have specific structure: it must have 4 empty QLabels for data, and one QPosuButton as
     * last widget.
     * @param index Index of spell in container of spells available to buy in visited town.
     */
    void drawTownSpellDataSingleLayout(QLayout* layout, const SpellType spell_type, const int spell_level);

public:
    TownSpellBuyingScene();
    void drawScene() override;

};

#endif