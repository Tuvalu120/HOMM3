/**
 * @file WorldDecisionMakingScene.h
 * @author Bartosz Han
 * @brief This file contains WorldDecisionMakingScene class, which is Scene used whenever player should make some kind of decision on the world map.
 *
 */
#ifndef WORLD_DECISION_MAKING_SCENE_H
#define WORLD_DECISION_MAKING_SCENE_H

#include <QLayout>

#include "Scenes/Scene.h"
#include "Decisions/DecisionOption.h"

/**
 * @class WorldDecisionMakingScene
 * @brief This class, which is one of Scenes used in GUI of this game, is used whenever player should make some kind of decision.
 *
 * Whenever any player has to make some kind of decision (defined as interacting with other objects,
 * which caused creating new 'Decision' object), this scene will be displayed,
 * showing player options for current decision.
 *
 */
class WorldDecisionMakingScene: public Scene{
private:
    std::vector<QLayout*> decision_layouts_;

public:
    WorldDecisionMakingScene();
    void drawScene() override;

    /**
     * @brief Set up Layout for displaying one option of current decision.
     *
     * This method will draw decision option's image, name, and its data into layout.
     * This method will also show the the button for selecting this option.
     *
     * @param layout QLayout on which decision option data should be drawn.
     * QLayout must have specific structure: it must have 2 empty QLabels for icon and name, then one QLayout
     * for decision option data and one QPosuButton for selecting option as last widget.
     * @param option DecisionOption&, whose data should be displayed on Layout.
     */
    void setUpLayout(QLayout* layout, DecisionOption& option);

    /**
     * @brief Diable Layout, used for displaying one option of current decision.
     *
     * This method will remove image, name, and other data related to the decision option from provided layout,
     * so that the state of this layout will be the same as before using setUpLayout() method for this layout.
     *
     * @param layout QLayout to clear.
     * QLayout must have specific structure: it must have 2 empty QLabels for icon and name, then one QLayout
     * for decision option data and one QPosuButton for selecting option as last widget.
     */
    void clearLayout(QLayout* layout);


};

#endif