/**
 * @file BattleScene.cc
 * @author Bartosz Han
 * @brief This file contains definition of class BattleScene.
 *
 */


#include "Scenes/BattleScene.h"
#include "Data/Constants.hpp"
#include "GameWindow.h"
#include "CanvasTileMapLabel.h"
#include "UIUtilities.hpp"
#include "GameObjects/TileMaps/Tile.h"
#include "Managers/GraphicsManager.h"
#include "Painter.h"

using namespace std;
using namespace constants;
using namespace ui_constants;

BattleScene::BattleScene():
    CanvasTileMapScene(BATTLE, BATTLEFIELD_WIDTH, BATTLEFIELD_HEIGHT){

    for (int i=0 ; i<getCanvasWidth() * getCanvasHeight() ; ++i){
            canvas_tiles_.push_back(make_unique<BattleCanvasTileMapLabel>(*this));
        }

        for (int i = 0; i < getCanvasWidth(); ++i){
            for(int j=0; j < getCanvasHeight(); ++j){
                getCanvasTileAt(i, j).setFixedHeight(64);
                getCanvasTileAt(i, j).setFixedWidth(64);
                GraphicsManager::getInstance().getGameWindow()->getUI()->battle_tile_map->addWidget(&getCanvasTileAt(i, j),j,i);
            }
        }
}

void BattleScene::drawScene(){
        if(BattleManager::getInstance().getCurrentBattle().isBattlefieldInitialized()){

            for (int i = 0; i< getCanvasWidth(); ++i){
                for(int j = 0; j < getCanvasHeight(); ++j){
                    Tile& current_tile = BattleManager::getInstance().getCurrentBattle().getBattlefield().getTileAt(i, j);
                    drawOnTile(current_tile, i, j);
                    getCanvasTileAt(i, j).show();
                }
            }

            drawBattleOrder(GraphicsManager::getInstance().getGameWindow()->getUI()->battle_order_layout);
            GraphicsManager::getInstance().getGameWindow()->displayBattleObjectTextData(
            GraphicsManager::getInstance().getGameWindow()->getUI()->battle_statistics_names_layout,
            GraphicsManager::getInstance().getGameWindow()->getUI()->battle_statistics_values_layout, BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject());

            auto attacker_hero = BattleManager::getInstance().getAttackingArmy().getArmyHeroPtr().get();
            auto defender_hero = BattleManager::getInstance().getDefendingArmy().getArmyHeroPtr().get();
            if(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() == attacker_hero){
                GraphicsManager::getInstance().getGameWindow()->setSpellsImagesOnButtons(*attacker_hero);
            }
            else if(&BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject() == defender_hero){
                GraphicsManager::getInstance().getGameWindow()->setSpellsImagesOnButtons(*defender_hero);
            }
            if(BattleManager::getInstance().getCurrentBattle().getFlagIsPlayerTurnFinished()){
                GraphicsManager::getInstance().getGameWindow()->disableSpellLayout();
            }
        }
        else{
            throw BattleLogicError("Tried to draw Battle Scene without initialized Battlefield");
        }
    }

void BattleScene::drawOnTile(Tile& tile_to_draw, const int canvas_tile_x_pos, const int canvas_tile_y_pos){

        QPixmap final_pixmap;

        auto attacker_hero = BattleManager::getInstance().getAttackingArmy().getArmyHeroPtr().get();
        auto defender_hero = BattleManager::getInstance().getDefendingArmy().getArmyHeroPtr().get();

        if (tile_to_draw.getXPos() == BattleManager::getInstance().getCurrentBattle().getBattlefield().getAttackerHeroSpawningPos().first &&
                tile_to_draw.getYPos() == BattleManager::getInstance().getCurrentBattle().getBattlefield().getAttackerHeroSpawningPos().second && attacker_hero != nullptr){

            final_pixmap = CanvasTileMapScene::generateTilePixmap(tile_to_draw, attacker_hero->getCurrentTextureName());
        }

        else if (tile_to_draw.getXPos() == BattleManager::getInstance().getCurrentBattle().getBattlefield().getDefenderHeroSpawningPos().first &&
                tile_to_draw.getYPos() == BattleManager::getInstance().getCurrentBattle().getBattlefield().getDefenderHeroSpawningPos().second && defender_hero != nullptr){

            final_pixmap = CanvasTileMapScene::generateTilePixmap(tile_to_draw, defender_hero->getCurrentTextureName(), true);
        }

        else if(tile_to_draw.hasInteractiveObject()){
            bool flag_is_pixmap_generated = false;
            InteractiveObject& object_on_tile = tile_to_draw.getObjectOnTile();
            Painter painter(final_pixmap);
            for(const auto &stack: BattleManager::getInstance().getDefendingArmy().getArmyStacks()){
                if (stack->getXPos() == object_on_tile.getXPos() && stack->getYPos() == object_on_tile.getYPos()){
                    final_pixmap = CanvasTileMapScene::generateTilePixmap(tile_to_draw, tile_to_draw.getObjectOnTile().getCurrentTextureName(), true);
                    flag_is_pixmap_generated = true;
                    stack->drawAdditionalData(painter);
                    break;
                }
            }
            if (!flag_is_pixmap_generated){
                for(const auto &stack: BattleManager::getInstance().getAttackingArmy().getArmyStacks()){
                    if (stack->getXPos() == object_on_tile.getXPos() && stack->getYPos() == object_on_tile.getYPos()){
                        final_pixmap = CanvasTileMapScene::generateTilePixmap(tile_to_draw, tile_to_draw.getObjectOnTile().getCurrentTextureName());
                        stack->drawAdditionalData(painter);
                        break;
                    }
                }
            }
        }

        else{
            float darken_factor = tile_to_draw.getTileType() == WORLD_BORDER_TILE ? TILE_DARKEN_FACTOR : 0.0f;
            final_pixmap = CanvasTileMapScene::generateTilePixmap(tile_to_draw, tile_to_draw.getAdditionalTextureName(), false, darken_factor);
        }

        getCanvasTileAt(canvas_tile_x_pos, canvas_tile_y_pos).setPixmap(final_pixmap);

}

pair<int, int> BattleScene::findTileMapPosOfCanvasTile(QLabel& tile) const{
    for (int i = 0; i< getCanvasWidth(); ++i){
        for(int j = 0; j < getCanvasHeight(); ++j){
            if (&getCanvasTileAt(i, j) == &tile){
                return pair<int,int>(i,j);
            }
        }
    }

    return pair<int,int>(-1,-1);
}
