/**
 * @file TownSpellBuyingScene.cc
 * @author Bartosz Han
 * @brief This file contains definition of class TownSpellBuyingScene.
 *
 */


#include "Scenes/TownSpellBuyingScene.h"
#include "Managers/TownManager.h"
#include "UIUtilities.hpp"
#include "GameWindow.h"

using namespace std;
using namespace game_data;
using namespace ui_constants;

TownSpellBuyingScene::TownSpellBuyingScene():Scene(TOWN_SPELL_BUYING){}

void TownSpellBuyingScene::drawScene(){
    Ui::GameWindow * ui = GraphicsManager::getInstance().getGameWindow()->getUI();

    vector<QLayout*> spell_layouts = {ui->town_spell_1_layout, ui->town_spell_2_layout, ui->town_spell_3_layout, ui->town_spell_4_layout,
        ui->town_spell_5_layout, ui->town_spell_6_layout, ui->town_spell_7_layout, ui->town_spell_8_layout, ui->town_spell_9_layout};

        int current_index = 0;
        for(const pair<SpellType, int>& available_spell: TownManager::getInstance().getCurrentTown().getTownResources().spells_in_mage_guild_){

            drawTownSpellDataSingleLayout(spell_layouts.at(current_index), available_spell.first, available_spell.second);
            ++current_index;
        }
        while(static_cast<size_t>(current_index) < spell_layouts.size()){
            clearLayout(spell_layouts.at(current_index));
            ++current_index;
        }
}

void TownSpellBuyingScene::drawTownSpellDataSingleLayout(QLayout* layout, const SpellType spell_type, const int spell_level){

    SpellData spell_battle_data = getSpellData(spell_type);
    string texture_name = spell_battle_data.texture_name;
    string spell_name = createSpellDesc(spell_type, spell_level);
    string spell_description = spell_battle_data.spell_description;
    ResourceArray spell_cost = COST_OF_BUYING_SPELL;
    string button_text = TOWN_SPELL_CAN_BUY_BUTTON_TEXT + spell_name;

    setImageOnLabel(layoutItemToQLabel(layout->itemAt(0)), texture_name);
    setTextOnLabel(layoutItemToQLabel(layout->itemAt(1)), spell_name);
    setTextOnLabel(layoutItemToQLabel(layout->itemAt(2)), spell_description);
    setImageOnLabel(layoutItemToQLabel(layout->itemAt(3)), generateResourceArrayImage(spell_cost));
    static_cast<QPushButton*>(layout->itemAt(4)->widget())->setText(QString::fromStdString(button_text));
};
