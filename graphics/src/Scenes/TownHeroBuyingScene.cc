/**
 * @file TownHeroBuyingScene.cc
 * @author Bartosz Han
 * @brief This file contains definition of class TownHeroBuyingScene.
 *
 */


#include <QLayout>

#include "Managers/TownManager.h"
#include "Scenes/TownHeroBuyingScene.h"
#include "UIUtilities.hpp"
#include "GameWindow.h"

using namespace std;
using namespace game_data;
using namespace ui_constants;

TownHeroBuyingScene::TownHeroBuyingScene(): Scene(TOWN_HERO_BUYING){}

void TownHeroBuyingScene::drawScene(){

    Ui::GameWindow* ui = GraphicsManager::getInstance().getGameWindow()->getUI();
    vector<QLayout*> hero_layouts = {ui->town_hero_1_layout, ui->town_hero_2_layout, ui->town_hero_3_layout};
    int current_index = 0;
    for(const HeroType& available_hero: TownManager::getInstance().getCurrentTown().getTownResources().heroes_in_tavern_){
        if(available_hero != INVALID_HERO){
            drawTownHeroDataSingleLayout(hero_layouts.at(current_index), available_hero);
            ++current_index;
        }
    }
    while(static_cast<size_t>(current_index) < hero_layouts.size()){
        clearLayout(hero_layouts.at(current_index));
        ++current_index;
    }
}

void TownHeroBuyingScene::drawTownHeroDataSingleLayout(QLayout* layout, HeroType hero_type){
    HeroData hero_data = getHeroData(hero_type);
    string texture_name = hero_data.texture_name;
    string hero_name = hero_data.hero_name;
    ResourceArray cost_of_hero = COST_OF_BUYING_HERO;
    string button_text = TOWN_HERO_CAN_BUY_BUTTON_TEXT + hero_name;


    setImageOnLabel(layoutItemToQLabel(layout->itemAt(0)), texture_name);
    setTextOnLabel(layoutItemToQLabel(layout->itemAt(1)), hero_name);
    setImageOnLabel(layoutItemToQLabel(layout->itemAt(2)), generateResourceArrayImage(cost_of_hero));
    static_cast<QPushButton*>(layout->itemAt(3)->widget())->setText(QString::fromStdString(button_text));
}
