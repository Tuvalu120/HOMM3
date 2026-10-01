/**
 * @file TownMainScene.cc
 * @author Bartosz Han
 * @brief This file contains definition of class TownMainScene.
 *
 */


#include "Scenes/TownMainScene.h"
#include "UIUtilities.hpp"
#include "GameWindow.h"
#include "Managers/TownManager.h"

using namespace std;

TownMainScene::TownMainScene():Scene(TOWN){};

void TownMainScene::drawScene(){
    const Town& current_town = TownManager::getInstance().getCurrentTown();
    Ui::GameWindow* ui = GraphicsManager::getInstance().getGameWindow()->getUI();

    vector<pair<QLayout*, BuildingType>> building_layouts = {
        {ui->hall_layout, HALL},
        {ui->fortifications_layout, FORTIFICATIONS},
        {ui->tavern_layout, TAVERN},
        {ui->mage_guild_layout, MAGE_GUILD},
        {ui->dwellings_layout, DWELLINGS}
    };

    for (const auto& layout : building_layouts) {
        drawTownBuildingDataSingleLayout(layout.first, layout.second, current_town);
    }

    drawArmyData(ui->town_troop_data, ui->town_resources_label, ui->town_spell_layout);
}
