/**
 * @file TreasureChest.cc
 * @author Bartosz Han
 * @brief This file contains definition of class TreasureChest.
 *
 */


#include "GameObjects/WorldObjects/TreasureChest.h"
#include "Data/GameData.hpp"
#include "Decisions/Decision.h"
#include "Managers/DecisionManager.h"

using namespace std;
using namespace constants;
using namespace game_data;

TreasureChest::TreasureChest():
    WorldGeneratedInteractiveObject(TREASURE_CHEST_IMAGE, CHESTS_OBJECT_TYPE,
    priority_=OTHER_OBJECT_PRIORITY, destroy_after_interaction_=false){};

void TreasureChest::interact(){
    unique_ptr<ChestDecision> decision = make_unique<ChestDecision>();
    DecisionManager::getInstance().makeDecision(std::move(decision));
    setFlagDestroyAfterInteraction();
}