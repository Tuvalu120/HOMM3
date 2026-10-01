/**
 * @file GameWindow.cc
 * @author Bartosz Han
 * @brief This file contains definition of class GameWindow.
 *
 */


#include <QApplication>
#include <QCloseEvent>
#include <QPainter>

#include "Managers/GraphicsManager.h"
#include "ui_gamewindow.h"
#include "GameWindow.h"
#include "Painter.h"
#include "UIConstants.hpp"
#include "UIUtilities.hpp"

using namespace std;
using namespace constants;
using namespace game_data;
using namespace ui_constants;

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), ui_(make_unique<Ui::GameWindow>()){

    ui_->setupUi(this);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setWindowFlags(windowFlags() &= ~Qt::WindowFullscreenButtonHint);

    vector<QPushButton*> spell_buttons = {ui_->spell_button_1, ui_->spell_button_2, ui_->spell_button_3, ui_->spell_button_4};
    for (size_t i = 0; i < spell_buttons.size(); ++i){
        spell_buttons.at(i)->setProperty("spell_id", static_cast<int>(i));
        connect(spell_buttons.at(i), &QPushButton::clicked, this, &GameWindow::onSpellButtonClicked);
    }

    vector<pair<QPushButton*, SceneType>> town_resource_buttons = {
        {ui_->troop_buying_button, TOWN_TROOP_BUYING},
        {ui_->hero_buying_button, TOWN_HERO_BUYING},
        {ui_->spell_buying_button, TOWN_SPELL_BUYING},
        {ui_->town_troop_return_button, TOWN},
        {ui_->town_hero_return_button, TOWN},
        {ui_->town_spell_return_button, TOWN}
    };

    for (const auto& button_pair : town_resource_buttons) {
        button_pair.first->setProperty("game_scene_id", button_pair.second);
        connect(button_pair.first, &QPushButton::clicked, this, &GameWindow::onTownResourceBuyingButtonClicked);
    }

    connect(ui_->world_end_turn_button, &QPushButton::clicked, this, &GameWindow::onWorldEndTurnButtonClicked);
    connect(ui_->battle_end_turn_button, &QPushButton::clicked, this, &GameWindow::onBattleEndTurnButtonClicked);

    vector<pair<QPushButton*, TownDecision>> town_decision_buttons = {
        {ui_->buy_troop_1_button, BUY_TROOP_1},
        {ui_->buy_troop_2_button, BUY_TROOP_2},
        {ui_->buy_troop_3_button, BUY_TROOP_3},
        {ui_->buy_troop_4_button, BUY_TROOP_4},
        {ui_->buy_troop_5_button, BUY_TROOP_5},
        {ui_->buy_troop_6_button, BUY_TROOP_6},
        {ui_->buy_troop_7_button, BUY_TROOP_7},
        {ui_->buy_hero_1_button, BUY_HERO_1},
        {ui_->buy_hero_2_button, BUY_HERO_2},
        {ui_->buy_hero_3_button, BUY_HERO_3},
        {ui_->buy_spell_1_button, BUY_SPELL_1},
        {ui_->buy_spell_2_button, BUY_SPELL_2},
        {ui_->buy_spell_3_button, BUY_SPELL_3},
        {ui_->buy_spell_4_button, BUY_SPELL_4},
        {ui_->buy_spell_5_button, BUY_SPELL_5},
        {ui_->buy_spell_6_button, BUY_SPELL_6},
        {ui_->buy_spell_7_button, BUY_SPELL_7},
        {ui_->buy_spell_8_button, BUY_SPELL_8},
        {ui_->buy_spell_9_button, BUY_SPELL_9},
        {ui_->hall_upgrade_button, UPGRADE_HALL},
        {ui_->fortifications_upgrade_button, UPGRADE_FORTIFICATIONS},
        {ui_->tavern_upgrade_button, UPGRADE_TAVERN},
        {ui_->mage_guild_upgrade_button, UPGRADE_MAGE_GUILD},
        {ui_->dwellings_upgrade_button, UPGRADE_DWELLINGS},
        {ui_->town_leave_button, LEAVE_TOWN}
    };

    for (const auto& button_pair : town_decision_buttons) {
        prepareTownDecisionButton(button_pair.first, button_pair.second);
    }


    disableLayout(ui_->spell_layout);

    vector<pair<QLayout*, int>> layouts_to_add_labels = {
        {ui_->town_troop_data, MAX_NUMBER_OF_STACKS},
        {ui_->world_troop_layout, MAX_NUMBER_OF_STACKS},
        {ui_->world_decision_troop_layout, MAX_NUMBER_OF_STACKS},
        {ui_->world_hero_ui, HERO_STATISTICS_DISPLAY_COUNT + 3},
        {ui_->world_decision_hero_ui, HERO_STATISTICS_DISPLAY_COUNT + 3},
        {ui_->battle_order_layout, BATTLE_ORDER_UI_DISPLAY_COUNT},
        {ui_->battle_statistics_names_layout, BATTLE_STAT_TEXT_LAYOUT_SIZE},
        {ui_->battle_statistics_values_layout, BATTLE_STAT_TEXT_LAYOUT_SIZE}
    };

    for (const auto& layout_pair : layouts_to_add_labels) {
        addLabelsToLayout(layout_pair.first, layout_pair.second);
    }

    addLabelsToGridLayout(ui_->town_spell_layout, 2, 2);
    addLabelsToGridLayout(ui_->world_spell_layout, 2, 2);
    addLabelsToGridLayout(ui_->world_decision_spell_layout, 2, 2);

    vector<QPushButton*> decision_option_buttons = {
        ui_->decision_option_button_1,
        ui_->decision_option_button_2,
        ui_->decision_option_button_3
    };

    for(size_t i=0; i<decision_option_buttons.size(); ++i){
        prepareDecisionOptionButton(decision_option_buttons.at(i), i);
    }
}

Ui::GameWindow* GameWindow::getUI(){
    return ui_.get();
}

void GameWindow::closeEvent(QCloseEvent* event){
    event->accept();
    QApplication::quit();
    exit(0);
}

void GameWindow::onSpellButtonClicked(){
    QPushButton *clicked_button = qobject_cast<QPushButton*>(sender());
    int spell_id = clicked_button->property("spell_id").toInt();
    if(GraphicsManager::getInstance().getFlagIsUIActive()){
        BattleManager::getInstance().getCurrentBattle().getCurrentBattleController().setDecisionVars(spell_id);
    }
}

void GameWindow::onTownResourceBuyingButtonClicked(){
    QPushButton *clicked_button = qobject_cast<QPushButton*>(sender());
    SceneType new_game_scene = static_cast<SceneType>(clicked_button->property("game_scene_id").toInt());
    GraphicsManager::getInstance().setToScene(new_game_scene);
}

void GameWindow::addLabelsToGridLayout(QGridLayout* const layout, const int width, const int height){
    for(int i=0; i<height; ++i){
        for(int j=0; j<width; ++j){
            layout->addWidget(new QLabel(), i, j);
        }
    }
}

void GameWindow::disableSpellLayout(){
    disableLayout(ui_->spell_layout);
}

void GameWindow::enableSpellLayout(){
    enableLayout(ui_->spell_layout);
}

void GameWindow::displayBattleObjectTextData(QLayout* battle_object_text_data_names_layout,
    QLayout* battle_object_text_data_values_layout, const BattleObject& battle_object){

    const auto display_data = battle_object.getDataText();

    for(size_t i=0; i<display_data.first.size(); ++i){
        setTextOnLabel(layoutItemToQLabel(battle_object_text_data_names_layout->itemAt(i)),    display_data.first.at(i),    Qt::AlignLeft);
        setTextOnLabel(layoutItemToQLabel(battle_object_text_data_values_layout->itemAt(i)),   display_data.second.at(i),   Qt::AlignLeft);
    }
    for(size_t i=display_data.first.size(); i<static_cast<size_t>(battle_object_text_data_names_layout->count()); ++i){
        setTextOnLabel(layoutItemToQLabel(battle_object_text_data_names_layout->itemAt(i)),  "");
        setTextOnLabel(layoutItemToQLabel(battle_object_text_data_values_layout->itemAt(i)), "");
    }
}


void GameWindow::prepareTownDecisionButton(QPushButton* button, TownDecision decision){
    button->setProperty("town_decision_id", decision);
    connect(button, &QPushButton::clicked, this, &GameWindow::onTownDecisionButtonClicked);
}

void GameWindow::onTownDecisionButtonClicked(){
    QPushButton *clicked_button = qobject_cast<QPushButton*>(sender());
    int decision = clicked_button->property("town_decision_id").toInt();
    WorldManager::getInstance().getCurrentPlayer().getPlayerController().setDecisionVars(decision);
}

void GameWindow::onWorldEndTurnButtonClicked(){
    WorldManager::getInstance().getCurrentPlayer().getPlayerController().setDecisionVars(END_TURN_INDICATOR);
}

void GameWindow::onBattleEndTurnButtonClicked(){
    BattleManager::getInstance().getCurrentBattle().getCurrentBattleController().setDecisionVars(END_TURN_INDICATOR);
}

void GameWindow::setSpellsImagesOnButtons(Hero& hero){
    vector<QPushButton*> spell_buttons = {ui_->spell_button_1, ui_->spell_button_2, ui_->spell_button_3, ui_->spell_button_4};
    for(int i=0; i<MAX_NUMBER_OF_SPELLS; ++i){
        if(hero.hasSpellAt(i)){
            QPixmap spell_pixmap(*GraphicsManager::getInstance().getTexture(hero.getSpellAt(i).getCurrentTextureName()));
            Painter painter(spell_pixmap);
            hero.getSpellAt(i).drawAdditionalData(painter);
            spell_buttons.at(i)->setIcon(spell_pixmap);
            spell_buttons.at(i)->setIconSize(spell_pixmap.rect().size());
            spell_buttons.at(i)->setFixedSize(TILE_SIZE, TILE_SIZE);
        }
    }
}

void GameWindow::onDecisionOptionButtonClicked(){
    QPushButton *clicked_button = qobject_cast<QPushButton*>(sender());
    int decision_option = clicked_button->property("decision_id").toInt();
    WorldManager::getInstance().getCurrentPlayer().getPlayerController().setDecisionVars(decision_option);
}

void GameWindow::prepareDecisionOptionButton(QPushButton* button, int decision_option_index){
 button->setProperty("decision_id", decision_option_index);
 button->setText(QString::fromStdString(DECISION_BUTTON_TEXT));
 connect(button, &QPushButton::clicked, this, &GameWindow::onDecisionOptionButtonClicked);
}

void GameWindow::keyPressEvent(QKeyEvent* event){
    if(GraphicsManager::getInstance().getCurrentScene() != WORLD){
        return;
    }

    switch(event->key()){

        case Qt::Key_Up:
            GraphicsManager::getInstance().addWorldMapOffsetY(-1);
            break;
        case Qt::Key_Down:
            GraphicsManager::getInstance().addWorldMapOffsetY(1);
            break;
        case Qt::Key_Left:
            GraphicsManager::getInstance().addWorldMapOffsetX(-1);
            break;
        case Qt::Key_Right:
            GraphicsManager::getInstance().addWorldMapOffsetX(1);
            break;
        case Qt::Key_Space:
            GraphicsManager::getInstance().resetWorldMapOffset();
            break;
        default:
            break;
    }
}
