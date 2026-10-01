/**
 * @file WorldDecisionMakingScene.cc
 * @author Bartosz Han
 * @brief This file contains definition of class WorldDecisionMakingScene.
 *
 */


#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QShowEvent>

#include "Scenes/WorldDecisionMakingScene.h"
#include "UIUtilities.hpp"
#include "GameWindow.h"
#include "Data/ObjectDataEnums.hpp"
#include "Managers/DecisionManager.h"
#include "Managers/GraphicsManager.h"
#include "ui_gamewindow.h"

using namespace std;
using namespace ui_constants;

WorldDecisionMakingScene::WorldDecisionMakingScene():Scene(WORLD_DECISION_MAKING){
    Ui::GameWindow* game_window = GraphicsManager::getInstance().getGameWindow()->getUI();
    decision_layouts_.push_back(game_window->decision_option_1);
    decision_layouts_.push_back(game_window->decision_option_2);
    decision_layouts_.push_back(game_window->decision_option_3);
};

void WorldDecisionMakingScene::drawScene(){


    if(!GraphicsManager::getInstance().getFlagIsDrawingDecisionSceneEnabled()){
        return;
    }

    if(!DecisionManager::getInstance().isDecisionInProgress()){
        throw DecisionLogicError("Tried to draw decision making scene, when no decision is made right now.");
    }

    // State of this Scene won't be changed while
    // player is making decision (and while this Scene is displayed), so it can be drawn only once.
    GraphicsManager::getInstance().setFlagIsDrawingDecisionSceneEnabled(false);

    Decision& decision = DecisionManager::getInstance().getCurrentDecision();
    Ui::GameWindow* game_window = GraphicsManager::getInstance().getGameWindow()->getUI();

    setTextOnLabel(game_window->decision_name, decision.getDecisionName());
    int n_decision_options = decision.getNOfDecisionOptions();


    for(size_t i=0; i<decision_layouts_.size(); ++i){
        clearLayout(decision_layouts_.at(i));
    }

    for(int i=0; i<n_decision_options; ++i){
        setUpLayout(decision_layouts_.at(i), decision.getDecisionOptionAt(i));
    }


    drawArmyData(game_window->world_decision_troop_layout,
        game_window->world_decision_resource_label,
        game_window->world_decision_spell_layout);

    drawHeroData(game_window->world_decision_hero_ui);
    setTextOnLabel(GraphicsManager::getInstance().getGameWindow()->getUI()->world_decision_hero_level_info,
        WorldManager::getInstance().getCurrentPlayer().getCurrentArmy().getArmyHero().getHeroLevelDescription());

}


void WorldDecisionMakingScene::setUpLayout(QLayout* layout, DecisionOption& option){

    enableLayout(layout);

    setImageOnLabel(layoutItemToQLabel(layout->itemAt(0)), *GraphicsManager::getInstance().getTexture(option.getCurrentTextureName()));
    layoutItemToQLabel(layout->itemAt(1))->setText(QString::fromStdString(option.getOptionName()));
    layoutItemToQLabel(layout->itemAt(1))->setAlignment(Qt::AlignCenter);
    auto text_data = option.getOptionDescription();
    vector<string> names = text_data.first;
    vector<string> values = text_data.second;

    QLayout* names_layout =  layout->itemAt(2)->layout()->itemAt(0)->layout();
    QLayout* values_layout = layout->itemAt(2)->layout()->itemAt(1)->layout();

    addLabelsToLayout(names_layout, names.size());
    addLabelsToLayout(values_layout, values.size());

    for(size_t i=0; i<names.size(); ++i){
        setTextOnLabel(layoutItemToQLabel(names_layout->itemAt(i)), names.at(i));
    }

    for(size_t i=0; i<values.size(); ++i){
        setTextOnLabel(layoutItemToQLabel(values_layout->itemAt(i)), values.at(i));
    }
}

void WorldDecisionMakingScene::clearLayout(QLayout* layout){

    layoutItemToQLabel(layout->itemAt(0))->clear();
    layoutItemToQLabel(layout->itemAt(1))->clear();

    QLayout* names_layout =  layout->itemAt(2)->layout()->itemAt(0)->layout();
    QLayout* values_layout = layout->itemAt(2)->layout()->itemAt(1)->layout();

    while(QLayoutItem* item = names_layout->takeAt(0)){
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    while(QLayoutItem* item = values_layout->takeAt(0)){
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    disableLayout(layout);
}
