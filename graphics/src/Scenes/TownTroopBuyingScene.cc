/**
 * @file TownTroopBuyingScene.cc
 * @author Bartosz Han
 * @brief This file contains definition of class TownTroopBuyingScene.
 *
 */


#include "Scenes/TownTroopBuyingScene.h"
#include "UIUtilities.hpp"
#include "GameWindow.h"
#include "GameObjects/Town.h"
#include "Managers/TownManager.h"
#include "Data/GameData.hpp"

using namespace std;
using namespace game_data;
using namespace constants;
using namespace ui_constants;

TownTroopBuyingScene::TownTroopBuyingScene():Scene(TOWN_TROOP_BUYING){};

void TownTroopBuyingScene::drawScene(){

    Ui::GameWindow* ui = GraphicsManager::getInstance().getGameWindow()->getUI();
    vector<pair<QLayout*, int>> town_troop_layouts = {
        {ui->town_troop_1_layout, 0},
        {ui->town_troop_2_layout, 1},
        {ui->town_troop_3_layout, 2},
        {ui->town_troop_4_layout, 3},
        {ui->town_troop_5_layout, 4},
        {ui->town_troop_6_layout, 5},
        {ui->town_troop_7_layout, 6}
    };

    for (const auto& layout : town_troop_layouts) {
        drawTownTroopDataSingleLayout(layout.first, layout.second);
    }
}

void TownTroopBuyingScene::drawTownTroopDataSingleLayout(QLayout* layout, const int index){

    const Town& current_town = TownManager::getInstance().getCurrentTown();
    const TroopType current_troop_type = current_town.getTownData().town_troops.at(index);
    const auto troop_quantity_in_town = current_town.getTownResources().troops_available_to_buy_.at(index).second;
    ResourceArray troop_cost = current_town.getTownData().cost_of_buying_troop.at(index);
    string troop_name = getTroopName(current_troop_type);

    string button_text = current_town.canBuyTroopAtIndex(index) ? TOWN_TROOP_CAN_BUY_BUTTON_TEXT + troop_name
        : TOWN_TROOP_CANNOT_BUY_BUTTON_TEXT;

    setImageOnLabel(layoutItemToQLabel(layout->itemAt(0)), getTroopTextureName(current_troop_type));
    setTextOnLabel(layoutItemToQLabel(layout->itemAt(1)), troop_name);
    setTextOnLabel(layoutItemToQLabel(layout->itemAt(2)), TOWN_TROOP_AVAILABLE_INFO + to_string(troop_quantity_in_town));
    if (troop_cost == EMPTY_RESOURCE_ARRAY){
        setImageOnLabel(layoutItemToQLabel(layout->itemAt(3)), generateResourceArrayImage(troop_cost, 1.0f, true, true));
    }
    else{
        setImageOnLabel(layoutItemToQLabel(layout->itemAt(3)), generateResourceArrayImage(troop_cost));
    }
    static_cast<QPushButton*>(layout->itemAt(4)->widget())->setText(QString::fromStdString(button_text));
}
